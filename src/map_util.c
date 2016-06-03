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

int put(map* m, uint32_t key, map_elem element) {
	uint32_t hash = hash_code(key, m->capacity);
	
	if(m->elements[hash] == NULL) {
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

int contains(map* m, map_elem element) {
	uint32_t hash = hash_code(element->seqnum, m->capacity);

	map_elem runner = m->elements[hash];
	while(runner != NULL && runner->seqnum != element->seqnum ) {
		runner = runner->next;
	}

	if(runner == NULL) return 0;
	return 1;
}

void get(map* m, uint32_t key, map_elem* element) {
	uint32_t hash = hash_code(key, m->capacity);
	
	map_elem runner = m->elements[hash];
	while(runner != NULL && runner->seqnum != key ) {
		runner = runner->next;
	}

	(*element) = runner;
		
}

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
