#include <stdio.h>
#include <string.h>

#include "message.h"
#include "message_util.h"
#include "buffer.h"
#include "buffer_util.h"

int main(void) {
	// test swap
	printf("test swap\n");
	packet* pac_array = (packet*) malloc(sizeof(packet) * 3);
	int i;
	for(i = 0; i < 3; i++) {
		packet pac;
		akh_pdu_header header = {0, RD, 1234567 + i, 0};
		createPacket(&pac, &header, "test", strlen("test"));
		pac_array[i] = pac;
	}
	printf("before swap\n");
	printf("seq_num: %u\n", ((akh_pdu_header *)pac_array[0])->seq_num);
	printf("seq_num: %u\n", ((akh_pdu_header *)pac_array[2])->seq_num);
	swap(pac_array, 0, 2);
	printf("after swap\n");
	printf("seq_num: %u\n", ((akh_pdu_header *)pac_array[0])->seq_num);
	printf("seq_num: %u\n", ((akh_pdu_header *)pac_array[2])->seq_num);
	printf("==========\n\n");
	
	// test heapify
	printf("test heapify");
	printf("before heapify\n");
	printf("seq_num: %u\n", get_seqnum(pac_array[0]));
	heapify(pac_array, 0, 3);
	printf("after heapify\n");
	printf("seq_num: %u\n", get_seqnum(pac_array[0]));
	printf("==========\n\n");

	// test heapify_up
	printf("test heapify_up\n");
	swap(pac_array, 0, 2);
	printf("before heapify_up");
	printf("seq_num: %u\n", get_seqnum(pac_array[0]));
	heapify_up(pac_array, 2, 3);
	printf("after heapify_up");
	printf("seq_num: %u\n", get_seqnum(pac_array[0]));
	printf("==========\n\n");

	// test create_buffer
	printf("test create_buffer\n");
	buffer* b;
	create_buffer(&b, 100);
	printf("buffer capacity: %d\n", b->capacity);
	printf("buffer count: %d\n", b->count);
	printf("buffer pac_array: %d\n", b->pac_array == NULL);
	printf("==========\n\n");

	// test push and pop
	printf("test push and pop\n");
	for(i = 0; i < 3; i++) {
		packet pac;
		akh_pdu_header header = {0, RD, 1234567 - i, 0};
		createPacket(&pac, &header, "test", strlen("test"));
		push(b, pac);
	}
	printf("after push, heap size: %d\n", b->count);
	for(i = 0; i < 3; i++) {
		printf("seqnum: %u\n", get_seqnum(b->pac_array[i]));
	}

	packet pac;
	printf("poping element\n");
	for(i = 0; i < 3; i++) {
		pop(b, &pac);
		printf("seqnum: %u\n", get_seqnum(pac));
	}

	printf("==========\n\n");

	// test free_buffer
	printf("test free_buffer");
	free_buffer(&b);
	printf("is buffer NULL: %d\n", b == NULL);

    return 0;
}
