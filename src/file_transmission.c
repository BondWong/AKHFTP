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


size_t write_segment(void *buf, size_t size, char *filename)
{
    return write_file_segment(buf, size, filename);
}

size_t read_segment(void *buf, size_t size, int seg_num, char *filename)
{
    return read_file_segment(buf, size, seg_num, filename);
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

// reciever uses the function
int recieve_file(int sock, struct sockaddr_in *send_adr, socklen_t *send_adr_sz, char *filename)
{
    char response[MAX_BUFFER_SIZE];
    ssize_t response_len;
    do {
        response_len = timer_recvfrom(sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)send_adr, send_adr_sz, TIMEOUT, NUM_TRY);

        // time-out
        if(response_len == -1)
            return -1;
        else if(((akh_pdu_header *)response)->msg_type == SS)
            write_segment(response, response_len, filename);

        akh_pdu_header *pheader = (akh_pdu_header *)response;
        puts("< receive file segment >");
        printf("msg_type = %x\tseq_num = %d\n", pheader->msg_type, pheader->seq_num);
    } while(((akh_pdu_header *)response)->msg_type == SS);

    return ((akh_pdu_header *)response)->msg_type;

    printf("recieve file\n");
    return 0;
}

// sender uses the function
int send_file(int sock, struct sockaddr_in *recv_adr, char *filename, akh_disconn_response disconn_response)
{
    uint32_t seg_size = disconn_response.segment_size;
    uint32_t req_num = disconn_response.segment_num;
    uint32_t *seg_list = disconn_response.segment_list;
    uint32_t seg_num;

    akh_pdu_header header;
    char buf[MAX_BUFFER_SIZE];
    packet pac;
    size_t buf_len, pac_len;

    int i;
    for(i = 0; i < seg_num; i++) {
        seg_num = seg_list[i];
        header = createHeader(SS, seg_num);
        buf_len = read_segment(buf, seg_size, seg_num, filename);
        pac_len = createPacket(&pac, &header, buf, buf_len);
        sendto(sock, pac, pac_len, 0, (struct sockaddr *)recv_adr, sizeof(*recv_adr));
        deletePacket(pac);
    }
    return 0;
}

