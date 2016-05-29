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
    char pac[MAX_BUFFER_SIZE];
    socklen_t clnt_adr_sz;
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

    int request_type = handle_request(serv_sock, &clnt_adr, &clnt_adr_sz, filename, &filesize);

    if(request_type == RD) {
        printf("< download request >\n");

        connection_download_server(&serv_sock, &clnt_adr, &clnt_adr_sz, filename, &filesize);
        send_file();
        //request disconnection and check if we need to retransmit missing segment
        akh_disconn_response disconn_response = disconnection_sender(&serv_sock,&clnt_adr);
        if (disconn_response.response_type == AC){
            close(serv_sock);
            return 0;
        }
        else if (disconn_response.response_type == RS){
            printf("There are %u missing segments.\n",disconn_response.segment_num);
            printf("List of missing segment's sequence number:\n");
            uint32_t cnt;
            for (cnt=0; cnt < disconn_response.segment_num; cnt++ ) {
                printf("%u\n", *((uint32_t *)(disconn_response.segment_list + cnt*sizeof(uint32_t))));
            }
            // Finish processing the response, free memory
            free( disconn_response.segment_list );
        }
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

