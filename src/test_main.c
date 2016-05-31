#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "file_util.h"

int main()
{
    char filename[] = "test.txt";
    char filename2[] = "test2.txt";
    off_t filesize = get_file_size(filename);
    char buf[500];
    size_t seg_size = 2;
    int num_skip = 3;
    size_t len;

    for(num_skip = 0; num_skip * seg_size < filesize; num_skip++) {
        len = read_file_segment(buf, seg_size, num_skip, filename);
        write_file_segment(buf, len, filename2);
        printf("read & write => %s\n", buf);
    }

    return 0;
}

/* size_t read_file_segment(void *buffer, size_t size, int num_skip, char *filename); */
/* size_t write_file_segment(void *buffer, size_t size, char *filename); */


