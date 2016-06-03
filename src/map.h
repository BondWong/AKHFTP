#ifndef __MAP_H__
#define __MAP_H__

struct node
{
    uint32_t seqnum;
    struct node* next;
};

typedef struct node node;
typedef struct node* map_elem;

struct map
{
    map_elem* elements;
    unsigned int capacity;
};

typedef struct map map;

#endif
