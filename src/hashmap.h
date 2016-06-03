#ifndef __HASHMAP_H__
#define __HASHMAP_H__
#include <stdint.h>

struct Hashmap_node
{
    uint64_t key;
    uint32_t value;
    struct Hashmap_node* next;
};

typedef struct Hashmap_node hashmap_node;

typedef struct
{
    hashmap_node** elements;
    unsigned int capacity;
} hashmap;

uint32_t calc_hashcode(hashmap* m, uint64_t key);
hashmap* create_hashmap(unsigned int capacity);
void delete_hashmap(hashmap* m);
void hashmap_put(hashmap* m, uint64_t key, uint32_t value);
int hashmap_get(hashmap* m, uint64_t key, uint32_t* value); // <0 for nothing get

#endif
