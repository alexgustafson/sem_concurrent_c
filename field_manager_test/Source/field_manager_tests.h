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
#include <unistd.h>


class FieldManagerTests : public UnitTest
{
public:
    FieldManagerTests() : UnitTest("") { pool = new ThreadPool(10);}
    
    void FirstTest();
    void CreateAndReleaseTests();
    void TestField();
    void concurrentJoin();
    void atLeast8PlayersMustJoin();
    void concurrentPlaying();
    void concurrentPlayingAndLeaving();
    void runTest();

private:
    
    ////////////////////////////////////////////////////////////////////////
    class Player : public ThreadPoolJob{
    public:
        Player() : ThreadPoolJob("Player"){ myName = ""; };
        void setName(String name){
            myName = name;
        }
        void setId(int pId)
        {
            playerId = pId;
        }
        
        JobStatus runJob() override{
            for (int i = 0; i < instructions.size(); i++) {
                NamedValueSet* instruction = instructions[i];
                
                String cmd = String(instruction->getName(0));

                if(cmd == "join"){
                    
                    Logger::writeToLog(myName + " is joining");
                    if (join_game() != 0){
                        Logger::writeToLog(myName + " could not join game");
                        return jobHasFinished;
                    }
                    Logger::writeToLog(myName + " joined");
                    
                }else if (cmd == "take")
                {
                    int x = instruction->getWithDefault("x", "");
                    int y = instruction->getWithDefault("y", "");
                    Logger::writeToLog(myName + " trying to take x: " + String(x) + " y: " + String(y));

                    if (take_cell(x, y, playerId) == 1){
                        Logger::writeToLog(myName + " taken x: " + String(x) + " y: " + String(y));
                    }else{
                        Logger::writeToLog(myName + " could not take x: " + String(x) + " y: " + String(y));
                    }
                    
                }else if (cmd == "sleep")
                {
                    Logger::writeToLog(myName + " sleeping " );
                    
                    int time = instruction->getWithDefault("sleep", 1);
                    sleep(time);
                    
                    Logger::writeToLog(myName + " awoken " );
                    
                }
                else if (cmd == "leave")
                {
                    Logger::writeToLog(myName + " leaving game " );
                    if (leave_game() != 0){
                        Logger::writeToLog(myName + " could not leave game");
                        return jobHasFinished;
                    }
                
                    Logger::writeToLog(myName + " left game " );
                
                }
            }
            Logger::writeToLog(myName + " finished all tasks." );

            return jobHasFinished;
        }
        OwnedArray<NamedValueSet> instructions;
    private:
        String myName;
        int playerId;
    };
    
    //ThreadPool pool;
    ScopedPointer<ThreadPool> pool;
    NamedValueSet* add_instruction(Player *player, const Identifier &inst, const var &parameter);

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FieldManagerTests)
};



#endif  // FIELD_MANAGER_TESTS_H_INCLUDED
