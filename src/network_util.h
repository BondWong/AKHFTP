#ifndef __network_util_h__
#define __network_util_h__

#include <sys/times.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "message.h"

void akh_send(int sock, packet pac, size_t pac_len, int send_options, int rece_options, 
	struct sockaddr* serv_adr, struct sockaddr* from_adr, socklen_t* adr_sz, char* response, int response_sz);

#endif