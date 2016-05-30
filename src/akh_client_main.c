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

    if(strcmp(argv[3], "-d") == 0) {
        if( (filesize = connection_download_client(sock, &serv_adr, filename)) == 0) {
    	    error_handling("sender does not have the file");
        }

        recieve_file();
//        //disconnection_reciever();
//        /* This is a test for disconnection_sender() function: (1) AC */
//        //create AC packet
//        akh_pdu_header AC_header = createHeader(AC,randNum());
//        packet AC_pac;
//        size_t AC_pac_len = createPacket(&AC_pac, &AC_header, NULL, 0);
//        //send AC packet to server
//        sendto(sock, AC_pac, AC_pac_len, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
//        deletePacket(AC_pac);
        /* This is a test for disconnection_sender() function: (2) RS */
        //create AC packet
        akh_pdu_header RS_header = createHeader(RS,randNum());
        packet RS_pac;
        int num_missing_segment = 2; // the number of missing segments
        char RS_body[sizeof(uint32_t)*(2+num_missing_segment)];
        *((uint32_t *) RS_body) = 10; // Segment size
        *((uint32_t *)(RS_body + sizeof(uint32_t))) = num_missing_segment; // Number of missing segments
        *((uint32_t *)(RS_body + sizeof(uint32_t)+sizeof(uint32_t))) = 111; //seq_num of the 1st missing segment
        *((uint32_t *)(RS_body + sizeof(uint32_t)+2*sizeof(uint32_t))) = 222; //seq_num of the 2nd missing segment
        size_t RS_body_len = sizeof(uint32_t)*(2*sizeof(uint32_t)+ *((uint32_t *)(RS_body + sizeof(uint32_t))));
        size_t RS_pac_len = createPacket(&RS_pac, &RS_header, RS_body, RS_body_len);
        //send AC packet to server
        sendto(sock, RS_pac, RS_pac_len, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
        deletePacket(RS_pac);
    }

    else if(strcmp(argv[3], "-u") == 0) {
        if(connection_upload_client(sock, &serv_adr, filename, &filesize) != 0)
            error_handling("rejected by receiver");

        send_file();
        //request disconnection and check if we need to retransmit missing segment
        akh_disconn_response disconn_response = disconnection_sender(&sock, &serv_adr);
    }

    close(sock);

    return 0;

}

