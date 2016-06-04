/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: Server supports multi client. Each client is assigned a server thread. 
 *	    These server threads are isolated from the socket. 
 *	    A monitor thread is responsible for socket receive operations. 
 *	    Original timer_recvfrom() function is replaced with read_with_timeout. 
 *	    The communication between monitor and each server thread is done through fifo pipes  * */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>

#include "thread_util.h"

/******************** CONCURRENT **********************/
clnt_thread_rcv_t read_with_timeout(int read_fd, time_t timeout_sec, suseconds_t timeout_usec, int num_try) {
    fd_set set;
    struct timeval timeout;
    
    FD_ZERO(&set);
    FD_SET(read_fd, &set);
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = timeout_usec;
    
    // FIXME: FD_SETSIZE might cause problem
    int select_result = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
    
    clnt_thread_rcv_t ret;
    
    for(int i = 0; i < num_try; ++i) {
        if(select_result == 0) {
            ret.pac = NULL;
            ret.pac_len = 0;
        }
        else if (select_result < 0) {
            perror("select error");
            exit(EXIT_FAILURE);    
        }
        else {
            int read_result = read(read_fd, &ret, sizeof(clnt_thread_rcv_t));
            if(read_result != sizeof(clnt_thread_rcv_t)) {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    return ret;
}

// thread call back function
void* clnt_thread_callback(void* thread_arg) {
    printf("I am a new thread being created with argument %d\n", ((argstruct*)thread_arg)->clnt_pipe_read_fd);
    int serv_sock = ((argstruct*)thread_arg)->serv_sock;
    
    char filename[MAX_FILENAME_SIZE];
    off_t filesize;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    // store thread infomation into TLS(thread local storage).
    pthread_setspecific(thread_key, thread_arg);
    
    int request_type = handle_request_pipe(&clnt_adr, &clnt_adr_sz, filename, &filesize);    
    if(request_type == RD) {
        puts("I clnt thread gets RD packet");
        connection_download_server(serv_sock, &clnt_adr, &clnt_adr_sz, filename, &filesize);

        akh_disconn_response disconn_response;
        disconn_response.segment_list = NULL;
        
        while(wait_disconnection_pipe(&clnt_adr, &clnt_adr_sz, &disconn_response) != 0) {
            send_file(serv_sock, &clnt_adr, filename, &disconn_response);
            request_close(serv_sock, &clnt_adr);
        }
    }
    else if(request_type == RU) {
        puts("I clnt thread gets RU packet");
        connection_upload_server(serv_sock, &clnt_adr, &clnt_adr_sz);

        uint32_t body_size = 100; 
        int num_time_out = 0;
        while(num_time_out < 3 && handle_request_close(serv_sock, &clnt_adr, filename, filesize, body_size) != 0) {
            int msg_type = receive_file_pipe(&clnt_adr, &clnt_adr_sz, filename, body_size);
            if(msg_type == -1) // problem in connection
                num_time_out++;
            else
                num_time_out = 0;
 
        }
        if(num_time_out > 2)
            error_handling("connection error");
    }

    return NULL;
}
