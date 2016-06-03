/* Class Name: CS544 Computer Networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose
 *    The header file of network_util.c
 *    it has two method
 *    timer_recvfrom => it is recvfrom with timer and maxinum number of trial. It prevents deadlock for listner
 *    akh_send => it is used for handshake
 */

#ifndef __network_util_h__
#define __network_util_h__

#include <sys/times.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "message.h"

#define NUM_TRY 3
#define TIMEOUT 3

// it returns -1 when it reachs time-out otherwise return what recvfrom returns
ssize_t timer_recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen, long sec, int num_try);

// it is used for handshake
void akh_send(int* sock, packet pac, size_t pac_len, int send_options, int rece_options, 
	struct sockaddr* serv_adr, struct sockaddr* from_adr, socklen_t* adr_sz, char* response, int response_sz);

#endif
