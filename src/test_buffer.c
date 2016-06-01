#include <stdio.h>
#include <string.h>

#include "message.h"
#include "message_util.h"
#include "buffer.h"
#include "buffer_util.h"

int main(void) {
	// test swap
	printf("test swap\n");
	buffer_element* elements = (buffer_element*) malloc(sizeof(buffer_element) * 3);
	int i;
	for(i = 0; i < 3; i++) {
		packet pac;
		akh_pdu_header header = {0, RD, 1234567 + i, 0};
		elements[i].pac_size = createPacket(&pac, &header, "test", strlen("test"));
		elements[i].pac = pac;
	}
	printf("before swap\n");
	printf("seq_num: %u\n", ((akh_pdu_header *)elements[0].pac)->seq_num);
	printf("seq_num: %u\n", ((akh_pdu_header *)elements[2].pac)->seq_num);
	swap(elements, 0, 2);
	printf("after swap\n");
	printf("seq_num: %u\n", ((akh_pdu_header *)elements[0].pac)->seq_num);
	printf("seq_num: %u\n", ((akh_pdu_header *)elements[2].pac)->seq_num);
	printf("==========\n\n");
	
	// test heapify
	printf("test heapify");
	printf("before heapify\n");
	printf("seq_num: %u\n", get_seqnum(elements[0].pac));
	heapify(elements, 0, 3);
	printf("after heapify\n");
	printf("seq_num: %u\n", get_seqnum(elements[0].pac));
	printf("==========\n\n");

	// test heapify_up
	printf("test heapify_up\n");
	swap(elements, 0, 2);
	printf("before heapify_up\n");
	printf("seq_num: %u\n", get_seqnum(elements[0].pac));
	heapify_up(elements, 2, 3);
	printf("after heapify_up\n");
	printf("seq_num: %u\n", get_seqnum(elements[0].pac));
	printf("==========\n\n");

	// test create_buffer
	printf("test create_buffer\n");
	buffer* b;
	create_buffer(&b, 100);
	printf("buffer capacity: %d\n", b->capacity);
	printf("buffer count: %d\n", b->count);
	printf("buffer elements is NULL: %d\n", b->elements == NULL);
	printf("==========\n\n");

	// test push and pop
	printf("test push and pop\n");
	for(i = 0; i < 3; i++) {
		packet pac;
		akh_pdu_header header = {0, RD, 1234567 - i, 0};
		ssize_t pac_size = createPacket(&pac, &header, "test", strlen("test"));
		push(b, pac, pac_size);
	}
	printf("after push, heap size: %d\n", b->count);
	for(i = 0; i < 3; i++) {
		printf("seqnum: %u\n", get_seqnum(b->elements[i].pac));
	}

	packet pac;
	ssize_t pac_size;
	printf("poping element\n");
	for(i = 0; i < 3; i++) {
		pop(b, &pac, &pac_size);
		printf("seqnum: %u\n", get_seqnum(pac));
		printf("pac_size: %zdc\n", pac_size);
	}

	printf("==========\n\n");

	// test free_buffer
	printf("test free_buffer\n");
	free_buffer(&b);
	printf("is buffer NULL: %d\n", b == NULL);
	printf("==========\n\n");

    return 0;
}
