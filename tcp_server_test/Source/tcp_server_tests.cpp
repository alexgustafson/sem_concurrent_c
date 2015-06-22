/*
  ==============================================================================

    tcp_server_tests.cpp
    Created: 8 Jun 2015 5:26:04pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#include "tcp_server_tests.h"


void TCPServerTests::FirstTest()
{
    beginTest("Testing The Tester");
    expect(true);
    expectEquals(true, !false);
}

void TCPServerTests::ServerTest()
{
    beginTest("Test Server");
    startserver();
    Thread::sleep(5);
    stopserver();
    while( server_running() > 0)
    {
        Thread::sleep(5);
    }

}

void TCPServerTests::ConnectionTest()
{
    beginTest("Connection Test");
    startserver();
    
    ScopedPointer<Client> client1 = new Client();
    client1->setName("Client 1");
    add_instruction(client1, "sleep", 2);
    add_instruction(client1, "say", "hello hello");
    add_instruction(client1, "sleep", 2);
    add_instruction(client1, "say", "hello hello");
    add_instruction(client1, "sleep", 2);
    
    
    pool.addJob(client1, false);

    while(pool.getNumJobs() > 0 )
    {
        Thread::sleep(5);
    }
    
    stopserver();
    while( server_running() > 0)
    {
        Thread::sleep(5);
    }
    Thread::sleep(5);
}


void TCPServerTests::runTest()
{
    FirstTest();
    //ServerTest();
    ConnectionTest();
}

NamedValueSet* TCPServerTests::add_instruction(TCPServerTests::Client *client, const Identifier &inst, const var &parameter)
{
    NamedValueSet* instruction = new NamedValueSet();
    instruction->set(inst, parameter);
    client->instructions.add(instruction);
    return instruction;
}