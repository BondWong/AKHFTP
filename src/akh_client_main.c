#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "security_util.h"
#include "connection.h"
#include "file_transmission.h"
#include "disconnection.h"

int main(int argc, char *argv[])
{
    if(argc != 5) {
        printf("Usage: %s <IP> <port> <option> <filename>\n", argv[0]);
        printf("<option>\n");
        printf("-d <filename>: download file\n");
        printf("-u <filename>: upload file\n");
    }

    char *filename = argv[4];
    int sock;
    struct sockaddr_in serv_adr;
    off_t filesize;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    sock = socket(PF_INET, SOCK_DGRAM, 0); // create UDP socket with queue size 0
    if(sock == -1) {
    	error_handling("create socket error");
    }
    printf("sock => %d\n", sock);

    if(strcmp(argv[3], "-d") == 0) {
        printf("< download request >\n");

        if( (filesize = connection_download_client(&sock, &serv_adr, filename)) == 0) {
    	    error_handling("fail connection error");
        }

        recieve_file();
        disconnection_reciever();
    }

    else if(strcmp(argv[3], "-u") == 0) {
        printf("< upload request >\n");

        connection_upload_client();
        send_file();
        disconnection_sender();
    }

    close(sock);

    return 0;

}

