/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: Defines macros of message types, etc. Also defines structure for "PDU header" and "disconnection response"; PDU body, and packet pointer.
 * * */
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdint.h>

#define RD 0x0001 // Request Download
#define RU 0x0002 // Request Upload
#define AD 0x0003 // Accept Download
#define AU 0x0004 // Accept Upload
#define RS 0x0005 // Request Segments
#define SS 0x0006 // Send Segment
//#define RB 0x0007 // Request Blocking
#define RC 0x0007 // Request Close
#define AC 0x0008 // Accept Close
#define DR 0x0009 // Deny Request
#define EA 0x000A // Error Alert

#define MAX_FILENAME_SIZE 128
#define MAX_BUFFER_SIZE (1024 * 128) // maximum buffer size 128 kb
#define MAX_RESPONSE_SIZE (1024 * 2) // maximum response size kb

typedef struct tag_akh_pdu_header
{
    uint16_t ver;      // version
    uint16_t msg_type; // message type
    uint32_t seq_num;  // sequence number
    uint32_t body_len; // body length
} akh_pdu_header;

typedef char * akh_pdu_body;

typedef char * packet;

typedef struct akh_disconnection_response
{
    uint16_t response_type; // the message type of the response to RS
    uint32_t segment_size; // the size of one segment during transmission
    uint32_t segment_num; // number of missing segment
    uint32_t *segment_list; // the list of the sequence numbers of missing segements
} akh_disconn_response;

#endif

