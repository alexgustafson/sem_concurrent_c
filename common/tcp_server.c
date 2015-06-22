
#include "tcp_server.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define MAX_LINE 16384
#define QUEUE_SIZE 16
#define RCVBUFSIZE 32

void *handle_tcp_client(void *arg);


struct thread_arg {
    int client_socket;
    int thread_idx;
};


int shutdown_server = 0;
int server_is_running = 0;

void *runserver(void *arg)
{
    int listener;
    fd_set readset;
    listener = open_listener_socket();

    //make_nonblocking(listener);
    bind_to_port(listener, 65002);
    FD_ZERO(&readset);

    //Listen
    if (listen(listener, QUEUE_SIZE) == -1)
        perror("Can't listen");
    
    //Accept a connection
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int idx = 0;
    
    while (!shutdown_server) {
        server_is_running = 1;
        //int maxfd = listener;
        /*FD_SET(listener, &readset);
        
        struct timeval tv = {1, 0}; // timeout on timeval to check if server should shutdown
        int select_return;
        if ((select_return = select(maxfd+1, &readset, NULL, NULL, &tv)) < 0) {
            perror("select");
            return (void *) NULL;
        }
        
        if (select_return == 0 && shutdown_server == 1) {
            break;
        }
        printf("tick");
        */
        int connect_d = accept(listener, (struct sockaddr *) &client_addr, &address_size);
        if (connect_d == -1){
            perror("Can't open secondary socket");
            continue;
        }
        pthread_t thread;
        printf("accepting new client on socket %i\r\n", connect_d);
        struct thread_arg *thread_data = (struct thread_arg *) malloc(sizeof(struct thread_arg));
        thread_data->client_socket = connect_d;
        thread_data->thread_idx = idx;
        idx++;
        pthread_create(&thread, NULL, handle_tcp_client, thread_data);
        pthread_detach(thread);
        
    }
    close(listener);
    server_is_running = 0;
    return (void *) NULL;
}

int startserver()
{
    shutdown_server = 0;
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, runserver, NULL);
    return 0;
}

void stopserver()
{
    shutdown_server = 1;
}

void *handle_tcp_client(void *arg) {
    printf("client thread started");

    int client_socket = *((int *) arg);
    free(arg); /* malloc was made before starting this thread */
    char command[RCVBUFSIZE];      /* Buffer for square string */
    int recv_msg_size;                    /* Size of received message */
    
    while (1) {
        /* Receive message from client */
        recv_msg_size = recv(client_socket, command, RCVBUFSIZE - 1, 0);
        //handle_error(recv_msg_size, "recv() failed", PROCESS_EXIT);
        printf("recieved message");
        if (recv_msg_size == 0) {
            /* zero indicates end of transmission */
            break;
        }
        printf(command);
        command[recv_msg_size] = '\000';
        /* Send received string and receive again until end of transmission */
        /* Square message and send it back to client */
   
    }
    
    close(client_socket);    /* Close client socket */
    return NULL;}

int server_running()
{
    return server_is_running;
}

