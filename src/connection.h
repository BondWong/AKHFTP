#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <arpa/inet.h>
#include <unistd.h>

// request handle
uint16_t handle_request(int serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize);

// when client makes connection with server for download
off_t connection_download_client(int sock, struct sockaddr_in *serv_adr, char *filename);

// when client requests download, server uses the function to make connection
void connection_download_server(int *serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize);

// when client makes connection with server for upload
int connection_upload_client();

// when client requests upload, server uses the function to make connection
int connection_upload_server();

#endif

