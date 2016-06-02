/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: This file contains the main function for a AKHFTP server.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    int serv_sock, option;
    struct sockaddr_in serv_adr, clnt_adr;
    char pac[MAX_BUFFER_SIZE];
    socklen_t clnt_adr_sz, optlen;

    // create server socket
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("UDP socket creation error");

    // set socket so that port can be reuse immediately after closure
    option = true;
    optlen = sizeof(option);
    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen) == -1 )
	    error_handling("Setting socket option to enable reusable port error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    int request_type = handle_request(serv_sock, &clnt_adr, &clnt_adr_sz, filename, &filesize);

    if(request_type == RD) {
        connection_download_server(serv_sock, &clnt_adr, &clnt_adr_sz, filename, &filesize);

        akh_disconn_response disconn_response;
        disconn_response.segment_list = NULL;

        while(wait_disconnection(serv_sock, &clnt_adr, &clnt_adr_sz, &disconn_response) != 0) {
            send_file(serv_sock, &clnt_adr, filename, &disconn_response);
            request_close(serv_sock, &clnt_adr);
        }
    }
    else if(request_type == RU) {
        connection_upload_server(serv_sock, &clnt_adr, &clnt_adr_sz);

        uint32_t body_size = 10; 
        int num_time_out = 0;
        while(num_time_out < 3 && handle_request_close(serv_sock, &clnt_adr, filename, filesize, 10) != 0) {
            int msg_type = receive_file(serv_sock, &clnt_adr, &clnt_adr_sz, filename, body_size);
            if(msg_type == -1) // problem in connection
                num_time_out++;
            else
                num_time_out = 0;
 
        }
        if(num_time_out > 2)
            error_handling("connection error");
    }

    close(serv_sock);
    return 0;
}

