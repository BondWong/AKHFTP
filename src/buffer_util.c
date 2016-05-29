#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "buffer.h"

void destroy_tree(buf_tree_node root) {
	if( root != 0 ) {
    	destroy_tree(root->left);
    	destroy_tree(root->right);
    	free( root );
  	}
}

uint32_t get_seqnum(packet pac) {
	return ((akh_pdu_header *)pac)->seq_num;
}

void insert(packet pac, buf_tree_node root) {
    if( root == 0 ) {
        root = (buf_tree_node) malloc( sizeof( struct node ) );
        root->pac = pac;
        /* initialize the children to null */
        root->left = 0;    
        root->right = 0;  
    }
    else if(get_seqnum(pac) < get_seqnum(root->pac)) {
        insert( pac, root->left );
    }
    else if(get_seqnum(pac) > get_seqnum(root->pac)) {
        insert( pac, root->right );
    }
}

void inorder_tree_walk(buf_tree_node root, packet* pac_array, int* index) {
	if(root == 0) return;
	inorder_tree_walk(root->left, pac_array, index);
	pac_array[*(index)++] = root->pac;
	inorder_tree_walk(root->right, pac_array, index);
}

akh_buffer create_buff() {
	akh_buffer buff = (akh_buffer) malloc(sizeof(struct buffer));
	return buff;
}

void clear_buff(akh_buffer buff) {
  if( buff != 0 ) {
  	buff->available = MAX_BUFFER_SIZE;
  	buff->size = 0;
    destroy_tree(buff->root);
    free( buff );
  }
}

int size(akh_buffer buff) {
	return buff->size;
}

// return 0 if push into buffer successfully, return -1 if buffer is full
int push(packet pac, akh_buffer buff) {
	if(sizeof(packet) * strlen((char*)pac) > buff->available) return -1;
	insert(pac, buff->root);
	buff->size++;

	return 0;
}

// after call get, do call clear so that it function correctly next time we call it
packet get(int index, akh_buffer buff) {
	if(buff->pac_array == 0) {
		buff->pac_array = (packet*) malloc(sizeof(packet) * size(buff));
	}
	// inorder tree walk
	int i = 0;
	inorder_tree_walk(buff->root, buff->pac_array, &i);
	return buff->pac_array[index];
}