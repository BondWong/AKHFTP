#include <stdio.h>
#include <string.h>

#include "message.h"
#include "message_util.h"
#include "buffer.h"
#include "buffer_util.h"

int main(void) {
    // test tree

    printf("hello world\n");
    akh_pdu_header header = {0, RD, 1234567, 0};
    akh_pdu_body body = "test";
    packet pac;
    createPacket(&pac, &header, body, strlen(body));

    buf_tree_node root = NULL;
    insert(pac, &root);
    printf("data: %d\t%d\n", pac, root->pac);
    printf("data: %d\n", pac);
    printf("data: %d\n", root);
    printf("hello world2\n");

    return 0;
}
