/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: header file for buffer
 * * */
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "message.h"
#include "map.h"

struct heap_node
{
    packet pac;
    ssize_t pac_size;
};

typedef struct heap_node buffer_element;

struct heap 
{
    struct heap_node* elements;
    map* m;
    unsigned int capacity;
    unsigned int count;
};

typedef struct heap buffer;

#endif
