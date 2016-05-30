#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "file_util.h"
#include "security_util.h"
#include "network_util.h"
#include "file_transmission.h"


int test_receive_file(int sock, struct sockaddr_in *send_adr, socklen_t *send_adr_sz)
{
    char response[MAX_BUFFER_SIZE];
    ssize_t response_len;
    do {
        response_len = timer_recvfrom(sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)send_adr, send_adr_sz, TIMEOUT, NUM_TRY);

        // time-out
        if(response_len == -1)
            return -1;

        puts("< test receive file >");
        displayHeader(*(akh_pdu_header *)response);
    } while(((akh_pdu_header *)response)->msg_type == SS);

    return ((akh_pdu_header *)response)->msg_type;
}

// reciever uses the function
int recieve_file()
{
    printf("recieve file\n");
    return 0;
}

// sender uses the function
int send_file()
{
    printf("send file\n");
    return 0;
}

