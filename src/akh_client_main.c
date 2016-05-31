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
    socklen_t serv_adr_sz;
    off_t filesize;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    sock = socket(PF_INET, SOCK_DGRAM, 0); // create UDP socket with queue size 0
    if(sock == -1) {
    	error_handling("create socket error");
    }

    if(strcmp(argv[3], "-d") == 0) {
        if( (filesize = connection_download_client(sock, &serv_adr, filename)) == 0) {
    	    error_handling("sender does not have the file");
        }

        while(handle_request_close(sock, &serv_adr, filename, filesize, 10) != 0) {
            /* int msg_type = test_receive_file(sock, &serv_adr, &serv_adr_sz); */
            int msg_type = receive_file(sock, &serv_adr, &serv_adr_sz, filename);
            /* if(msg_type == -1) { // problem in connection */
            /*     continue; */
            /* } */
            /* else if(msg_type == RC) { // recieve request close */
            /*     continue; */
            /* } */
        }
    }

    else if(strcmp(argv[3], "-u") == 0) {
        if(connection_upload_client(sock, &serv_adr, filename, &filesize) != 0)
            error_handling("rejected by receiver");

        akh_disconn_response disconn_response;
        disconn_response.segment_list = NULL;

        while(wait_disconnection(sock, &serv_adr, &serv_adr_sz, &disconn_response) != 0) {
            send_file();
            request_close(sock, &serv_adr);
        }

        /* send_file(); */
        //request disconnection and check if we need to retransmit missing segment
        /* akh_disconn_response disconn_response = disconnection_sender(&sock, &serv_adr); */
    }

    close(sock);

    return 0;
}

