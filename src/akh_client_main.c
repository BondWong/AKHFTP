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
    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    akh_pdu_header header = createHeader(RD, 0);
    akh_pdu_body body = "Hello World";
    packet pac;
    size_t pac_len = createPacket(&pac, header, body, strlen(body));
    /* printf("pac_len => %d\n", pac_len); */
    /* displayHeader(*(akh_pdu_header *)pac); */
    /* printf("body => %s\n", pac + sizeof(header)); */


    int sock;
    socklen_t adr_sz;
    struct sockaddr_in serv_adr, from_adr;
    char message[20] = "hello world!!!";
    int str_len;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));


    sendto(sock, pac, pac_len, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
    str_len = recvfrom(sock, message, 20, 0, (struct sockaddr *)&from_adr, &adr_sz);
    message[str_len] = 0;
    printf("Message from server: %s\n", message);

    close(sock);
    deletePacket(pac);
    return 0;
}

