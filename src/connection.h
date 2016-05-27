#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <arpa/inet.h>
#include <unistd.h>

// when client makes connection with server for download
off_t connection_download_client(int *sock, struct sockaddr_in *serv_adr, char *filename);

// when client requests download, server uses the function to make connection
int connection_download_server();

// when client makes connection with server for upload
int connection_upload_client();

// when client requests upload, server uses the function to make connection
int connection_upload_server();

#endif

