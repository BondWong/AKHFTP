#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "file_util.h"
#include "connection.h"
#include "network_util.h"

// request handle
uint16_t handle_request(int *serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize)
{
    char pac[MAX_BUFFER_SIZE];
    size_t pac_len;
    *clnt_adr_sz = sizeof(struct sockaddr_in);
    pac_len = recvfrom(*serv_sock, pac, MAX_BUFFER_SIZE, 0, (struct sockaddr *)clnt_adr, clnt_adr_sz);
    
    uint16_t msg_type = ((akh_pdu_header *)pac)->msg_type;
    char *requested_file;

    if(msg_type == RD) {
        *filesize = 0;
        requested_file = pac + sizeof(akh_pdu_header);
    }
    else if(msg_type == RU) {
        *filesize = (off_t *)(pac + sizeof(akh_pdu_header));
        requested_file = pac + sizeof(akh_pdu_header) + sizeof(off_t);
    }
    else
        error_handling("wrong request type");

    strcpy(filename, requested_file);

    printf("requested packet info\n");
    displayHeader(*(akh_pdu_header *)pac);
    printf("filename: %s\n", filename);
    printf("filesize: %d\n", *filesize);

    return msg_type;
}


// when client makes connection with server for download
off_t connection_download_client(int *sock, struct sockaddr_in *serv_adr, char *filename)
{
    // create RD package
    akh_pdu_header header = createHeader(RD, randNum());
    akh_pdu_body body = filename;

    packet pac;
    size_t pac_len = createPacket(&pac, &header, body, strlen(body));

    socklen_t adr_sz;
    struct sockaddr_in from_adr;
    unsigned int str_len;
    char response[MAX_RESPONSE_SIZE];

    // send RD package to server and waiting for server's response
    akh_send(sock, pac, pac_len, 0, 0, (struct sockaddr *)serv_adr, (struct sockaddr *)&from_adr, &adr_sz, response, MAX_RESPONSE_SIZE);

    uint16_t msg_type = ((akh_pdu_header *)response)->msg_type;
    if(msg_type != AD) {
        printf("rejected by server\n");
        return 0;
    }

    off_t filesize = *(off_t *)( (akh_pdu_header *)response + 1 );

    printf("request pac\n");
    displayHeader(header);
    printf("filename: %s\n", body);

    printf("\nresponse pac\n");
    displayHeader(*(akh_pdu_header *)response);
    printf("file size %d\n", filesize);

    return filesize;
}


// when client requests download, server uses the function to make connection
void connection_download_server(int *serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize)
{
    printf("filename: %s\n", filename);
    akh_pdu_header header = createHeader(AD, randNum());
    *filesize = get_file_size(filename);
    packet response;
    size_t response_len = createPacket(&response, &header, (akh_pdu_body)filesize, sizeof(off_t));

    sendto(*serv_sock, response, response_len, 0, (struct sockaddr *)clnt_adr, *clnt_adr_sz);
    deletePacket(response);


    printf("server: receive download request\n");
    printf("filename: %s\n", filename);
    printf("filesize: %d\n", *filesize);
}


// when client makes connection with server for upload
int connection_upload_client()
{
    printf("client: request upload\n");
    return 0;
}

// when client requests upload, server uses the function to make connection
int connection_upload_server()
{
    printf("server: receive upload request\n");
    return 0;
}


