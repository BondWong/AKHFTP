/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: the header file of map_util.c
 *     the functions in the file help to manage map
 * * */
#ifndef __MAP_UTIL_H__
#define __MAP_UTIL_H__

#include <stdint.h>

#include "map.h"

uint32_t hash_code(uint32_t key, unsigned int capacity);
map* create_map(unsigned int capacity);
void free_map(map** m);
int put(map* m, uint32_t key, map_elem element);
int contains(map* m, map_elem element);
void get(map* m, uint32_t key, map_elem* element);
void remove_elem(map* m, uint32_t key);

#endif
