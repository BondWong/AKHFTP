#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    memcpy(*pac + sizeof(akh_pdu_header), body, body_len);
    return sizeof(akh_pdu_header) + body_len;
}

void deletePacket(packet pac)
{
    free(pac);
}

