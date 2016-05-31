#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "file_util.h"
#include "disconnection.h"
#include "security_util.h"
#include "network_util.h"


// check file status
// if finish downloading, accept close and return 0
// otherwise, request segment and return -1
int handle_request_close(int *sock, struct sockaddr_in *send_adr, char *filename, off_t filesize, uint32_t seg_size)
{
    off_t current_filesize = get_file_size(filename);

    // finish download, accept close
    if(filesize <= current_filesize) {
        akh_pdu_header header = createHeader(AC, randNum());
        packet pac;
        size_t pac_len = createPacket(&pac, &header, NULL, 0);

        //send AC packet to sender
        sendto(sock, pac, pac_len, 0, (struct sockaddr *)send_adr, sizeof(*send_adr));
        deletePacket(pac);

        puts("< send accept close >");
        displayHeader(header);

        return 0;
    }
    // not finish download, request segment
    else {
        uint32_t curr_segment_num = (current_filesize % seg_size != 0) ? 1 + current_filesize / seg_size : current_filesize / seg_size;
        uint32_t last_segment_num = (filesize % seg_size != 0) ? 1 + filesize / seg_size : filesize / seg_size;
        uint32_t num_missing_segment = last_segment_num - curr_segment_num; // the number of missing segments
        
        akh_pdu_header header = createHeader(RS, randNum());
        packet pac;
        uint32_t body[2 + num_missing_segment];
        body[0] = seg_size; // segment size
        body[1] = num_missing_segment; // number of missing segments

        int i;
        for(i = curr_segment_num; i < last_segment_num; i++) {
            body[i - curr_segment_num + 2] = i;
        }

        size_t body_len = sizeof(body);
        size_t pac_len = createPacket(&pac, &header, body, body_len);

        //send AC packet to server
        sendto(sock, pac, pac_len, 0, (struct sockaddr *)send_adr, sizeof(*send_adr));
        deletePacket(pac);

        puts("< send request segments>");
        displayHeader(header);
        printf("segment size => %d\n", body[0]);
        printf("num missing segment => %d\n", body[1]);
        for(i = 0; i < num_missing_segment; i++)
            printf("segment[%d] => %d\n", i, body[2+i]);

        sleep(2);
        return -1;
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

