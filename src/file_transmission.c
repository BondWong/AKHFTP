/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: define functions for 1) sending and receiving file segments
 *				 2) read and write file segments
 * */
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
#include "buffer.h"
#include "buffer_util.h"
#include "map.h"
#include "map_util.h"


size_t write_segment(void *buf, size_t size, char *filename)
{
    return write_file_segment(buf, size, filename);
}

size_t read_segment(void *buf, size_t size, int seg_num, char *filename)
{
    return read_file_segment(buf, size, seg_num, filename);
}


// reciever uses this function to receive file
int receive_file(int sock, struct sockaddr_in *send_adr, socklen_t *send_adr_sz, char *filename, uint32_t seg_size)
{
    off_t current_filesize = get_file_size(filename);
    uint32_t curr_segment_num = (current_filesize % seg_size != 0) ? 1 + current_filesize / seg_size : current_filesize / seg_size;

    char response[MAX_BUFFER_SIZE];
    ssize_t response_len;
    buffer *b;
	create_buffer(&b, 1024);

    do {
        response_len = timer_recvfrom(sock, response, MAX_BUFFER_SIZE, 0, (struct sockaddr *)send_adr, send_adr_sz, TIMEOUT, NUM_TRY);
        akh_pdu_header *pheader = (akh_pdu_header *)response;

        // time-out
        if(response_len == -1)
            return -1;
        else if(pheader->msg_type == SS) {
            printf("SS seq_num => %d\n", pheader->seq_num);
            if(pheader->seq_num == curr_segment_num) {
                write_segment(response + sizeof(akh_pdu_header), response_len - sizeof(akh_pdu_header), filename);
                curr_segment_num++;
            }
            else {
		        push(b, response, response_len);
            }
            uint32_t temp;
            do {
                temp = curr_segment_num;
                pop(b, response, &response_len);
                pheader = (akh_pdu_header *)response;
                if(pheader->seq_num == curr_segment_num) {
                    write_segment(response + sizeof(akh_pdu_header), response_len - sizeof(akh_pdu_header), filename);
                    curr_segment_num++;
                }
                push(b, response, response_len);
            } while(pheader->seq_num == temp);

        }

        puts("< receive file segment >");
        printf("msg_type = %x\tseq_num = %d\n", pheader->msg_type, pheader->seq_num);
    } while(((akh_pdu_header *)response)->msg_type == SS);


    return ((akh_pdu_header *)response)->msg_type;
}


// sender uses this function to send files
int send_file(int sock, struct sockaddr_in *recv_adr, char *filename, akh_disconn_response *disconn_response)
{
    uint32_t seg_size = disconn_response->segment_size;
    uint32_t req_num = disconn_response->segment_num;
    uint32_t *seg_list = disconn_response->segment_list;
    uint32_t seg_num;

    akh_pdu_header header;
    char buf[MAX_BUFFER_SIZE];
    packet pac;
    size_t buf_len, pac_len;

    int i;
    for(i = 0; i < req_num; i++) {
        seg_num = seg_list[i];
        header = createHeader(SS, seg_num);
        buf_len = read_segment(buf, seg_size, seg_num, filename);
        pac_len = createPacket(&pac, &header, buf, buf_len);
        sendto(sock, pac, pac_len, 0, (struct sockaddr *)recv_adr, sizeof(*recv_adr));
        deletePacket(pac);

        puts("< send file segment >");
        displayHeader(header);
    }
    return 0;
}

