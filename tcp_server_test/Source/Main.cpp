/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "tcp_server_tests.h"

//==============================================================================
int main (int argc, char* argv[])
{

    static TCPServerTests tcpServerTests;
    UnitTestRunner runner;
    runner.setAssertOnFailure(false);
    runner.runAllTests();

    return 0;
}
