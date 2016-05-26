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
        printf("Usage: %s <IP> <port> <option>\n", argv[0]);
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
        printf("download request\n");
        printf("filename: %s\n", argv[4]);


        if( (filesize = connection_download_client(&sock, &serv_adr, filename)) == 0) {
    	    error_handling("fail connection error");
        }
        printf("filesize: %d\n", filesize);

        recieve_file();
        disconnection_reciever();
    }

    else if(strcmp(argv[3], "-u") == 0) {
        printf("upload request\n");
        printf("filename: %s\n", argv[4]);

        connection_upload_client();
        send_file();
        disconnection_sender();
    }

    close(sock);

    return 0;


    /* socklen_t adr_sz; */
    /* struct sockaddr_in from_adr; */

    /* int sock; */
    /* socklen_t adr_sz; */
    /* struct sockaddr_in serv_adr, from_adr; */
    /* memset(&serv_adr, 0, sizeof(serv_adr)); */
    /* serv_adr.sin_family = AF_INET; */
    /* serv_adr.sin_addr.s_addr = inet_addr(argv[1]); */
    /* serv_adr.sin_port = htons(atoi(argv[2])); */
    /*  */
    /* sock = socket(PF_INET, SOCK_DGRAM, 0); */
    /* if(sock == -1) { */
    /* 	error_handling("create socket error"); */
    /* } */


    /* // request download */
    /* akh_pdu_header header = createHeader(RD, randNum()); */
    /* akh_pdu_body body = "test_main.c"; */
    /* packet pac; */
    /* size_t pac_len = createPacket(&pac, header, body, strlen(body)); */
    /*  */
    /* char response[MAX_BUFFER_SIZE]; */
    /* unsigned int str_len; */
    /*  */
    /* // send RD to server */
    /* sendto(sock, pac, pac_len, 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr)); */
    /* str_len = recvfrom(sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&from_adr, &adr_sz); */
    /*  */
    /* // get file size */
    /* char fsize[4]; */
    /* int file_size; */
    /* memcpy(fsize, response + 12, 4); */
    /* file_size = atoi(fsize); */
    /* printf("Message from server: %s\n", response); */
    /* printf("File size: %d\n", file_size); */
    /*  */
    /* // set up buffer */
    /* char buffer[MAX_BUFFER_SIZE]; */
    /* memset(&buffer, 0, sizeof(char) * MAX_BUFFER_SIZE); */
    /*  */
    /* // send RS to request file segment */
    /* uint32_t seq_num = 12345; */
    /* header = createHeader(RS, seq_num); */
    /* body = "1024"; // 1 kb */
    /* pac_len = createPacket(&pac, header, body, strlen(body)); */
    /*  */
    /* close(sock); */
    /* deletePacket(pac); */
    /* return 0; */
}

