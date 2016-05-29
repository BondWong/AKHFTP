#ifndef __DISCONNECTION_H__
#define __DISCONNECTION_H__

#include <stdio.h>
#include "message.h"
// reciever uses to disconnect
int disconnection_reciever();

// sender uses to disconnect
akh_disconn_response disconnection_sender(int *sock, struct sockaddr_in *dst_addr);

#endif

