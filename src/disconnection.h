/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: The header file of disconnection.c
 *	    The two parties in the file transfer process are:
 *	    file sender ----file----> file receiver
 *	    This file declares functions for disconnecting the two paries.
 *	    1. handle_request_close: function for the file receiver to handle request close (RC) message from the file sender
 *	    2. request_close:  function that generates a RC message
 *	    3. wait_disconnection: function for the file sender to handle the responses (AC or RS) to the RC message it sent 
 * * */
#ifndef __DISCONNECTION_H__
#define __DISCONNECTION_H__

#include <stdio.h>

#include "message.h"

// check file status
// if finish downloading, accept close
// otherwise, request segment
int handle_request_close(int *sock, struct sockaddr_in *send_adr, char *filename, off_t filesize, uint32_t seg_size);

// sender requests close after sending all segment
void request_close(int sock, struct sockaddr_in *recv_adr);

// time-out, return -1
// if receiver accepted close request, return 0
// if receipient is not ready to close, i.e. there is missing segments, return 1
// otherwise, return 2;
int wait_disconnection(int sock, struct sockaddr_in *recv_adr, socklen_t *recv_adr_sz, akh_disconn_response *disconn_response);

#endif

