
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
#define RCVBUFSIZE 256

void *handle_tcp_client(void *arg);

struct thread_arg {
    int client_socket;
    int thread_idx;
};

join_cb join_callback;
leave_cb leave_callback;
take_cb take_callback;
size_cb size_callback;
status_cb status_callback;
winner_cb winner_callback;

int server_port = 0;

void register_callbacks(join_cb jcb, leave_cb lcb, take_cb tcb, size_cb scb, status_cb stcb, winner_cb wcb)
{
    join_callback = jcb;
    leave_callback = lcb;
    take_callback = tcb;
    size_callback = scb;
    status_callback = stcb;
    winner_callback = wcb;
}

int shutdown_server = 0;
int server_is_running = 0;
int all_sockets[255];
char all_names[255][255];
int game_finished = 0;
int winner_established = 0;

int say(int socket, char *s, int check_winner) {
    int result;
    if (socket < 1) {
        return 0;
    }
    
    if (winner_established == 1 && check_winner > 0) {
        char response[1000];
        sprintf(response, "END %s \n", all_names[winner_callback()]);
        result = send(socket, response, strlen(response), 0);
    }else {
        result = send(socket, s, strlen(s), 0);
    }
    
    if (result == -1)
        fprintf(stderr, "%s: %s socket: %i \n", "Error sending message",  strerror(errno), socket);
    return result;
        
}

void *run_referee(void *arg) {
    
    while (!shutdown_server) {
        if (winner_callback() > -1) {
            winner_established = 1;
            printf("Referee says there is a winner!\n");
        }else{
            winner_established = 0;
            printf("Referee says no winner.\n");

        }
        sleep(2);
    }
    return NULL;
}

void *runserver(void *arg)
{
    int listener;
    fd_set readset;
    listener = open_listener_socket();

    //make_nonblocking(listener);
    bind_to_port(listener, server_port);
    FD_ZERO(&readset);

    //Listen
    if (listen(listener, QUEUE_SIZE) == -1)
        perror("Can't listen");
    
    //Accept a connection
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int idx = 0;
    
    all_sockets[idx] = listener;
    pthread_t referee;
    pthread_create(&referee, NULL, run_referee, NULL);
    
    
    while (!shutdown_server) {
        server_is_running = 1;

        int connect_d = accept(listener, (struct sockaddr *) &client_addr, &address_size);
        if (connect_d == -1){
            if (shutdown_server == 1) {
                break;
            }
            perror("Can't open secondary socket");
            break;
        }
        pthread_t thread;
        printf("SERVER: accepting new client on socket %i\r\n", connect_d);
        struct thread_arg *thread_data = (struct thread_arg *) malloc(sizeof(struct thread_arg));
        thread_data->client_socket = connect_d;
        
        idx++;
        thread_data->thread_idx = idx;
        
        pthread_create(&thread, NULL, handle_tcp_client, thread_data);
        pthread_detach(thread);
        
    }
    if (all_sockets[0] > 0) {
        close(all_sockets[0]);
    }
    pthread_join (referee, NULL);
    server_is_running = 0;
    return (void *) NULL;
}

int startserver(int port)
{
    server_port = port;
    shutdown_server = 0;
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, runserver, NULL);
    shutdown_server = 0;
    server_is_running = 0;
    game_finished = 0;
    winner_established = 0;


    return 0;
}

void stopserver()
{
    shutdown_server = 1;
    for (int i = 0; i < 256; i++) {
        if (all_sockets[i] > 0) {
            close(all_sockets[i]);
            all_sockets[i] = 0;
        }
    }
    
    for (int i = 0; i < 256; i++)
    {
        strcpy(all_names[i], "");
    }
    join_callback = NULL;
    leave_callback = NULL;
    take_callback = NULL;
    size_callback = NULL;
    status_callback = NULL;
    winner_callback = NULL;
}



void *handle_tcp_client(void *arg) {
    printf("SERVER: client thread started");
    
    struct thread_arg *thread_data = (struct thread_arg *)arg;

    int client_socket = thread_data->client_socket;
    int idx = thread_data->thread_idx;
    
    free(arg); /* malloc was made before starting this thread */
    char command[RCVBUFSIZE];      /* Buffer for square string */
    char response[RCVBUFSIZE];
    int recv_msg_size;                    /* Size of received message */
    
    all_sockets[idx] = client_socket;
    int is_joined = 0;
    
    
    while (!shutdown_server) {
        
        /* Receive message from client */
        command[0] = '\0';
        recv_msg_size = recv(client_socket, command, RCVBUFSIZE - 1, 0);
        command[recv_msg_size] = '\0';
        
        if (recv_msg_size == 0) {
            /* zero indicates end of transmission */
            break;
        }
        
        printf("server recieved message: %s \n", command);
        
        if (shutdown_server) {
            break;
        }
        
        if(strncasecmp(command, "HELLO\n", 5) == 0)
        {
            if (is_joined) {
                //already joined, so skip this
                say(all_sockets[idx], "ALREADY JOINED DUMMY\n", 0);
                continue;
            }
            
            int field_size = size_callback();
            sprintf(response, "SIZE %i \n", field_size);
            say(all_sockets[idx], response, 0);
            
            if (join_callback() == 0) {
                
                is_joined = 1;
                say(all_sockets[idx], "START\n", 1);
                
            }else{
                say(all_sockets[idx], "NACK\n", 1);
            }
            
        }else if (strncasecmp(command, "SHUTDOWN\n", 7) == 0)
        {
            stopserver();
            break;
            
        }else if (strncasecmp(command, "TAKE", 4)== 0)
        {
            int coords[2];
            char name[1000];
            
            parse_take(command, coords, name);
            int success = take_callback(coords[0], coords[1], idx);
            if  (success == 1) {
                strcpy(all_names[idx], name);
                say(all_sockets[idx], "TAKEN \n", 1);
            }else {
                say(all_sockets[idx], "INUSE \n", 1);
            }
                        
        }else if (strncasecmp(command, "STATUS", 6)== 0)
        {
            int coords[2];
            parse_status(command, coords);
            
            int player_id = status_callback(coords[0], coords[1]);
            say(all_sockets[idx], all_names[player_id], 1);

        }
        
        command[recv_msg_size] = '\0';
   
    }
    if (all_sockets[idx] > 0) {
        close(client_socket);    /* Close client socket */
        all_sockets[idx] = 0;
    }
    
    return NULL;
}

int server_running()
{
    return server_is_running;
}

void send_to_all_sockets( char* message) {
    for (int i = 1; i < 256; i++) {
        if (all_sockets[i] > 0) {
            say(all_sockets[i], message, 0);
        }
    }
}





