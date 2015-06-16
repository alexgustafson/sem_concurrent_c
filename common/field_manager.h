/*
  ==============================================================================

    field_manager.h
    Created: 10 Jun 2015 5:44:42pm
    Author:  Alexander Gustafson

  ==============================================================================
*/

#ifndef FIELD_MANAGER_H_INCLUDED
#define FIELD_MANAGER_H_INCLUDED

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    #define CELL_IS_OCCUPIED = -1
    #define OUT_OF_BOUNDS = -2
    
    int initialize_field_manager();
    void release_field_manager();

    void set_size(int n);
    int get_size();
    struct cell* get_cell(int x, int y);
    int get_cell_player(int x, int y);
    int take_cell(int x, int y, int player_id);
    int join_game();
    int leave_game();
    void set_delay(int n);
    int is_there_a_winner();

#ifdef __cplusplus
}
#endif
#endif  // FIELD_MANAGER_H_INCLUDED
