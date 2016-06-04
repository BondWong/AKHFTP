/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: the header file of buffer_util.c
 *     the functions in the file help to manage buffer
 * * */
#include "error_handling.h"
#include "file_util.h"

// if there is no file, return -1
int check_file_status(char *filename)
{
    struct stat fileinfo;
    return stat(filename, &fileinfo);
}


off_t get_file_size(char *filename)
{
    struct stat fileinfo;
    if(stat(filename, &fileinfo) == -1) {
    	// error_handling("get file size error");
        return 0;
    }
    return fileinfo.st_size;
}

size_t read_file_segment(void *buffer, size_t size, int num_skip, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        return 0;
    fseek(fp, size * num_skip, SEEK_SET);
    size_t len = fread(buffer, 1, size, fp);
    fclose(fp);
    return len;
}

size_t write_file_segment(void *buffer, size_t size, char *filename)
{
    FILE *fp = fopen(filename, "a");
    if(fp == NULL)
        return 0;
    size_t len = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return len;
}

