#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "buffer.h"
#include "buffer_util.h"
#include "message_util.h"

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

void swap(packet* pac_array, unsigned int i, unsigned int j) {
	packet temp = pac_array[i];
	pac_array[i] = pac_array[j];
	pac_array[j] = temp;
}

void heapify(packet* pac_array, unsigned int index, unsigned int count) {
	if(index >= count) return;
	unsigned int rightIndex = get_right(index);
	unsigned int leftIndex = get_left(index);

	unsigned int min = index;
	if(rightIndex < count && CMP(pac_array[rightIndex], pac_array[min])) {
		min = rightIndex;
	}
	if(leftIndex < count && CMP(pac_array[leftIndex], pac_array[min])) {
		min = leftIndex;
	}
	if(min != index) {
		swap(pac_array, min, index);
		index = min;
		heapify(pac_array, index, count);
	}
}

void heapify_up(packet* pac_array, unsigned int index, unsigned int count) {
	if(index >= count) return;
	unsigned int parent = get_parent(index);

	if(CMP(pac_array[index], pac_array[parent])) {
		swap(pac_array, parent, index);
		index = parent;
		heapify_up(pac_array, index, count);
	}
}

void create_buffer(buffer** h, unsigned int capacity) {
	if(capacity <= 0) {
		*h = NULL;
	}

	packet* pac_array = (packet*) malloc(sizeof(packet) * capacity);
	*h = (buffer*) malloc(sizeof(buffer));
	(*h)->capacity = capacity;
	(*h)->count = 0;
	(*h)->pac_array = pac_array;
}

void free_buffer(buffer** h) {
	(*h)->capacity = 0;
	(*h)->count = 0;
	free((*h)->pac_array);
	(*h)->pac_array = NULL;
	free((*h));
	*h = NULL;
}

// return 1 if successful, 0 if the buffer is filled, -1 if error
int push(buffer* h, packet pac) {
	if(h == NULL) return -1;
	if(h->count == h->capacity) return 0;

	h->pac_array[h->count] = pac;
	
	h->count++;
	heapify_up(h->pac_array, h->count - 1, h->count);

	return 1;
}

// return 1 if successful, 0 if buffer is empty, -1 if error 
int pop(buffer* h, packet* pac) {
	if(h == NULL) return -1;
	if(h->count == 0) return 0;

	*pac = h->pac_array[0];

	swap(h->pac_array, 0, h->count - 1);
	h->count--;
	heapify(h->pac_array, 0, h->count);

	return 1;
}
