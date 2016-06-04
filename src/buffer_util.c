/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: the header file of buffer_util.c
 *	    1. get_seqnum: get seqence number from packet
 *	    2. get_parent: get parent index
 *	    3. get_right: get right child index
 *	    4. get_left: get left child index
 *	    5. swap: swap two heap node in the array
 *      6. heapify: arrange elements to form a heap by floating elements down
 *      7. heapify_up: arrange elements to form a heap by floating elements up
 *      8. create_buffer: create a buffer, allocating memory and initializing fileds
 *      9. free_buffer: free memory that is allocated to the buffer
 *      10. push: add an element into buffer
 *      11. pop: get an element out from buffer
 * * */
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "buffer.h"
#include "buffer_util.h"
#include "message_util.h"
#include "map_util.h"

#define CMP(a, b) (get_seqnum(a) < get_seqnum(b))

uint32_t get_seqnum(packet pac) {
    return ((akh_pdu_header *)pac)->seq_num;
}

unsigned int get_parent(unsigned int index) {
	if(index == 0) return 0;
	return (index - 1) / 2;
}

unsigned int get_right(unsigned int index) {
	return index * 2 + 2;
}

unsigned int get_left(unsigned int index) {
	return index * 2 + 1;
}

void swap(buffer_element* elements, unsigned int i, unsigned int j) {
	buffer_element temp = elements[i];
	elements[i] = elements[j];
	elements[j] = temp;
}

void heapify(buffer_element* elements, unsigned int index, unsigned int count) {
	if(index >= count) return;
	unsigned int rightIndex = get_right(index);
	unsigned int leftIndex = get_left(index);

	unsigned int min = index;
	if(rightIndex < count && CMP(elements[rightIndex].pac, elements[min].pac)) {
		min = rightIndex;
	}
	if(leftIndex < count && CMP(elements[leftIndex].pac, elements[min].pac)) {
		min = leftIndex;
	}
	if(min != index) {
		swap(elements, min, index);
		index = min;
		heapify(elements, index, count);
	}
}

void heapify_up(buffer_element* elements, unsigned int index, unsigned int count) {
	if(index >= count) return;
	unsigned int parent = get_parent(index);

	if(CMP(elements[index].pac, elements[parent].pac)) {
		swap(elements, parent, index);
		index = parent;
		heapify_up(elements, index, count);
	}
}

void create_buffer(buffer** h, unsigned int capacity) {
	if(capacity <= 0) {
		*h = NULL;
	}

	buffer_element* elements = (buffer_element*) malloc(sizeof(buffer_element) * capacity);
	*h = (buffer*) malloc(sizeof(buffer));
	(*h)->capacity = capacity;
	(*h)->count = 0;
	(*h)->elements = elements;
	(*h)->m = create_map(capacity);
}

void free_buffer(buffer** h) {
	(*h)->capacity = 0;
	(*h)->count = 0;
	free((*h)->elements);
	(*h)->elements = NULL;
	free_map(&((*h)->m));
	free((*h));
	*h = NULL;
}

/**
* push packet into buffer
*
* @param buffer, packet, size of packet
*
* @return int
* @Description: return 1 if successful, 0 if the buffer is filled, -1 if error
*
**/
int push(buffer* h, packet pac, ssize_t pac_size) {
	if(h == NULL) return -1;
	if(h->count == h->capacity) return 0;

	map_elem elem = (map_elem) malloc(sizeof(map_elem));
	elem->seqnum = get_seqnum(pac);
	elem->next = NULL;

	if(contains(h->m, elem) == 1) return 1;

	put(h->m, elem->seqnum, elem);
	buffer_element* element = (buffer_element*) malloc(sizeof(buffer_element));
	element->pac = pac;
	element->pac_size = pac_size;
	h->elements[h->count] = *element;
	
	h->count++;
	heapify_up(h->elements, h->count - 1, h->count);

	return 1;
}

/**
* pop packet from buffer
*
* @param buffer, packet, size of packet
*
* @return int
* @Description: return 1 if successful, 0 if buffer is empty, -1 if error 
*
**/
int pop(buffer* h, packet* pac, ssize_t* pac_size) {
	if(h == NULL) return -1;
	if(h->count == 0) return 0;

	*pac = h->elements[0].pac;
	*pac_size = h->elements[0].pac_size;
	remove_elem(h->m, get_seqnum(h->elements[0].pac));

	swap(h->elements, 0, h->count - 1);
	h->count--;
	heapify(h->elements, 0, h->count);

	return 1;
}
