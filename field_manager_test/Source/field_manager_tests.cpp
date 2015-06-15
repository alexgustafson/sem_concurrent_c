/*
  ==============================================================================

    field_manager_tests.cpp
    Created: 10 Jun 2015 5:48:16pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#include "field_manager_tests.h"


void FieldManagerTests::FirstTest()
{
    beginTest("Testing The Tester");
    expect(true);
    expectEquals(true, !false);
}

void FieldManagerTests::CreateAndReleaseTests()
{
    beginTest("Create and Release Tests");
    initialize_field_manager();
    for (int i = 0; i < 50; i++) {
        set_size(i);
    }
    take_cell(0, 0, 212);
    for (int i = 50; i > 0; i--) {
        set_size(i);
    }
    expectEquals(get_cell_player(0, 0), 212);
    release_field_manager();
}

void FieldManagerTests::TestField()
{
    beginTest("Create Field");
    initialize_field_manager();

    set_size(1);
    take_cell(0, 0, 1);
    
    set_size(2);
    take_cell(1, 0, 2);
    take_cell(0, 1, 3);
    take_cell(1, 1, 4);

    set_size(3);
    
    take_cell(2, 0, 5);
    take_cell(0, 2, 6);
    take_cell(2, 1, 7);
    take_cell(1, 2, 8);
    take_cell(2, 2, 9);
    
    set_size(4);
    
    take_cell(3, 3, 16);
    
    expectEquals(get_cell_player(3, 3), 16);
    
    set_size(3);
    
    expectEquals(get_cell_player(0, 0), 1);
    expectEquals(get_cell_player(1, 0), 2);
    expectEquals(get_cell_player(0, 1), 3);
    expectEquals(get_cell_player(1, 1), 4);
    expectEquals(get_cell_player(1, 2), 8);
    expectEquals(get_cell_player(2, 2), 9);

    release_field_manager();
}

void FieldManagerTests::concurrentJoin()
{
    initialize_field_manager();
    
    ScopedPointer<Player> player1 = new Player();
    player1->setName("Player 1");
    add_instruction(player1, "join", 0);

    ScopedPointer<Player> player2 = new Player();
    player2->setName("Player 2");
    add_instruction(player2, "join", 0);
    
    ScopedPointer<Player> player3 = new Player();
    player3->setName("Player 3");
    add_instruction(player3, "join", 0);
    
    ScopedPointer<Player> player4 = new Player();
    player4->setName("Player 4");
    add_instruction(player4, "join", 0);
    
    //NamedValueSet* instruction = add_instruction(player3, "take", 0);
    //instruction->set("x", 2);
    //instruction->set("y", 1);
    
    pool.addJob(player1, false);
    pool.addJob(player2, false);
    pool.addJob(player3, false);
    pool.addJob(player4, false);

    while(pool.getNumJobs() > 0)
    {
        Thread::sleep(1);
    }
    
    expectEquals(get_size(), 16);
    
    release_field_manager();
}

void FieldManagerTests::concurrentPlaying()
{
    initialize_field_manager();
    
    ScopedPointer<Player> player1 = new Player();
    player1->setName("Player 1");
    add_instruction(player1, "join", 0);
    
    ScopedPointer<Player> player2 = new Player();
    player2->setName("Player 2");
    add_instruction(player2, "join", 0);
    
    ScopedPointer<Player> player3 = new Player();
    player3->setName("Player 3");
    add_instruction(player3, "join", 0);
    
    ScopedPointer<Player> player4 = new Player();
    player4->setName("Player 4");
    add_instruction(player4, "join", 0);
    
    release_field_manager();
}

NamedValueSet* FieldManagerTests::add_instruction(FieldManagerTests::Player *player, const Identifier &inst, const var &parameter)
{
    NamedValueSet* instruction = new NamedValueSet();
    instruction->set(inst, parameter);
    player->instructions.add(instruction);
    return instruction;
}


void FieldManagerTests::runTest()
{
    FirstTest();
    CreateAndReleaseTests();
    TestField();
    concurrentJoin();
    concurrentPlaying();
}
