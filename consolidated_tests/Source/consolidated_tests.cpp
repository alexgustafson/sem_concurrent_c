/*
  ==============================================================================

    consolidated_tests.cpp
    Created: 23 Jun 2015 3:27:55pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#include "consolidated_tests.h"


void ConsolidatedTests::FirstTest()
{
    beginTest("Testing The Tester");
    expect(true);
    expectEquals(true, !false);
}
void ConsolidatedTests::CreateAndReleaseTests()
{
    beginTest("Create and Release Tests");
    initialize_field_manager();
    startserver(65002);
    
    register_callbacks(join_game,
                       leave_game,
                       take_cell,
                       get_size,
                       get_cell_player,
                       is_there_a_winner);
    
    Thread::sleep(5);
    stopserver();
    release_field_manager();
    Thread::sleep(50);
}

void ConsolidatedTests::TwoPlayersSimple()
{
    beginTest("Create and Release Tests");
    initialize_field_manager();
    set_delay(0);
    startserver(65002);
    
    register_callbacks(join_game,
                       leave_game,
                       take_cell,
                       get_size,
                       get_cell_player,
                       is_there_a_winner);
    
    
    ScopedPointer<Client> frank = new Client();
    frank->setName("Frank");
    add_instruction(frank, "sleep", 10);
    add_instruction(frank, "join", "");
    
    NamedValueSet* instruction = add_instruction(frank, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "status", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    
    add_instruction(frank, "sleep", 3);
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "status", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    
    ScopedPointer<Client> susy = new Client();
    susy->setName("Susy");
    add_instruction(susy, "sleep", 5);
    add_instruction(susy, "join", "");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    instruction->set("name", "Susy");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 0);
    instruction->set("name", "Susy");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 0);
    instruction->set("name", "Susy");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 1);
    instruction->set("name", "Susy");
    
    pool.addJob(frank, false);
    pool.addJob(susy, false);
    
    while(pool.getNumJobs() > 0 )
    {
        Thread::sleep(100);
    }
    
    stopserver();
    while( server_running() > 0)
    {
        Thread::sleep(100);
    }
    release_field_manager();
    Thread::sleep(1000);
    
}


void ConsolidatedTests::TwoPlayersWithDelay()
{
    beginTest("Create and Release Tests");
    initialize_field_manager();
    startserver(65002);
    set_delay(1);
    register_callbacks(join_game,
                       leave_game,
                       take_cell,
                       get_size,
                       get_cell_player,
                       is_there_a_winner);
    
    
    ScopedPointer<Client> frank = new Client();
    frank->setName("Frank");
    add_instruction(frank, "sleep", 10);
    add_instruction(frank, "join", "");
    
    NamedValueSet* instruction = add_instruction(frank, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "status", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    
    add_instruction(frank, "sleep", 3);
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 0);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 1);
    instruction->set("name", "Frank");
    
    instruction = add_instruction(frank, "status", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    
    ScopedPointer<Client> susy = new Client();
    susy->setName("Susy");
    add_instruction(susy, "sleep", 5);
    add_instruction(susy, "join", "");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 1);
    instruction->set("name", "Susy");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 0);
    instruction->set("y", 0);
    instruction->set("name", "Susy");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 0);
    instruction->set("name", "Susy");
    
    instruction = add_instruction(susy, "take", "");
    instruction->set("x", 1);
    instruction->set("y", 1);
    instruction->set("name", "Susy");
    
    add_instruction(susy, "sleep", 20);
    
    pool.addJob(frank, false);
    pool.addJob(susy, false);
    
    while(pool.getNumJobs() > 0 )
    {
        Thread::sleep(100);
    }
    
    stopserver();
    while( server_running() > 0)
    {
        Thread::sleep(100);
    }
    release_field_manager();
    Thread::sleep(1000);
    
}



void ConsolidatedTests::runTest()
{
    FirstTest();
    //CreateAndReleaseTests();
    //TwoPlayersSimple();
    TwoPlayersWithDelay();

}

NamedValueSet* ConsolidatedTests::add_instruction(ConsolidatedTests::Client *client, const Identifier &inst, const var &parameter)
{
    NamedValueSet* instruction = new NamedValueSet();
    instruction->set(inst, parameter);
    client->instructions.add(instruction);
    return instruction;
}