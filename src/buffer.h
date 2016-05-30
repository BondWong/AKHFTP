#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "message.h"

// TO Do change binary tree to heap

struct node
{
    packet pac;
    struct node *left;
    struct node *right;
};

struct buffer
{
    struct node *root;
    unsigned long availability;
    int size;
    packet* pac_array;
};

typedef struct node * buf_tree_node;
typedef struct buffer * akh_buffer;

#endif
