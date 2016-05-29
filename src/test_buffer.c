#include <stdio.h>
#include <string.h>

#include "message_util.h"
#include "buffer.h"
#include "buffer_util.h"

int main(void) {
	// test tree

	buf_tree_node root = 0;
	akh_pdu_header header = {0, RD, 1234567, 0};
	packet pac;
	akh_pdu_body body = "test";
	createPacket(&pac, &header, body, strlen(body));

	insert(pac, root);
	printf("data: %s\n", root->pac);

	return 0;
}
