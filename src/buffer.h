#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "message.h"

struct heap 
{
    packet* pac_array;
    unsigned int capacity;
    unsigned int count;
};

typedef struct heap buffer;

#endif
