#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

off_t get_file_size(char *filename);

#endif

