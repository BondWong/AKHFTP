#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "message.h"
#include "message_util.h"
#include "error_handling.h"

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int serv_sock;
    char message[20] = "Thanks!";
    char buf[20];
    size_t pac_len;
    int str_len;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_adr, clnt_adr;

    char pac[MAX_BUFFER_SIZE];
    // create server socket
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("UDP socket creation error");
    // set socket so that port can be reuse immediately after closure
    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) == -1 ){
	error_handling("Setting socket option to enable reusable port error");	
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    clnt_adr_sz = sizeof(clnt_adr);
    pac_len = recvfrom(serv_sock, pac, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
    displayHeader(*(akh_pdu_header *)pac);
    printf("body => %s\n", pac + sizeof(akh_pdu_header));

    sendto(serv_sock, message, strlen(message), 0, (struct sockaddr *)&clnt_adr, clnt_adr_sz);


    close(serv_sock);
    return 0;
}

