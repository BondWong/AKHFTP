/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: the header file of file_util.c
 * * */
#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int check_file_status(char *filename);
off_t get_file_size(char *filename);

size_t read_file_segment(void *buffer, size_t size, int num_skip, char *filename);
size_t write_file_segment(void *buffer, size_t size, char *filename);

#endif

