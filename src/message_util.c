/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: This file contains functions related to PDU
 *	    1. createHeader: create common AKH header
 *	    2. displayHeader: print out information in header
 *	    3. createPacket: create a AKH packet
 *	    4. deletePacket: free the memory previously allocated for a packet
 *	    5. get_pac_max: stub function, return 2kb
 * * * */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "message_util.h"

akh_pdu_header createHeader(uint16_t msg_type, uint32_t seq_num)
{
    akh_pdu_header header = {0, msg_type, seq_num, 0}; // { .ver = 0, .msg_type = msg_type, .seq_num = seq_num, .body_len = 0 };
    return header;
}

void displayHeader(akh_pdu_header header)
{
    printf("ver => %d\n", header.ver);
    printf("msg_type => %d\n", header.msg_type);
    printf("seq_num => %u\n", header.seq_num);
    printf("body_len => %d\n", header.body_len);
}

size_t createPacket(packet *pac, akh_pdu_header *header, akh_pdu_body body, size_t body_len)
{
    header->body_len = body_len;
    *pac = (packet)malloc(sizeof(akh_pdu_header) + body_len);
    if(*pac == NULL)
        return 0;

    memcpy(*pac, header, sizeof(akh_pdu_header));
    if(body != NULL)
        memcpy(*pac + sizeof(akh_pdu_header), body, body_len);
    return sizeof(akh_pdu_header) + body_len;
}

void deletePacket(packet pac)
{
    free(pac);
}

// stub function, return 2kb
unsigned long get_pac_max() {
    return 2 * 1024;
}

void printPacket(packet pac, size_t pac_len) {
    printf("=======PACKET CONTENT(size %u)=====\n", (unsigned int)pac_len);
    if(pac_len < 12 || pac_len > MAX_BUFFER_SIZE) {
        printf("printPacket: pac_len %u < header size 12\n", (unsigned int)pac_len);
        return;
    }

    for(size_t i = 0; i < pac_len; ++i) {
        if (i != 0 && (i%4)==0) printf("\n");
        printf("%02x ", (unsigned char)pac[i]);
    }
    printf("\n====== END ==========\n");
}
