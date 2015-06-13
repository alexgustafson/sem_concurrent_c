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

void FieldManagerTests::concurrentTest1()
{
    
}

void FieldManagerTests::runTest()
{
    FirstTest();
    CreateAndReleaseTests();
    TestField();
}