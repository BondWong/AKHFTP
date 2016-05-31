#ifndef __BUFFER_UTIL_H__
#define __BUFFER_UTIL_H__

#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "buffer.h"

uint32_t get_seqnum(packet pac);
unsigned int get_parent(unsigned int index);
unsigned int get_left(unsigned int index);
unsigned int get_right(unsigned int index);
void swap(packet* pac_array, unsigned int i, unsigned int j);
void heapify(packet* pac_array, unsigned int index, unsigned int count);
void heapify_up(packet* pac_array, unsigned int index, unsigned int count);
void create_buffer(buffer** h, unsigned int capacity);
void free_buffer(buffer** h);
int push(buffer* h, packet pac);
int pop(buffer* h, packet* pac);

#endif

