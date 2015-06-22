/*
  ==============================================================================

    common.h
    Created: 17 Jun 2015 5:18:24pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#ifdef __cplusplus  // PITA : http://stackoverflow.com/questions/9334650/linker-error-calling-c-function-from-objective-c
extern "C" {
#endif

#import <sys/errno.h>
#include <sys/uio.h>
#include <sys/fcntl.h>
#include <signal.h>
    
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

const int MAX_LINE_LENGTH = 1000;

//commands
const int OPEN_FILE = 10;
const int SEND_FILE = 20;


char* string_for_command(int command)
{
    char* string;
    
    switch (command) {
        case OPEN_FILE:
            string = "OPEN_FILE";
            break;
        case SEND_FILE:
            string = "SEND_FILE";
            break;
            
        default:
            string = "";
            break;
    }
    return string;
}

typedef struct Command {
    int version;
    int command;
    
} Command;


Command *new_command()
{
    Command *new_command = (Command *)malloc(sizeof(Command));
    new_command->version = 001;
    new_command->command = 0;

    return new_command;
}



void error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int open_listener_socket() {
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == -1)
        error("Can't open socket");
    return s;
}

void make_nonblocking(int fd)
{
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

void bind_to_port(int socket, int port) {
    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = (in_port_t) htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse)) == -1)
        error("Can't set the reuse option on the socket");
    int c = bind(socket, (struct sockaddr *) &name, sizeof(name));
    if (c == -1)
        error("Can't bind to socket q");
}

int read_command(Command *cmd, int socket) {
    char *buffer = (char *)cmd;
    int slen = sizeof(Command);
    int c = (int)recv(socket, buffer, sizeof(Command), 0);
    while ((c > 0) && (c < sizeof(Command))) {
        buffer += c;
        slen -= c;
        c = (int)recv(socket, buffer, slen, 0);
    }
    if (c < 0)
        //indicates an error, so send
        // back c to trigger and error check
        return c;
    
    printf("Recieved Command: %s\r\n", string_for_command(cmd->command));
    //printf("With Data: %s\r\n", cmd->data);
    
    return 1;
}


void send_command(int socket, Command *cmd) {
    size_t result = send(socket, cmd, sizeof(Command), 0);
    if (result == -1)
        fprintf(stderr, "%s: %s\n", "Error sending message", strerror(errno));
}

int catch_signal(int sig, void (*handler)(int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

void parse_command(Command *cmd, char *command_string) {
    char *token = strtok(command_string, " ");
    cmd->version = 001;


    if (strncasecmp("take", token, 8) == 0) {
        cmd->command = OPEN_FILE;
        
    } else {
        cmd->command = 0;
    }
}

int say(int socket, char *s) {
        char string[500];
        sprintf(string, "%s\n", s);
        int result = send(socket, string, strlen(string), 0);
        if (result == -1)
            fprintf(stderr, "%s: %s\n", "Error sending message", strerror(errno));
        return result;
}

    
#ifdef __cplusplus
}
#endif

#endif  // COMMON_H_INCLUDED
