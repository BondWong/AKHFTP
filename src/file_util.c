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