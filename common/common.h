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

#include <sys/errno.h>
#include <sys/uio.h>
#include <sys/fcntl.h>
#include <signal.h>
    
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

const int MAX_LINE_LENGTH = 1000;

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



    
void parse_take(char *buffer, int *coords, char *name)
{
    int count = 0;
    char *word = &buffer[0];
    char *sp;
    for(word = strtok_r(buffer, " ", &sp); word; word = strtok_r(NULL, " ",&sp))
    {
        if(count == 0)
        {

        }else if(count > 0 && count < 3){
            coords[count - 1] = atoi(word);
        }else if(count == 3){
            strcpy(name, word);
        }else{
            break;
        }
        count++;
    }
}
    
void parse_status(char *buffer, int *coords)
{
    int count = 0;
    char *word = &buffer[0];
    char *sp;
    for(word = strtok_r(buffer, " ", &sp); word; word = strtok_r(NULL, " ",&sp))
    {
        if(count == 0)
        {
            
        }else if(count > 0 && count < 3){
            coords[count - 1] = atoi(word);
        }else{
            break;
        }
        count++;
    }
}

    
#ifdef __cplusplus
}
#endif

#endif  // COMMON_H_INCLUDED
