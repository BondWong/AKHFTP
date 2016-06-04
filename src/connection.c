#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "file_util.h"
#include "connection.h"
#include "security_util.h"
#include "network_util.h"

#include "thread_util.h"

// request handle
uint16_t handle_request(int serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize)
{
    char pac[MAX_BUFFER_SIZE];
    size_t pac_len;
    *clnt_adr_sz = sizeof(struct sockaddr_in);
    pac_len = recvfrom(serv_sock, pac, MAX_BUFFER_SIZE, 0, (struct sockaddr *)clnt_adr, clnt_adr_sz);
    
    printPacket(pac, pac_len);
    
    uint16_t msg_type = ((akh_pdu_header *)pac)->msg_type;
    char *requested_file;

    if(msg_type == RD) {
        *filesize = 0;
        requested_file = pac + sizeof(akh_pdu_header);
    }
    else if(msg_type == RU) {
        *filesize = *((off_t *)(pac + sizeof(akh_pdu_header)));
        requested_file = pac + sizeof(akh_pdu_header) + sizeof(off_t);
    }
    else
        error_handling("wrong request type");

    strcpy(filename, requested_file);

    if(msg_type == RD)
        puts("< requested download >");
    else if(msg_type == RU)
        puts("< requested upload >");

    displayHeader(*(akh_pdu_header *)pac);
    printf("filename: %s\n", filename);
    printf("filesize: %d\n", *filesize);

    return msg_type;
}

// request handle
uint16_t handle_request_pipe(struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize)
{
    // unpack thread local storage, extract this thread's reading port.
    argstruct* tls = (argstruct*)pthread_getspecific(thread_key);
    int clnt_thread_read_fd = tls->clnt_pipe_read_fd;
    printf("handle request pipe: read fd = %d\n", clnt_thread_read_fd);
    // read the pipe with no timeout:
    
    clnt_thread_rcv_t pkg = read_with_timeout(clnt_thread_read_fd, 0, 0, 1);
    
    printf("read with time out finished...\n");

    char *pac;
    size_t pac_len;
    *clnt_adr_sz = sizeof(struct sockaddr_in);
    
    // unpack the pkg
    pac = pkg.pac;
    pac_len = pkg.pac_len;
    *clnt_adr = pkg.clnt_addr;
    
    uint16_t msg_type = ((akh_pdu_header *)pac)->msg_type;
    char *requested_file;

    if(msg_type == RD) {
        *filesize = 0;
        requested_file = pac + sizeof(akh_pdu_header);
    }
    else if(msg_type == RU) {
        *filesize = *((off_t *)(pac + sizeof(akh_pdu_header)));
        requested_file = pac + sizeof(akh_pdu_header) + sizeof(off_t);
    }
    else
        error_handling("wrong request type");

    strcpy(filename, requested_file);

    if(msg_type == RD)
        puts("< requested download >");
    else if(msg_type == RU)
        puts("< requested upload >");

    displayHeader(*(akh_pdu_header *)pac);
    printf("filename: %s\n", filename);
    printf("filesize: %d\n", *filesize);

    return msg_type;
}


// when client makes connection with server for download
// when accepted, return filesize. Otherwise return 0
off_t connection_download_client(int sock, struct sockaddr_in *serv_adr, char *filename)
{
    // create RD package
    akh_pdu_header header = createHeader(RD, randNum());
    akh_pdu_body body = filename;

    packet pac;
    size_t pac_len = createPacket(&pac, &header, body, strlen(body));

    printPacket(pac, pac_len);

    // send RD package to server
    sendto(sock, pac, pac_len, 0, (struct sockaddr *)serv_adr, sizeof(*serv_adr));
    deletePacket(pac);

    socklen_t adr_sz;
    struct sockaddr_in from_adr;
    ssize_t response_len;
    char response[MAX_BUFFER_SIZE];

    // recieve package from server
    adr_sz = sizeof(struct sockaddr_in);
    response_len = timer_recvfrom(sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&from_adr, &adr_sz, TIMEOUT, NUM_TRY);

    printPacket(response, response_len);

    /* // send RD package to server and waiting for server's response */
    /* akh_send(sock, pac, pac_len, 0, 0, (struct sockaddr *)serv_adr, (struct sockaddr *)&from_adr, &adr_sz, response, MAX_RESPONSE_SIZE); */

    if(response_len == -1)
    	error_handling("fail connection error");
    else if(((akh_pdu_header *)response)->msg_type == EA)
        error_handling("server does not have file");
    else if(((akh_pdu_header *)response)->msg_type != AD)
    	error_handling("request rejected by server");

    off_t filesize = *(off_t *)( (akh_pdu_header *)response + 1 );

    puts("< download request >");

    puts("- send pac -");
    displayHeader(header);
    printf("filename: %s\n", body);

    puts("- receive pac -");
    displayHeader(*(akh_pdu_header *)response);
    printf("file size %d\n", filesize);

    return filesize;
}


// when client requests download, server uses the function to make connection
void connection_download_server(int serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize)
{
    akh_pdu_header header;
    *filesize = get_file_size(filename);
    packet response;
    size_t response_len = createPacket(&response, &header, (akh_pdu_body)filesize, sizeof(off_t));

    if(*filesize != 0){
        header = createHeader(AD, randNum());
        response_len = createPacket(&response, &header, (akh_pdu_body)filesize, sizeof(off_t));
    }
    else {
        header = createHeader(EA, randNum());
	akh_pdu_body security_level = (akh_pdu_body)malloc(sizeof(char) * 2);
	security_level = (akh_pdu_body) SEC_LEVEL_LOW;
	akh_pdu_body error_description = (akh_pdu_body) FILE_NOT_FOUND;
	akh_pdu_body body = (akh_pdu_body) malloc(sizeof(char) * 4);
	memcpy(body, security_level, strlen(security_level));
	memcpy(body + sizeof(security_level), error_description, strlen(error_description));
	response_len = createPacket(&response, &header, (akh_pdu_body)filesize, sizeof(off_t));
    }	

    printPacket(response, response_len);

    sendto(serv_sock, response, response_len, 0, (struct sockaddr *)clnt_adr, *clnt_adr_sz);
    deletePacket(response);

    if(header.msg_type == EA)
        error_handling("server does not have file");

    puts("< accept download >");
    displayHeader(header);
    printf("filesize: %d\n", *filesize);
}


// when client makes connection with server for upload
// when accepted, return 0. Otherwise return -1
int connection_upload_client(int sock, struct sockaddr_in *serv_adr, char *filename, off_t *filesize)
{
    *filesize = get_file_size(filename);
    size_t body_len = sizeof(off_t) + strlen(filename);

    // create RU package
    akh_pdu_header header = createHeader(RU, randNum());
    akh_pdu_body body = (akh_pdu_body)malloc(body_len);
    memcpy(body, filesize, sizeof(off_t));
    strcpy(body + sizeof(off_t), filename);

    packet pac;
    size_t pac_len = createPacket(&pac, &header, body, body_len);

    // send RU package to server
    sendto(sock, pac, pac_len, 0, (struct sockaddr *)serv_adr, sizeof(*serv_adr));
    free(body);
    deletePacket(pac);

    socklen_t adr_sz;
    struct sockaddr_in from_adr;
    ssize_t response_len;
    char response[MAX_BUFFER_SIZE];

    // recieve package from server
    adr_sz = sizeof(struct sockaddr_in);
    response_len = timer_recvfrom(sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&from_adr, &adr_sz, TIMEOUT, NUM_TRY);

    if(response_len == -1)
    	error_handling("fail connection error");
    else if(((akh_pdu_header *)response)->msg_type != AU)
    	error_handling("request rejected by server");

    puts("< upload request >");
    puts("- send pac -");
    displayHeader(header);
    printf("filesize: %d\n", *filesize);
    printf("filename: %s\n", filename);
    puts("- receive pac -");
    displayHeader(*(akh_pdu_header *)response);

    return 0;
}

// when client requests upload, server uses the function to make connection
void connection_upload_server(int serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, off_t *filesize)
{
    akh_pdu_header header = createHeader(AU, randNum());
    if(*filesize > 1024) // greater than 1KB reject
        header = createHeader(DR, randNum());
    else
        header = createHeader(AU, randNum());

    packet response;
    size_t response_len = createPacket(&response, &header, NULL, 0);

    sendto(serv_sock, response, response_len, 0, (struct sockaddr *)clnt_adr, *clnt_adr_sz);
    deletePacket(response);

    if(*filesize > 1024) // greater than 1KB reject
        error_handling("request filesize is big");

    puts("< accept upload >");
    displayHeader(header);
}

