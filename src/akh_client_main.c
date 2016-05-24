#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "message.h"
#include "message_util.h"
#include "error_handling.h"

#define MAX_BUFFER_SIZE 128 * 1024 // maximum buffer size 128 kb
int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    int sock;
    socklen_t adr_sz;
    struct sockaddr_in serv_adr, from_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));


    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
    	error_handling("create socket error");
    }

    // request download
    akh_pdu_header header = createHeader(RD, 0);
    akh_pdu_body body = "test.txt";
    packet pac;
    size_t pac_len = createPacket(&pac, header, body, strlen(body));

    char response[20];
    int str_len;

    // send RD to server
    sendto(sock, pac, pac_len, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
    str_len = recvfrom(sock, response, 20, 0, (struct sockaddr *)&from_adr, &adr_sz);
    response[str_len] = 0;

    // get file size
    char fsize[4];
    int file_size;
    memcpy(fsize, response + 12, 4);
    file_size = atoi(fsize);
    printf("Message from server: %s\n", response);
    printf("File size: %d\n", file_size);

    // set up buffer
    char buffer[MAX_BUFFER_SIZE];
    memset(&buffer, 0, sizeof(char) * MAX_BUFFER_SIZE);

    // send RS to request file segment
    uint32_t seq_num = 12345;
    header = createHeader(RS, seq_num);
    body = "1024"; // 1 kb
    pac_len = createPacket(&pac, header, body, strlen(body));

    close(sock);
    deletePacket(pac);
    return 0;
}

