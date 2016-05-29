#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "file_util.h"
#include "security_util.h"
#include "connection.h"
#include "file_transmission.h"
#include "disconnection.h"


int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    char filename[MAX_FILENAME_SIZE];
    off_t filesize;
    int serv_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("UPD socket creation error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    int request_type = handle_request(serv_sock, &clnt_adr, &clnt_adr_sz, filename, &filesize);

    if(request_type == RD) {
        printf("< download request >\n");

        connection_download_server(&serv_sock, &clnt_adr, &clnt_adr_sz, filename, &filesize);
        send_file();
        disconnection_sender();
    }

    else if(request_type == RU) {
        printf("< upload request >\n");

        connection_upload_server();
        recieve_file();
        disconnection_reciever();
    }

    close(serv_sock);
    return 0;
}

