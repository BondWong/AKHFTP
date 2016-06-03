#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <arpa/inet.h>
#include <unistd.h>

// request handle
uint16_t handle_request(int serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize);
uint16_t handle_request_pipe(struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize);

// when client makes connection with server for download
// when accepted, return filesize. Otherwise return 0
off_t connection_download_client(int sock, struct sockaddr_in *serv_adr, char *filename);

// when client requests download, server uses the function to make connection
void connection_download_server(int serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz, char *filename, off_t *filesize);

// when client makes connection with server for upload
// when accepted, return 0. Otherwise return -1
int connection_upload_client(int sock, struct sockaddr_in *serv_adr, char *filename, off_t *filesize);

// when client requests upload, server uses the function to make connection
void connection_upload_server(int serv_sock, struct sockaddr_in *clnt_adr, socklen_t *clnt_adr_sz);

#endif

