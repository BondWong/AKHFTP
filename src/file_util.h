/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: the header file of file_util.c
 *     the functions in the file help to manage files. For example, get file size or check file existance. Specially read_file_segment can read block of the file and write_file_segment can write block of the file
 * * */
#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

// check file existance
int check_file_status(char *filename);

// get file size
off_t get_file_size(char *filename);

// read a block of the file
size_t read_file_segment(void *buffer, size_t size, int num_skip, char *filename);

// write a block of the file
size_t write_file_segment(void *buffer, size_t size, char *filename);

#endif

