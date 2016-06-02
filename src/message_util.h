/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: The header file for message_util.c
 * * */
#ifndef __MESSAGE_UTIL_H__
#define __MESSAGE_UTIL_H__

#include <stdint.h>
#include <string.h>

#include "message.h"

akh_pdu_header createHeader(uint16_t msg_type, uint32_t seq_num);
void displayHeader(akh_pdu_header header);
size_t createPacket(packet *pac, akh_pdu_header *header, akh_pdu_body body, size_t body_len);
void deletePacket(packet pac);
unsigned long get_pac_max();

#endif

