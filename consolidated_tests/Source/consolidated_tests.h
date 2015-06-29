/*
  ==============================================================================

    consolidated_tests.h
    Created: 23 Jun 2015 3:27:55pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#ifndef CONSOLIDATED_TESTS_H_INCLUDED
#define CONSOLIDATED_TESTS_H_INCLUDED

#include "JuceHeader.h"
#include "tcp_server.h"
#include "field_manager.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_MESSAGE_SIZE 256

class ConsolidatedTests : public UnitTest
{
public:
    ConsolidatedTests() : UnitTest("") {}
    
    void FirstTest();
    void CreateAndReleaseTests();
    void TwoPlayersSimple();
    void TwoPlayersWithDelay();
    void ThreePlayersWithDelay();
    void runTest();
    
private:
    
    ////////////////////////////////////////////////////////////////////////
    class Client : public ThreadPoolJob{
    public:
        Client() : ThreadPoolJob("Player"){
            myName = "";

        };
        void setName(String name){
            myName = name;
        }
        
        JobStatus runJob() override{
            
            struct sockaddr_in server_address;
            unsigned short server_port = 65002;
            
            String server_ip = "0.0.0.0";
            
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (sock == -1)
                perror("Can't open socket");
                
            /* Construct the server address structure */
            memset(&server_address, 0, sizeof(server_address));     /* Zero out structure */
            server_address.sin_family = AF_INET;             /* Internet address family */
            server_address.sin_addr.s_addr = inet_addr(server_ip.getCharPointer());   /* Server IP address */
            server_address.sin_port = htons(server_port); /* Server port: htons host to network byte order */
            
            int c = connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
            if (c < 0) {
                perror("Can't connect");
            }
            
            char receive_buffer[MAX_MESSAGE_SIZE];
            
            for (int i = 0; i < instructions.size(); i++) {
                NamedValueSet* instruction = instructions[i];
                
                String cmd = String(instruction->getName(0));
                
                if (cmd == "sleep")
                {
                    Logger::writeToLog(myName + " sleeping\n" );
                    
                    int time = instruction->getWithDefault("sleep", 1);
                    sleep(time);
                    Logger::writeToLog(myName + " awoken " );
                    
                }else if (cmd == "join")
                {
                    Logger::writeToLog(myName + " atempting to connect to game server\n" );
                    send_string(String("HELLO\n") );
                    
                    int sz = rec(receive_buffer);
                    if (sz < 1) {
                        /* zero indicates end of transmission */
                        break;
                    }
                    fprintf(stderr, "%s", receive_buffer);
                    if(strncasecmp(receive_buffer, "SIZE n\n", 4) == 0){
                        Logger::writeToLog(String::formatted(String("%s conneted to game, waiting for start\n"),myName.toRawUTF8(), receive_buffer ));
                        
                        int sz = rec(receive_buffer);
                        if (sz < 1) {
                            /* zero indicates end of transmission */
                            break;
                        }
                        if(strncasecmp(receive_buffer, "START\n", 4) == 0){
                            Logger::writeToLog(String::formatted(String("%s recieved response %s \n"),myName.toRawUTF8(), receive_buffer ));
                            Logger::writeToLog(myName + " starting " );
                        }
                        
                    }else if (strncasecmp(receive_buffer, "NACK\n", 4) == 0)
                    {
                        printf("could not connet to game\n");
                        break;
                    }
                    
                    
                }else if (cmd == "take")
                {
                    int x = instruction->getWithDefault("x", 1);
                    int y = instruction->getWithDefault("y", 1);
                    String name = instruction->getWithDefault("name", "");
                    send_string(String::formatted(String("TAKE %i %i %s\n"), x, y, name.toRawUTF8() ) );
                    
                    int sz = rec(receive_buffer);
                    if (sz < 1) {
                        /* zero indicates end of transmission */
                        break;
                    }
                    Logger::writeToLog(String::formatted(String("%s recieved response %s \n"),myName.toRawUTF8(), receive_buffer ));
                    
                }else if (cmd == "status")
                {
                    int x = instruction->getWithDefault("x", 1);
                    int y = instruction->getWithDefault("y", 1);
                    
                    send_string(String::formatted(String("STATUS %i %i\n"), x, y ) );
                    int sz = rec(receive_buffer);
                    if (sz < 1) {
                        /* zero indicates end of transmission */
                        break;
                    }
                    Logger::writeToLog(String::formatted(String("%s recieved response %s \n"),myName.toRawUTF8(), receive_buffer ));

                }
            }
            close(sock);
            Logger::writeToLog(myName + " finished all tasks." );
            Logger::writeToLog(myName + " shutting down\n" );
            return jobHasFinished;
        }
        
        int send_string(String msg) {
            int result = send(sock, msg.toRawUTF8(), strlen(msg.toRawUTF8()), 0);
            if (result == -1)
                fprintf(stderr, "%s: %s\n", "Error sending message", strerror(errno));
            return result;
        }
        
        int rec(char* buffer)
        {
            int recv_msg_size = recv(sock, buffer, MAX_MESSAGE_SIZE, 0);
            if (recv_msg_size == 0) {
                return 0;
            }
            buffer[recv_msg_size] = '\0';
            if(strncasecmp(buffer, "END", 3) == 0)
            {
                fprintf(stderr, "%s", buffer);
                return -1;
            }
            return 1;
        }
        
        OwnedArray<NamedValueSet> instructions;
    private:
        String myName;
        int sock;
        
    };
    
    ThreadPool pool;
    NamedValueSet* add_instruction(Client *client, const Identifier &inst, const var &parameter);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsolidatedTests)
};





#endif  // CONSOLIDATED_TESTS_H_INCLUDED
