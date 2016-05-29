#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "disconnection.h"
#include "security_util.h"
#include "message.h"
#include "message_util.h"

// reciever uses to disconnect
int disconnection_reciever()
{
    printf("disconnection from reciever\n");
    return 0;
}

// sender uses to disconnect
akh_disconn_response disconnection_sender(int *sock, struct sockaddr_in *dst_addr)
{
    //create RC packet
    akh_pdu_header header = createHeader(RC, randNum());
    packet pac;
    size_t pac_len = createPacket(&pac, &header, NULL, 0);
    //send RC packet to server
    sendto(*sock, pac, pac_len, 0, (struct sockaddr *) dst_addr, sizeof(*dst_addr));
    deletePacket(pac);
    printf("disconnection from sender\n");
    
    // create a buff for response
    char response[MAX_BUFFER_SIZE];
    //receive packet from destination
    int str_len;
    socklen_t dst_addr_sz = sizeof(struct sockaddr_in);
    str_len = recvfrom(*sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&dst_addr, &dst_addr_sz);
    uint16_t msg_type = ((akh_pdu_header *)response)->msg_type;
    uint32_t seq_num = ((akh_pdu_header *)response)->seq_num; // get the sequence number of the missing segment
    // if receipient is not ready to close, i.e. there is missing segments
    if (msg_type == RS) {
        printf("Closure rejected. There are some missing segments!\n");
        akh_disconn_response disconn_response;
        disconn_response.response_type = msg_type;
        disconn_response.segment_size = *((uint32_t *)(response + sizeof(akh_pdu_header)));
        disconn_response.segment_num = *((uint32_t *)(response + sizeof(akh_pdu_header)+ sizeof(uint32_t)));
        disconn_response.segment_list = (char *)malloc(disconn_response.segment_num * sizeof(uint32_t));
        memcpy(disconn_response.segment_list, (char *)(response + sizeof(akh_pdu_header) + sizeof(uint32_t) + sizeof(uint32_t)), disconn_response.segment_num * sizeof(uint32_t));
        return disconn_response;
        // do not free memory here; after we finish using the segment_list, the memory can be freed.
    }
    else if (msg_type == AC ){
        printf("Accept closure of connection!\n");
        akh_disconn_response disconn_response;
        disconn_response.response_type = msg_type;
        return disconn_response;
    }
    else // if receive types other than AC or RS, then something is wrong
    {
        printf("Closure rejected with unknow reason\n");
        akh_disconn_response disconn_response;
        disconn_response.response_type = msg_type;
        return disconn_response;
    }
}

