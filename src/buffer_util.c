#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "buffer.h"
#include "buffer_util.h"
#include "message_util.h"

void destroy_tree(buf_tree_node root) {
    if( root != NULL ) {
        destroy_tree(root->left);
        destroy_tree(root->right);
        free( root );
	root = NULL;
    }
}

uint32_t get_seqnum(packet pac) {
    return ((akh_pdu_header *)pac)->seq_num;
}

// return 1 if insert successfully, 0 if failed. (usually, due to the duplicate of node)
int insert(packet pac, buf_tree_node * root) {
    if( (*root) == NULL ) {
        (*root) = (buf_tree_node) malloc( sizeof( struct node ) );
        (*root)->pac = pac;
        /* initialize the children to null */
        (*root)->left = NULL;    
	(*root)->right = NULL;  
	return 1;
    }
    else if(get_seqnum(pac) < get_seqnum((*root)->pac)) {
        return insert( pac, &((*root)->left) );
    }
    else if(get_seqnum(pac) > get_seqnum((*root)->pac)) {
        return insert( pac, &((*root)->right) );
    }
    return 0;
}

void inorder_tree_walk(buf_tree_node root, packet* pac_array, int* index) {
    if(root == NULL) return;
    inorder_tree_walk(root->left, pac_array, index);
    pac_array[(*index)++] = root->pac;
    inorder_tree_walk(root->right, pac_array, index);
}

akh_buffer create_buff() {
    akh_buffer buff = (akh_buffer)malloc(sizeof(struct buffer));
    buff->availability = MAX_BUFFER_SIZE;
    buff->size = 0;
    buff->root = NULL;
    buff->pac_array = NULL;
    return buff;
}

void clear_buff(akh_buffer buff) {
  if( buff != NULL ) {
      buff->availability = MAX_BUFFER_SIZE;
      buff->size = 0;
      destroy_tree(buff->root);
      free(buff->pac_array);
      buff->pac_array = NULL;
  }
}

int size(akh_buffer buff) {
    return buff->size;
}

// return 1 if push into buffer successfully, return -1 if buffer is full, return 0 if the pac is repeated
int push(packet pac, akh_buffer buff) {
    if(get_pac_max() > buff->availability) return -1;
    if(insert(pac, &(buff->root))) {
        buff->size++;
	return 1;
    }

    return 0;
}

// after call get, do call clear so that it function correctly next time we call it
packet get(int index, akh_buffer buff) {
    if(buff->pac_array == NULL) {
        buff->pac_array = (packet*) malloc(sizeof(packet) * size(buff));
	// inorder tree walk
	int i = 0;
        inorder_tree_walk(buff->root, buff->pac_array, &i);
    }
    return buff->pac_array[index];
}
