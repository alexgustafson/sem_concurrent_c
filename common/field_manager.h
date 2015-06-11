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
    
    int initialize_field_manager();
    void release_field_manager();

    void set_size(int n);
    int get_size();
    struct cell* get_cell(int x, int y);
    int get_cell_player(int x, int y);
    void take_cell(int x, int y, int player_id);


#ifdef __cplusplus
}
#endif
#endif  // FIELD_MANAGER_H_INCLUDED
