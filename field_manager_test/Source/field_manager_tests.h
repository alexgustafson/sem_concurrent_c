/*
  ==============================================================================

    field_manager_tests.h
    Created: 10 Jun 2015 5:48:16pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#ifndef FIELD_MANAGER_TESTS_H_INCLUDED
#define FIELD_MANAGER_TESTS_H_INCLUDED

#include "JuceHeader.h"
#include "field_manager.h"


class FieldManagerTests : public UnitTest
{
public:
    FieldManagerTests() : UnitTest("") {}
    
    void FirstTest();
    void CreateAndReleaseTests();
    void TestField();
    
    void runTest();
    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FieldManagerTests)
};



#endif  // FIELD_MANAGER_TESTS_H_INCLUDED
