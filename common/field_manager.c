#include "field_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct cell {
    int player_id;
    pthread_mutex_t cell_lock;
};

struct field {
    pthread_rwlock_t field_lock;
    struct cell* cells;
};

int dim = 0;

struct field* field;

void request_global_lock();
void release_global_lock();

int initialize_field_manager()
{
    field = malloc(sizeof(struct field));
    field->cells = NULL;
    if (pthread_rwlock_init(&field->field_lock, NULL) != 0)
    {
        perror("could not initialize field lock");
        return NULL;
    }
    dim = 0;
    return 1;
}

void release_field_manager()
{
    request_global_lock();
    for (int i = 0; i < (dim*dim); i++)
    {
        struct cell tmpcell = field->cells[i];
        pthread_mutex_destroy(&tmpcell.cell_lock);
    }
    free(field->cells);
    free(field);
    release_global_lock();
    pthread_rwlock_destroy(&field->field_lock);
    dim = 0;
}

void request_global_lock()
{
    pthread_rwlock_wrlock(&field->field_lock);
}

void release_global_lock()
{
    pthread_rwlock_unlock(&field->field_lock);
}

void request_global_read()
{
    pthread_rwlock_rdlock(&field->field_lock);
}

void release_global_read()
{
    pthread_rwlock_unlock(&field->field_lock);
}

void request_cell_lock(int x, int y)
{
    pthread_mutex_lock(&get_cell(x, y)->cell_lock);
}

void release_cell_lock(int x, int y)
{
    pthread_mutex_unlock(&get_cell(x, y)->cell_lock);
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
    request_global_lock();

    int new_count = n * n;
    int current_count = dim * dim;
    int diff = new_count - current_count;
    
    if (diff > 0) // increase field size
    {
        struct cell* new_cells = create_new_cells(diff);
        if(dim > 0){
            field->cells = realloc(field->cells, sizeof(struct cell) * new_count);
            memcpy(&field->cells[current_count], new_cells, sizeof(struct cell) * diff);
            free(new_cells);
        }else
        {
            field->cells = new_cells;
        }
        new_cells = NULL;
    }
    else if (diff < 0) // remove cells
    {
        
        for (int i = 0; i < diff; i++)
        {
            realloc(field->cells, sizeof(struct cell) * new_count);
        }
    }
    dim = n;
    
    release_global_lock();
}

int get_size()
{
    return dim;
}

int coords_to_index(int x, int y)
{
    if ((x > dim) || (y > dim)) {
        perror("out of bounds");
    }
    
    int max = MAX(x, y) + 1;
    int min = MIN(x, y) + 1;
    int diff = max - min;
    
    int cell_index = (max * max) - (2*diff);
    if (y > x)
        cell_index++;
    
    return cell_index - 1;
}

struct cell* get_cell(int x, int y)
{
    return &field->cells[coords_to_index(x, y)];
}

void take_cell(int x, int y, int player_id)
{
    request_global_read();
    request_cell_lock(x, y);
    get_cell(x, y)->player_id = player_id;
    release_cell_lock(x, y);
    release_global_read();
}

int get_cell_player(int x, int y)
{
    return get_cell(x, y)->player_id;
}









