#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "message.h"

struct heap_node
{
    packet pac;
    ssize_t pac_size;
};

typedef struct heap_node buffer_element;

struct heap 
{
    struct heap_node* elements;
    unsigned int capacity;
    unsigned int count;
};

typedef struct heap buffer;

#endif
