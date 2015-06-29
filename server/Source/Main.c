/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

//#include "../JuceLibraryCode/JuceHeader.h"
#include "field_manager.h"
#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>

//==============================================================================
int main (int argc, char* argv[])
{
    int port = 65002, size = 4 ;
    if (argc == 3) {
        port = atoi(argv[1]);
        size = atoi(argv[2]);
    }
    
    initialize_field_manager(4);

    register_callbacks(join_game,
                       leave_game,
                       take_cell,
                       get_size,
                       get_cell_player,
                       is_there_a_winner);

    startserver(port);
        


    getchar();
    stopserver();
    release_field_manager();
    
    return 0;
}
