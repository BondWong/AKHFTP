#ifndef __DISCONNECTION_H__
#define __DISCONNECTION_H__

#include <stdio.h>

#include "message.h"

// check file status
// if finish downloading, accept close
// otherwise, request segment
int handle_request_close(int *sock, struct sockaddr_in *send_adr, char *filename, off_t filesize, uint32_t seg_size, int test);

// sender requests close after sending all segment
void request_close(int sock, struct sockaddr_in *recv_adr);

// time-out, return -1
// if receiver accepted close request, return 0
// if receipient is not ready to close, i.e. there is missing segments, return 1
// otherwise, return 2;
int wait_disconnection(int sock, struct sockaddr_in *recv_adr, socklen_t *recv_adr_sz, akh_disconn_response *disconn_response);

#endif

