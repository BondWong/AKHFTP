/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: the header file of buffer_util.c
 *	    1. hash_code: get hash code for a given key and map capacity
 *	    2. create_map: create a map, allocating memory and initializing fields
 *	    3. free_map: deallocating memory of a map, setting fields to default value
 *	    4. put: put a node into the map for a giving key
 *	    5. get: get a node from map according to a giving key
 *      6. contains: to see whether the map contains an element
 *      7. remove_elem: remove a node in map according to a giving key
 * * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map_util.h"

uint32_t hash_code(uint32_t key, unsigned int capacity) {
	char* nums = (char*)calloc(32,sizeof(char));
	sprintf(nums, "%d", key);

	uint32_t hash = 0;
	int a = 378551, b = 63689, i;
	for (i = 0; i < 32; i++) {
		hash = hash * a + nums[i];
		a = a * b;
	}
	free(nums);
	return hash % capacity;
}

map* create_map(unsigned int capacity) {
	map* m = (map*)malloc(sizeof(map));
	m->elements = (map_elem*)malloc(sizeof(map_elem) * capacity);
	unsigned int i;
	for(i = 0; i < capacity; i++) {
		m->elements[i] = NULL;
	}
	m->capacity = capacity;

	return m;
}

void free_map(map** m) {
	free((*m)->elements);
	(*m)->elements = NULL;
	(*m)->capacity = 0;
	free(*m);
	*m = NULL;
}

/**
* put an element into map with a given key
*
* @param map, key, element
*
* @return int
* @Description: return 1 if successful
*
**/
int put(map* m, uint32_t key, map_elem element) {
	uint32_t hash = hash_code(key, m->capacity);
	printf("putting key %d, seqnum %d, hash %d\n", key, element->seqnum, hash);
	
	if(m->elements[hash] == NULL) {
	    printf("type1\n");
		m->elements[hash] = element;
		return 1;
	}
	map_elem runner = m->elements[hash];
	while(runner->next != NULL) {
		runner = runner->next;
	}
	runner->next = element;
	
	return 1;
}

/**
* check whether a map contains an element
*
* @param map, element
*
* @return int
* @Description: return 1 if contains, return 0 if not
*
**/
int contains(map* m, map_elem element) {
	uint32_t hash = hash_code(element->seqnum, m->capacity);

	map_elem runner = m->elements[hash];
	while(runner != NULL && runner->seqnum != element->seqnum ) {
		runner = runner->next;
	}

	if(runner == NULL) return 0;
	return 1;
}

/**
* get an element from map
*
* @param map, key, element
*
* @return void
*
**/
void get(map* m, uint32_t key, map_elem* element) {
	uint32_t hash = hash_code(key, m->capacity);
    printf("getting key: %d, hash %d...\n", key, hash);

	map_elem runner = m->elements[hash];
	while(runner != NULL && runner->seqnum != key ) {
	    printf("runner seqnum: %d...\n", runner->seqnum);
		runner = runner->next;
	}
	(*element) = runner;
}

/**
* remove an element from map with a given key
*
* @param map, key
*
* @return void
*
**/
void remove_elem(map* m, uint32_t key) {
	uint32_t hash = hash_code(key, m->capacity);

	map_elem runner = m->elements[hash];
	map_elem marker = runner;
	while(runner != NULL && runner->seqnum != key) {
		marker = runner;
		runner = runner->next;
	}

	if(runner != NULL) {
		marker->next = runner->next;
		free(runner);
	}

}
