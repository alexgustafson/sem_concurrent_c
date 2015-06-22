/*
  ==============================================================================

    tcp_server_tests.h
    Created: 8 Jun 2015 5:26:04pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#ifndef TCP_SERVER_TESTS_H_INCLUDED
#define TCP_SERVER_TESTS_H_INCLUDED

#include "JuceHeader.h"
#include "tcp_server.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>



class TCPServerTests : public UnitTest
{
public:
    TCPServerTests() : UnitTest("") {}
    
    void FirstTest();
    void ServerTest();
    void ConnectionTest();
    
    void runTest();
    
private:
    
    ////////////////////////////////////////////////////////////////////////
    class Client : public ThreadPoolJob{
    public:
        Client() : ThreadPoolJob("Player"){ myName = ""; };
        void setName(String name){
            myName = name;
        }
        void setId(int pId)
        {
            playerId = pId;
        }
        
        JobStatus runJob() override{
            struct sockaddr_in server_address;
            unsigned short server_port = 65002;
            char message[1024];
            unsigned int message_size;
            String server_ip = "127.0.0.1";
            
            int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
            
            
            for (int i = 0; i < instructions.size(); i++) {
                NamedValueSet* instruction = instructions[i];
                
                String cmd = String(instruction->getName(0));
                
                if (cmd == "sleep")
                {
                    Logger::writeToLog(myName + " sleeping " );
                    
                    int time = instruction->getWithDefault("sleep", 1);
                    sleep(time);
                    
                    Logger::writeToLog(myName + " awoken " );
                }else if (cmd == "say")
                {
                    Logger::writeToLog(myName + " sending message " );
                    
                    String message = instruction->getWithDefault("say", "");
                    char* new_cstring = (char *)message.toRawUTF8();

                    
                    int result = send(sock, new_cstring, strlen(new_cstring), 0);
                    if (result == -1)
                        fprintf(stderr, "%s: %s\n", "Error sending message", strerror(errno));

                    Logger::writeToLog(myName + " sent message " );
                }
            }
            return jobHasFinished;
        }
        OwnedArray<NamedValueSet> instructions;
    private:
        String myName;
        int playerId;
        
    };
    
    
    ThreadPool pool;
    NamedValueSet* add_instruction(Client *player, const Identifier &inst, const var &parameter);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TCPServerTests)
};



#endif  // TCP_SERVER_TESTS_H_INCLUDED
