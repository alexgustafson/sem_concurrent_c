#include "field_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int dim = 0;
struct cell* field;

pthread_rwlock_t* global_lock;

int initialize_field_manager()
{
    if(pthread_rwlock_init(global_lock, NULL) != 0)
    {
        perror("could not initialize global lock");
        return -1;
    }
    return 1;
}

int request_global_lock()
{
        
}

void free_cell(struct cell *cell) {
    //critical section
    pthread_mutex_destroy(&cell->cell_lock);
    free(cell);
}

struct cell * create_new_cells(int n)
{
    struct cell *new_cells = malloc(sizeof(struct cell) * n);
    if (new_cells == NULL)
        perror("out of memory");
    
    for (int i = 0; i < n ; i++) {
        new_cells[i].player_id = -1;
        if (pthread_mutex_init(&new_cells[i].cell_lock, NULL) != 0)
        {
            return (NULL);
        }
    }
    return new_cells;
}

void set_size(int n)
{
    //critical section
    int new_count = n * n;
    int current_count = dim * dim;
    int diff = new_count - current_count;
    
    if (diff > 0) // increase field size
    {
        struct cell* new_cells = create_new_cells(diff);
        if(dim > 0){
            memcpy(field + (sizeof(struct cell)*dim), new_cells, diff*sizeof(struct cell));
        }else
        {
            field = new_cells;
        }
    }
    else if (diff < 0) // remove cells
    {
        
    }
    dim = n;
    //critical section
}

int get_size()
{
    //critical section
    return dim;
    //critical section
}

struct cell* get_cell(int x, int y)
{
    //critical section
    if ((x > dim) || (y > dim)) {
        perror("out of bounds");
    }
    
    int max = MAX(x, y) + 1;
    int min = MIN(x, y) + 1;
    int diff = max - min;
    
    int cell_number = (max * max) - (2*diff);
    if (x > y)
        cell_number++;
    
    return &field[cell_number];
}

void take_cell(int x, int y, int player_id)
{
    get_cell(x, y)->player_id = player_id;
}







