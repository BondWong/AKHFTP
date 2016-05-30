#include <stdio.h>
#include <string.h>

#include "message.h"
#include "message_util.h"
#include "buffer.h"
#include "buffer_util.h"

int main(void) {
    // test insertion 
    printf("test insertion\n");
    akh_pdu_header header = {0, RD, 1234567, 0};
    akh_pdu_body body = "test";
    packet pac;
    createPacket(&pac, &header, body, strlen(body));
    printf("pac seqnum: %u\n", ((akh_pdu_header *) pac)->seq_num);

    buf_tree_node root = NULL;
    insert(pac, &root);
    printf("tree pac seqnum: %u\n", ((akh_pdu_header *)root->pac)->seq_num);
    printf("==========\n\n");

    // test get_seqnum
    printf("test get_seqnum\n");
    printf("pac seqnum: %u\n", ((akh_pdu_header *) pac)->seq_num);
    printf("get_seqnum: %u\n", get_seqnum(pac));
    printf("==========\n\n");

    // test inorder tree walk
    printf("test inorder tree walk\n");
    int seqnum = 1234568;
    int i = 0;
    for(i = 0; i < 11; i++, seqnum++) {
	    akh_pdu_header h = {0, RD, seqnum, 0};
	    packet packet;
	    createPacket(&packet, &h, body, strlen(body));
	    insert(packet, &root);
    } 
    int index = 0;
    packet* pac_array = (packet*)malloc(sizeof(packet) * 11);
    inorder_tree_walk(root, pac_array, &index);
    for(i = 0; i < 12; i++) {
	    printf("pac seqnum: %u\n", ((akh_pdu_header *)pac_array[i])->seq_num);
    }
    printf("=========\n\n");

    // test destory tree
    printf("test destroy tree\n");
    destroy_tree(root);
    printf("pac seqnum: %u\n", ((akh_pdu_header *)root->pac)->seq_num);
    printf("=========\n\n");

    // test create buffer
    printf("test create buffer\n");
    akh_buffer buf = NULL;
    buf = create_buff();
    printf("is buf NULL: %d\n", buf == NULL);
    printf("buffer size: %d\n", size(buf));
    printf("buffer availability: %lu\n", buf->availability);
    printf("buffer root: %d\n", buf->root == NULL);
    printf("=========\n\n");

    // test push
    printf("test push\n");
    // when there are enough space
    int result;
    result = push(pac, buf);
    printf("result is: %d\n", result);
    printf("size is: %d\n", size(buf));

    // when duplicate occrus
    result = push(pac, buf);
    printf("result is: %d\n", result);
    printf("size is: %d\n", size(buf));

    // when there are not enough space
    unsigned long availability = buf->availability;
    buf->availability = 0;
    result = push(pac, buf);
    printf("result is: %d\n", result);
    printf("size is %d\n", size(buf));
    buf->availability = availability;

    // test get
    printf("test get\n");
    seqnum = 1234568;
    for(i = 0; i < 11; i++, seqnum++) {
	    akh_pdu_header h = {0, RD, seqnum, 0};
	    packet packet;
	    createPacket(&packet, &h, body, strlen(body));
	    push(packet, buf);
    } 
    for(i = 0; i < size(buf); i++) {
	    packet p = get(i, buf);
	    printf("pac seqnum: %u\n", ((akh_pdu_header *)p)->seq_num);
    }
    printf("=========\n\n");

    // test clear buff
    clear_buff(buf);
    printf("is buf NULL: %d\n", buf == NULL);
    printf("buffer size: %d\n", size(buf));
    printf("buffer availability: %lu\n", buf->availability);
    printf("buffer pac_array: %d\n", buf->pac_array == NULL);
    printf("buffer root: %d\n", buf->root == NULL);
    printf("=========\n\n");

    return 0;
}
