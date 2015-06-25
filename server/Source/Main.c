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

//==============================================================================
int main (int argc, char* argv[])
{

    
        initialize_field_manager(4);
        startserver(65002);
        
        register_callbacks(join_game,
                           leave_game,
                           take_cell,
                           get_size,
                           get_cell_player,
                           is_there_a_winner);
        

        getchar();
        stopserver();
        release_field_manager();
    
    return 0;

    return 0;
}
