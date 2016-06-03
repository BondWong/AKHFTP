/* Class Name: CS544 Computer networks
 * Date: 6/2/2016
 * Group member: Jae Hoon Kim, Junking Huang, Ni An
 * Purpose: This file contains the main function for a AKHFTP server.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "hashmap.h"
#include "thread_util.h"

#define THREAD_MAP_CAPACITY 50
#define PIPE_READ   0
#define PIPE_WRITE  1

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int serv_sock, option;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t optlen;

    // create server socket
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("UDP socket creation error");

    // set socket so that port can be reuse immediately after closure
    option = true;
    optlen = sizeof(option);
    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen) == -1 )
	    error_handling("Setting socket option to enable reusable port error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // initialize a map that takes in clnt_thread_key:pipeFD[PIPE_WRITE] pair
    hashmap* clnt_thread_LUT = create_hashmap(THREAD_MAP_CAPACITY);
    // initialize thread local storage key
    pthread_key_create(&thread_key, NULL);

    // Only the monitor thread will watch the socket
    while(1) {
        puts("Monitor waiting incoming packet...");
    
        char* pac = (char*)malloc(MAX_BUFFER_SIZE*sizeof(char));
        size_t pac_len;
        socklen_t clnt_adr_sz = sizeof(struct sockaddr_in);
        pac_len = recvfrom(serv_sock, pac, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        
        puts("Monitor received packet:");
        printPacket(pac, pac_len);
        
        // convert clnt_adr to uint64_t hashmap key using sockaddr_in.sin_family, sin_port, sin_addr
        uint64_t clnt_thread_key = ((uint64_t)(clnt_adr.sin_addr.s_addr) << 32) | (clnt_adr.sin_port << 16) | clnt_adr.sin_family;
        uint32_t result = 0;
        
        printf("monitor searching key %ld...\n", clnt_thread_key);
        if(hashmap_get(clnt_thread_LUT, clnt_thread_key, &result) < 0) {
            printf("monitor cannot find key %ld...\n", clnt_thread_key);
            // no thread has been created for this client.
            
            // 1: create a pipe for the client thread
            int* pipe_fd = (int*)malloc(sizeof(int)*2);
            if(pipe(pipe_fd) < 0) {
                perror("cannot create pipe for incoming new client.");
                continue;
            }
            printf("monitor created pipe [%d,%d] for clnt thread...\n", pipe_fd[0], pipe_fd[1]);
            
            // 2: add thread writing port to the map
            result = pipe_fd[PIPE_WRITE];
            hashmap_put(clnt_thread_LUT, clnt_thread_key, result);
            
            printf("monitor puts [%ld=>%d] into map...\n", clnt_thread_key, result);
        
            // 3: create thread
            // 4: pass argstruct to thread
            argstruct* threadarg = (argstruct*)malloc(sizeof(argstruct));
            threadarg->clnt_pipe_read_fd = pipe_fd[PIPE_READ];
            threadarg->serv_sock = serv_sock;
            
            printf("monitor ready to create a thread for clnt with port %d\n", *pipe_fd);
            pthread_t thread;
            pthread_create(&thread, NULL, 
                           clnt_thread_callback, (void*)threadarg);  // pipe_fd[PIPE_READ] is the reading port.
        }

        int client_thread_pipe_write_fd = result;
        printf("monitor figured out client thread writing end at %d...\n", result);
        
        // ready the communication package
        clnt_thread_rcv_t package;
        package.pac = pac;
        package.pac_len = pac_len;
        package.clnt_addr = clnt_adr;
        package.clnt_addr_sz = clnt_adr_sz;
        
        // write the communcation package to the pipe
        if(write(client_thread_pipe_write_fd, (void*)&package, sizeof(clnt_thread_rcv_t)) != sizeof(clnt_thread_rcv_t)) {
            perror("cannot pass received package to the client thread.");
            continue;
        }
    }
    
    //  FIXME: memory leak on pipe_fd.
    close(serv_sock);
    return 0;
}

