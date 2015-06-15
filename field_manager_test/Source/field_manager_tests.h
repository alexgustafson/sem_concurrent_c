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
    void concurrentJoin();
    void concurrentPlaying();
    void runTest();

private:
    
    ////////////////////////////////////////////////////////////////////////
    class Player : public ThreadPoolJob{
    public:
        Player() : ThreadPoolJob("Player"){ myname = ""; };
        void setName(String name){
            myname = name;
        }
        JobStatus runJob() override{
            for (int i = 0; i < instructions.size(); i++) {
                NamedValueSet* instruction = instructions[i];
                
                String cmd = String(instruction->getName(0));

                if(cmd == "join"){
                    
                    Logger::writeToLog(myname + " is joining");
                    if (join_game() != 0){
                        Logger::writeToLog(myname + " could not join game");
                        return jobHasFinished;
                    }
                    Logger::writeToLog(myname + " joined");
                    
                }else if (cmd == "take")
                {
                    int x = instruction->getWithDefault("x", "");
                    int y = instruction->getWithDefault("y", "");
                    
                    Logger::writeToLog("taking x: " + String(x) + " y: " + String(y));
                    
                }else if (cmd == "sleep")
                {
                    
                }
            }
            return jobHasFinished;
        }
        OwnedArray<NamedValueSet> instructions;
    private:
        String myname;
    };
    
    ThreadPool pool;
    NamedValueSet* add_instruction(Player *player, const Identifier &inst, const var &parameter);

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FieldManagerTests)
};



#endif  // FIELD_MANAGER_TESTS_H_INCLUDED
