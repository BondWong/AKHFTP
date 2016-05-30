#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "disconnection.h"
#include "security_util.h"
#include "network_util.h"


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


// check file status
// if finish downloading, accept close and return 0
// otherwise, request segment and return -1
int handle_request_close(int *sock, struct sockaddr_in *send_adr, char *filename, off_t filesize, int test)
{
    if(test == 0) {
        akh_pdu_header RS_header = createHeader(RS,randNum());
        packet RS_pac;
        uint32_t num_missing_segment = 2; // the number of missing segments
        uint32_t RS_body[(2 + num_missing_segment)];
        RS_body[0] = 10; // segment size
        RS_body[1] = num_missing_segment; // number of missing segments
        RS_body[2] = 111; // seq_num of the 1st missing segment
        RS_body[3] = 222; // seq_num of the 2nd missing segment

        size_t RS_body_len = sizeof(uint32_t)*(2 + num_missing_segment);
        size_t RS_pac_len = createPacket(&RS_pac, &RS_header, RS_body, RS_body_len);
        //send AC packet to server
        sendto(sock, RS_pac, RS_pac_len, 0, (struct sockaddr *)send_adr, sizeof(*send_adr));
        deletePacket(RS_pac);

        puts("< send request segments>");
        displayHeader(RS_header);
        printf("segment size => %d\n", RS_body[0]);
        printf("num missing segment => %d\n", RS_body[1]);
        int i;
        for(i = 0; i < num_missing_segment; i++)
            printf("segment[%d] => %d\n", i, RS_body[2+i]);
     
        return -1;
    }
    else if(test == 1) {
        akh_pdu_header header = createHeader(AC,randNum());
        packet pac;
        size_t pac_len = createPacket(&pac, &header, NULL, 0);
        //send AC packet to server
        sendto(sock, pac, pac_len, 0, (struct sockaddr *)send_adr, sizeof(*send_adr));
        deletePacket(pac);

        puts("< send accept close >");
        displayHeader(header);

        return 0;
    }
}


// sender requests close after sending all segment
void request_close(int sock, struct sockaddr_in *recv_adr)
{
    // create RC package
    akh_pdu_header header = createHeader(RC, randNum());

    packet pac;
    size_t pac_len = createPacket(&pac, &header, NULL, 0);

    // send RD package to server
    sendto(sock, pac, pac_len, 0, (struct sockaddr *)recv_adr, sizeof(*recv_adr));
    deletePacket(pac);

    puts("< request close >");
    displayHeader(header);
}


// time-out, return -1
// if receiver accepted close request, return 0
// if receipient is not ready to close, i.e. there is missing segments, return 1
// otherwise, return 2;
int wait_disconnection(int sock, struct sockaddr_in *recv_adr, socklen_t *recv_adr_sz, akh_disconn_response *disconn_response)
{
    if(disconn_response->segment_list != NULL)
        free(disconn_response->segment_list);

    char response[MAX_BUFFER_SIZE];
    ssize_t response_len;

    // recieve package from receiver
    *recv_adr_sz = sizeof(*recv_adr);
    response_len = timer_recvfrom(sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)recv_adr, recv_adr_sz, TIMEOUT, NUM_TRY);
    if(response_len == -1) {
    	// error_handling("fail connection error");
        puts("time-out");
        return -1;
    }

    disconn_response->response_type = ((akh_pdu_header *)response)->msg_type;

    // if receiver accepted close request, return 0
    if(disconn_response->response_type == AC) {
        puts("< receive accept close >");
        return 0;
    }
    // if receipient is not ready to close, i.e. there is missing segments, return 1
    else if(disconn_response->response_type == RS) {
        int i;
        uint32_t *p_body = (uint32_t *)(response + sizeof(akh_pdu_header));
        disconn_response->segment_size = p_body[0];
        disconn_response->segment_num = p_body[1];
        // do not free memory here; after we finish using the segment_list, the memory can be freed.
        disconn_response->segment_list = (uint32_t *)malloc(disconn_response->segment_num * sizeof(uint32_t));
        for(i = 0; i < disconn_response->segment_num; i++)
            disconn_response->segment_list[i] = p_body[2 + i];

        // memcpy(disconn_response->segment_list, response + sizeof(akh_pdu_header) + 2 * sizeof(uint32_t), disconn_response->segment_num * sizeof(uint32_t));

        puts("< receive request segment >");
        printf("segment_num => %d\n", disconn_response->segment_num);
        for(i = 0; i < disconn_response->segment_num; i++)
            printf("segment[%d] => %d\n", i, disconn_response->segment_list[i]);
        return 1;
    }
    // otherwise, return 2;
    else {
        printf("< receive other message type %x >", disconn_response->response_type);
        return 2;
    }
}

