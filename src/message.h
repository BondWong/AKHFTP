#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdint.h>

#define RD 0x0001 // Request Download
#define RU 0x0002 // Request Upload
#define AD 0x0003 // Accept Download
#define RS 0x0004 // Request Segments
#define SS 0x0005 // Send Segment
#define RB 0x0006 // Request Blocking
#define RC 0x0007 // Request Close
#define AC 0x0008 // Accept Close
#define DR 0x0009 // Deny Request
#define EA 0x000A // Error Alert

#define MAX_BUFFER_SIZE (1024 * 128) // maximum buffer size 128 kb

typedef struct tag_akh_pdu_header
{
    uint16_t ver;      // version
    uint16_t msg_type; // message type
    uint32_t seq_num;  // sequence number
    uint32_t body_len; // body length
} akh_pdu_header;

typedef char * akh_pdu_body;

typedef char * packet;

#endif

