#include "error_handling.h"
#include "file_util.h"

off_t get_file_size(char *filename)
{
    struct stat fileinfo;
    if(stat(filename, &fileinfo) == -1) {
    	// error_handling("get file size error");
        return 0;
    }
    return fileinfo.st_size;
}