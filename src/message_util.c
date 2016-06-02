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

// create common AKH header
akh_pdu_header createHeader(uint16_t msg_type, uint32_t seq_num)
{
    akh_pdu_header header = {0, msg_type, seq_num, 0}; // { .ver = 0, .msg_type = msg_type, .seq_num = seq_num, .body_len = 0 };
    return header;
}

//  print out information in header
void displayHeader(akh_pdu_header header)
{
    printf("ver => %d\n", header.ver);
    printf("msg_type => %d\n", header.msg_type);
    printf("seq_num => %u\n", header.seq_num);
    printf("body_len => %d\n", header.body_len);
}
//create a AKH packet
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
// free the memory previously allocated for a packet
void deletePacket(packet pac)
{
    free(pac);
}

// stub function, return 2kb
unsigned long get_pac_max() {
    return 2 * 1024;
}
