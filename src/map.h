/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: header file for map
 * * */
#ifndef __MAP_H__
#define __MAP_H__


/**
* a node is a map element, it contains seqence number and a pointer to next node.
* The pointer allows us to use linked-list manner to solve element collision problem
**/
struct node
{
    uint32_t seqnum;
    struct node* next;
};

typedef struct node node;
typedef struct node* map_elem;

/**
* a map structure that contains element array and the array size
**/
struct map
{
    map_elem* elements;
    unsigned int capacity;
};

typedef struct map map;

#endif
