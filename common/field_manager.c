#include "field_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifdef __APPLE__
#include "apple_pthread_barrier.h"
#endif

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
int min_players;
int player_count = 0;
int delay = 0;
int join_countdown = 10000;
struct field* field;

void request_global_lock();
void release_global_lock();

pthread_cond_t join_ready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t join_lock = PTHREAD_MUTEX_INITIALIZER;

int initialize_field_manager(int size)
{
    join_countdown = size/2;
    
    field = malloc(sizeof(struct field));
    field->cells = NULL;
    if (pthread_rwlock_init(&field->field_lock, NULL) != 0)
    {
        perror("could not initialize field lock");
        return NULL;
    }

    dim = 0;
    delay = 0;
    set_size(size);
    
    player_count = 0;
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
    delay = 0;
    join_countdown = 10000;

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

int request_cell_lock(int x, int y)
{
    if (get_cell(x, y) == NULL) {
        return -1;
    }
    return pthread_mutex_trylock(&get_cell(x, y)->cell_lock);
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

void _set_size(int n)
{
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
    min_players = dim / 2;
}

void set_size(int n)
{
    request_global_lock();
    
    _set_size(n);
    
    release_global_lock();
}

void increment_size()
{
    request_global_lock();
    
    _set_size((dim + 1));
    
    release_global_lock();
}

void decrement_size()
{
    request_global_lock();
    
    _set_size((dim - 1));
    
    release_global_lock();
}



int get_size()
{
    return dim * dim;
}

int get_dim()
{
    return dim;
}

int coords_to_index(int x, int y)
{
    if (x > dim || y > dim || x < 0 || y < 0){
        return - 2; //out of bounds
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
    if (coords_to_index(x, y) < 0) {
        return NULL;
    }
    return &field->cells[coords_to_index(x, y)];
}

int take_cell(int x, int y, int player_id)
{
    int respsonse;
    //request_global_read();
    int result = request_cell_lock(x, y);
    if (result == 0)
    {
        get_cell(x, y)->player_id = player_id;
        sleep(delay);
        release_cell_lock(x, y);
        respsonse = 1;
    }else{
        respsonse= result;
    }

    //release_global_read();
    return respsonse;
}

int get_cell_player(int x, int y)
{
    return get_cell(x, y)->player_id;
}

int join_game()
{
    pthread_mutex_lock(&join_lock);
    if (--join_countdown > 0)
        pthread_cond_wait(&join_ready, &join_lock);
    pthread_mutex_unlock(&join_lock);
    pthread_cond_signal(&join_ready);
    
    return 0;
}

int leave_game()
{

    player_count--;
    if (player_count < 1) {
        release_field_manager();
    }
    return 0;
}

void set_delay(int n)
{
    delay = n;
}

int is_there_a_winner()
{
    request_global_lock();
    if(dim < 2)
    {
        release_global_lock();
        return -1;
    }
    
    struct cell cell = field->cells[0];
    int winning_player = cell.player_id;
    for (int i = 1; i < (dim * dim); i++) {
        
        struct cell next_cell = field->cells[i];
        if (next_cell.player_id == winning_player) {
            
        }else{
            release_global_lock();
            return -1;
        }
    }
    
    release_global_lock();
    return winning_player;
}






