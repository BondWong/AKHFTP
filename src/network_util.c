#include <sys/time.h>
#include <sys/types.h>

#define MAX_RETRY 10 // retry 10 times at most

void send(int sock, packet pac, size_t pac_len, int send_options, int rece_options, struct sockaddr_in serv_adr, struct sockaddr_in from_adr, socklen_t adr_sz, char* response, int response_sz) {
	int result;
	fd_set sockset;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&sockset);
	FD_SET(sock, &sockset);
  
	sendto(sock, pac, pac_len, send_options, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	result = select( + 1, &sockset, NULL, NULL, &tv);
	if(result == - 1) {
		error_handling("create select error");
	} else if(result == 1) {
		str_len = recvfrom(, response, response_sz, rece_options, (struct sockaddr *)&from_adr, &adr_sz);
		response[str_len] = 0;
	} else {
		printf("timeout");
	}
}

