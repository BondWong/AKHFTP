#ifndef __MAP_H__
#define __MAP_H__

struct seqnum_node
{
    uint32_t seqnum;
    seqnum_node* next;
};

struct map
{
    seqnum_node* seqnum_node_array;
    unsigned int capacity;
};

#endif