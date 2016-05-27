#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"

#define MAX_RETRY 10 // retry 10 times at most

void _akh_send(int* sock, packet pac, size_t pac_len, int send_options, int rece_options, 
	struct sockaddr* serv_adr, struct sockaddr* from_adr, socklen_t* adr_sz, char* response, 
	int response_sz, fd_set* sockset, struct timeval * tv, int* retry) {
	int result;
	
	sendto(*sock, pac, pac_len, send_options, serv_adr, sizeof(*serv_adr));
	deletePacket(pac);

	result = select(*sock + 1, sockset, NULL, NULL, tv);
	if(result == - 1) {
		error_handling("create select error");
	} else if(result) {
		int str_len;
		str_len = recvfrom(*sock, response, response_sz, rece_options, from_adr, adr_sz);
		response[str_len] = 0;
	} else {
		if(*retry < MAX_RETRY) {
			(*retry)++;
			// printf("retry...\n");
			_akh_send(sock, pac, pac_len, send_options, rece_options, serv_adr, from_adr, adr_sz, response, response_sz, sockset, tv, retry);
		}
		else printf("timeout\n");
	}
}

void akh_send(int* sock, packet pac, size_t pac_len, int send_options, int rece_options, 
	struct sockaddr* serv_adr, struct sockaddr* from_adr, socklen_t* adr_sz, char* response, int response_sz) {

	fd_set sockset;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	FD_ZERO(&sockset);
	FD_SET(*sock, &sockset);
	int retry = 0;

	_akh_send(sock, pac, pac_len, send_options, rece_options, serv_adr, from_adr, adr_sz, response, response_sz, &sockset, &tv, &retry);

}