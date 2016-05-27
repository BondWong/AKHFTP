#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "security_util.h"
#include "connection.h"
#include "network_util.h"

// when client makes connection with server for download
off_t connection_download_client(int *sock, struct sockaddr_in *serv_adr, char *filename)
{
    // create RD package
    akh_pdu_header header = createHeader(RD, randNum());
    akh_pdu_body body = filename;

    packet pac;
    size_t pac_len = createPacket(&pac, header, body, strlen(body));

    socklen_t adr_sz;
    struct sockaddr_in from_adr;
    unsigned int str_len;
    char response[MAX_BUFFER_SIZE];

    printf("\nrequest pac\n");
    displayHeader(*(akh_pdu_header *)pac);

    // send RD package to server and waiting for server's response
    akh_send(sock, pac, pac_len, 0, 0, (struct sockaddr *) serv_adr, (struct sockaddr *)&from_adr, &adr_sz, response, MAX_BUFFER_SIZE);

    printf("\nresponse pac\n");
    displayHeader(*(akh_pdu_header *)response);
    uint16_t msg_type = (*(akh_pdu_header *)response).msg_type;
    if(msg_type != AD) {
        printf("rejected by server\n");
        return 0;
    }

    off_t filesize = *(off_t *)( (akh_pdu_header *)response + 1 );

    return filesize;
}

// when client requests download, server uses the function to make connection
int connection_download_server()
{
    printf("server: receive download request\n");
    return 0;
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


