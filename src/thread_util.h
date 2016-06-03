#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handling.h"
#include "message.h"
#include "message_util.h"
#include "file_util.h"
#include "security_util.h"
#include "connection.h"
#include "file_transmission.h"
#include "disconnection.h"

pthread_key_t thread_key;

// communicate between monitor-thread using clnt_thread_rcv_t package through pipe
typedef struct {
    char* pac;
    size_t pac_len;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_sz;
} clnt_thread_rcv_t;

// when creating thread, monitor will tell clnt_thread these information:
typedef struct {
	int	clnt_pipe_read_fd;
	int	serv_sock;
} argstruct;

clnt_thread_rcv_t read_with_timeout(int fd, time_t timeout_sec, suseconds_t timeout_usec, int num_try);

// each client is served by one client thread
void* clnt_thread_callback(void* thread_arg);
