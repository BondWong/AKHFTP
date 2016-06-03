/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: This file contains the main function for a AKHFTP client.
 * */
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
    if(argc != 5 && argc != 1) {
        printf("Usage: %s <IP> <port> <option> <filename>\n", argv[0]);
        printf("<option>\n");
        printf("-d <filename>: download file\n");
        printf("-u <filename>: upload file\n");
    }
    // create a command line UI
    char host_ip[16];
    char host_port[6];
    char option[3];
    char filename[MAX_FILENAME_SIZE];
    if (argc == 1){
        printf("Please input IP address of the server\n");
        scanf("%s", host_ip);
        printf("Please input port number of the service (default 9190)\n");
        scanf("%s", host_port);
        printf("Please input client options '-d' or '-u'\n");
        scanf("%s", option);
        printf("Please input file name (e.g. Download: 'server.txt' or Upload: 'client.txt)\n");
        scanf("%s", filename);
    }
    // $make run_client_download or $make run_client_upload
    if (argc == 5){
        strcpy(host_ip, argv[1]);
        strcpy(host_port, argv[2]);
        strcpy(option, argv[3]);
        strcpy(filename, argv[4]);
    }
    int sock;
    struct sockaddr_in serv_adr;
    socklen_t serv_adr_sz;
    off_t filesize;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    /**************** CLIENT *******************/
    serv_adr.sin_addr.s_addr = inet_addr(host_ip);
    serv_adr.sin_port = htons(atoi(host_port));

    sock = socket(PF_INET, SOCK_DGRAM, 0); // create UDP socket with queue size 0
    if(sock == -1) {
    	error_handling("create socket error");
    }
    
    struct sockaddr created_socket_addr;
    socklen_t created_socket_len;
    getsockname(sock, &created_socket_addr, &created_socket_len);
    char* sa_data = created_socket_addr.sa_data;

    if(strcmp(option, "-d") == 0) {
        if( (filesize = connection_download_client(sock, &serv_adr, filename)) == 0) {
    	    error_handling("sender does not have the file");
        }

        uint32_t body_size = 10; 
        int num_time_out = 0;
        while(num_time_out < 3 && handle_request_close(sock, &serv_adr, filename, filesize, body_size) != 0) {
            int msg_type = receive_file(sock, &serv_adr, &serv_adr_sz, filename, body_size);
            if(msg_type == -1) // problem in connection
                num_time_out++;
            else
                num_time_out = 0;
            /* else if(msg_type == RC) { // recieve request close */
            /*     num_time_out = 0; */
            /* } */
        }
        if(num_time_out > 2)
            error_handling("connection error");
    }

    else if(strcmp(option, "-u") == 0) {
        if(connection_upload_client(sock, &serv_adr, filename, &filesize) != 0)
            error_handling("rejected by receiver");

        akh_disconn_response disconn_response;
        disconn_response.segment_list = NULL;

        while(wait_disconnection(sock, &serv_adr, &serv_adr_sz, &disconn_response) != 0) {
            send_file(sock, &serv_adr, filename, &disconn_response);
            request_close(sock, &serv_adr);
        }
    }

    close(sock);

    return 0;
}

