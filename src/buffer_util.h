#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "buffer.h"

void destroy_tree(buf_tree_node root);
uint32_t get_seqnum(packet pac);
void insert(packet pac, buf_tree_node root);
void inorder_tree_walk(buf_tree_node root, packet* pac_array, int* index);

akh_buffer create_buff();
void clear_buff(akh_buffer buff);
int size(akh_buffer buff);
int push(packet pac, akh_buffer buff);
packet get(int index, akh_buffer buff);