#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"

#define MAX_RETRY 10 // retry 10 times at most


ssize_t timer_recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen, long sec, int num_try)
{
    int i, result;
    fd_set target, detection;
    struct timeval timeout;
    ssize_t buff_len;
    
    FD_ZERO(&target);
    FD_SET(sock, &target);

    for(i = 0; i < num_try; i++) {
        detection = target;
        timeout.tv_sec = sec;
        timeout.tv_usec = 0;

        result = select(sock + 1, &detection, 0, 0, &timeout);
        if(result == -1) {
            puts("select() error");
            break;
        }
        else if(result == 0) {
            puts("Time-out");
        }
        else {
            if(FD_ISSET(sock, &detection)) {
                return recvfrom(sock, buff, nbytes, flags, from, addrlen);
            }
        }
    }

    return -1;
}
    

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
