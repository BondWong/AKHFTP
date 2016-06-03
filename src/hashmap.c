/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: hashmap data structure. 
 *	    Key:value pair is uint64_t:uint32_t. 
 *	    Support insert and inquiry operation. 
 *	    Use link list structure.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

uint32_t calc_hashcode(hashmap* m, uint64_t key) {
    unsigned int capacity = m->capacity;

	char* nums = (char*)calloc(64,sizeof(char));
	sprintf(nums, "%ld", key);

	uint32_t hash = 0;
	int a = 378551, b = 63689, i;
	for (i = 0; i < 64; i++) {
		hash = hash * a + nums[i];
		a = a * b;
	}
	free(nums);
	return hash % capacity;
}

hashmap* create_hashmap(unsigned int capacity) {
    hashmap* m = (hashmap*)malloc(sizeof(hashmap));
    m->elements = (hashmap_node**)calloc(capacity, sizeof(hashmap_node*));
    m->capacity = capacity; 
    return m;
}

void delete_hashmap(hashmap* m) { // TODO: free link lists inside hashmap.
    return;
}

void hashmap_put(hashmap* m, uint64_t key, uint32_t value) {
    uint32_t hashcode = calc_hashcode(m, key);
    hashmap_node* runner = m->elements[hashcode];

    while(runner != NULL && runner->key != key && runner->next != NULL)
        runner = runner->next;
    if(runner != NULL && runner->key == key)
        runner->value = value;
    else {
        hashmap_node* newnode = (hashmap_node*)malloc(sizeof(hashmap_node));
        newnode->key = key;
        newnode->value = value;
        newnode->next = NULL;
        if(runner == NULL) 
            m->elements[hashcode] = newnode;
        else
            runner->next = newnode;
    }
}

int hashmap_get(hashmap* m, uint64_t key, uint32_t* value){
    uint32_t hashcode = calc_hashcode(m, key);
    hashmap_node* runner = m->elements[hashcode];
    
    while(runner != NULL && runner->key != key)
        runner = runner->next;
    if(runner == NULL)
        return -1;
    *value = runner->value;
    return 0;
}
