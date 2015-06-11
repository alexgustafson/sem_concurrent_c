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

class TCPServerTests : public UnitTest
{
public:
    TCPServerTests() : UnitTest("") {}
    
    void FirstTest()
    {

        beginTest("Testing The Tester");
        expect(true);
        expectEquals(true, !false);
        startserver();
    }
    
    void runTest()
    {
        FirstTest();
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TCPServerTests)
};



#endif  // TCP_SERVER_TESTS_H_INCLUDED
