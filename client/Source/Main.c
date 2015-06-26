

#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//==============================================================================
int main (int argc, char* argv[])
{
    char *host = NULL;
    int port = 65002 ;
    
    if (argc == 2) {
        host = argv[1];
        port = atoi(argv[2]);
    }else{
        host = "127.0.0.1";
    }
    
    struct sockaddr_in server_address;
    unsigned short server_port = port;
    
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        perror("Can't open socket");
    
    /* Construct the server address structure */
    memset(&server_address, 0, sizeof(server_address));     /* Zero out structure */
    server_address.sin_family = AF_INET;             /* Internet address family */
    server_address.sin_addr.s_addr = inet_addr(host);   /* Server IP address */
    server_address.sin_port = htons(server_port); /* Server port: htons host to network byte order */
    
    int c = connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
    if (c < 0) {
        perror("Can't connect");
    }
    
    char receive_buffer[256];
    char *cmd = NULL;
    size_t size;
    int shutdown = 0;
    while (!shutdown) {
        
        if (getline(&cmd, &size, stdin) == -1)
        {
        }else{
            if(strncasecmp(cmd, "HELLO\n", 5) == 0){
                
                int result = send(sock, cmd, size, 0);
                if (result == -1)
                    fprintf(stderr, "%s:\n", "Error sending message");
                
                int recv_msg_size = recv(sock, receive_buffer, 256, 0);
                if (recv_msg_size == 0) {
                    return 0;
                }
                receive_buffer[recv_msg_size] = '\0';
                fprintf(stderr,"%s", receive_buffer);
                
                if (strncasecmp(receive_buffer, "NACK\n", 4) == 0) {
                    fprintf(stderr,"cannot connect to game\n");
                    shutdown = 1;
                    break;
                }
                
                fprintf(stderr,"waiting for game to start\n");
                
                recv_msg_size = recv(sock, receive_buffer, 256, 0);
                if (recv_msg_size == 0) {
                    return 0;
                }
                receive_buffer[recv_msg_size] = '\0';
                
                
            }else if(strncasecmp(cmd, "TAKE", 4) == 0)
            {
                int result = send(sock, cmd, size, 0);
                if (result == -1)
                    fprintf(stderr, "%s:\n", "Error sending message");
                
                int recv_msg_size = recv(sock, receive_buffer, 256, 0);
                if (recv_msg_size == 0) {
                    return 0;
                }
                receive_buffer[recv_msg_size] = '\0';
                fprintf(stderr,receive_buffer, "");
                
                if (strncasecmp(receive_buffer, "END ", 3) == 0) {
                    shutdown = 1;
                    break;
                }

            }else if(strncasecmp(cmd, "STATUS", 6) == 0)
            {
                int result = send(sock, cmd, size, 0);
                if (result == -1)
                    fprintf(stderr, "%s:\n", "Error sending message");
                
                int recv_msg_size = recv(sock, receive_buffer, 256, 0);
                if (recv_msg_size == 0) {
                    return 0;
                }
                receive_buffer[recv_msg_size] = '\0';
                fprintf(stderr,receive_buffer, "");
                
                if (strncasecmp(receive_buffer, "END ", 3) == 0) {
                    shutdown = 1;
                    break;
                }
                
            }
            
            
        }
    }
    


    close(sock);
    return 0;
}
