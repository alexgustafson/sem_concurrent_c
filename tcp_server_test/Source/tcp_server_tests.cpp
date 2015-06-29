/*
  ==============================================================================

    tcp_server_tests.cpp
    Created: 8 Jun 2015 5:26:04pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#include "tcp_server_tests.h"

//test callbacks

int always_allow_join()
{
    return 0;
}


int forbid_join()
{
    return -1;
}

int always_allow_leave()
{
    return 0;
}

int taken = 0;
int always_allow_take(int x, int y, int player_id)
{
    taken = 1;
    return 1;
}

int second = 0;
int every_second_take(int x, int y, int player_id)
{
    if(second)
    {
        taken = 1;
        second = 0;
        return 1;
    }else{
        second = 1;
        return 0;
    }
}

int return_4()
{
    return 4;
}

int always_status_1(int x, int y)
{
    return 1;
}

int never_winner()
{
    return -1;
}

int always_winner_1()
{
    return 1;
}

int win_after_take()
{
    if (taken == 1)
        return 1;
    else
        return -1;
}


void TCPServerTests::FirstTest()
{
    beginTest("Testing The Tester");
    expect(true);
    expectEquals(true, !false);
}

void TCPServerTests::ServerTest()
{
    beginTest("Test Server");
    register_callbacks(always_allow_join,
                       always_allow_leave,
                       always_allow_take,
                       return_4,
                       always_status_1,
                       never_winner);
    startserver(65002);
    Thread::sleep(5);
    stopserver();
}

void TCPServerTests::ConnectionTest()
{
    beginTest("Connection Test");
    
    register_callbacks(always_allow_join,
                       always_allow_leave,
                       always_allow_take,
                       return_4,
                       always_status_1,
                       never_winner);

    startserver(65002);
    Thread::sleep(20);

    ScopedPointer<Client> client1 = new Client();
    client1->setName("Frank");
    add_instruction(client1, "sleep", 2);
    add_instruction(client1, "join", "");
    add_instruction(client1, "sleep", 2);
    
    NamedValueSet* instruction = add_instruction(client1, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(client1, "status", "");
    instruction->set("x", 0);
    instruction->set("y", 1);

    add_instruction(client1, "sleep", 2);
    
    pool.addJob(client1, false);

    while(pool.getNumJobs() > 0 )
    {
        Thread::sleep(5);
    }
    
    stopserver();
    while( server_running() > 0)
    {
        Thread::sleep(10);
    }
}

void TCPServerTests::ThreePlayerTest()
{
    beginTest("Three Player Test");
    
    register_callbacks(always_allow_join,
                       always_allow_leave,
                       always_allow_take,
                       return_4,
                       always_status_1,
                       never_winner);
    startserver(65002);
    
    ScopedPointer<Client> frank = new Client();
    frank->setName("Frank");
    add_instruction(frank, "sleep", 2);
    add_instruction(frank, "join", "");
    add_instruction(frank, "sleep", 2);
    
    NamedValueSet* instruction = add_instruction(frank, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "status", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    
    add_instruction(frank, "sleep", 2);
    
    ScopedPointer<Client> susy = new Client();
    susy->setName("Susy");
    add_instruction(susy, "sleep", 5);
    add_instruction(susy, "join", "");
    add_instruction(susy, "sleep", 2);
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 0);
    instruction->set("name", "Susy");
    
    
    ScopedPointer<Client> joe = new Client();
    susy->setName("Joe");
    add_instruction(joe, "sleep", 6);
    add_instruction(joe, "join", "");
    add_instruction(joe, "sleep", 2);
    
    instruction = add_instruction(joe, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 1);
    instruction->set("name", "Joe");
    
    
    pool.addJob(frank, false);
    pool.addJob(susy, false);
    pool.addJob(joe, false);
    
    while(pool.getNumJobs() > 0 )
    {
        Thread::sleep(5);
    }
    
    stopserver();
    while( server_running() > 0)
    {
        Thread::sleep(10);
    }
}

void TCPServerTests::runTest()
{
    FirstTest();
    ServerTest();
    Thread::sleep(100);
    ConnectionTest();
    Thread::sleep(100);
    ThreePlayerTest();
}

NamedValueSet* TCPServerTests::add_instruction(TCPServerTests::Client *client, const Identifier &inst, const var &parameter)
{
    NamedValueSet* instruction = new NamedValueSet();
    instruction->set(inst, parameter);
    client->instructions.add(instruction);
    return instruction;
}