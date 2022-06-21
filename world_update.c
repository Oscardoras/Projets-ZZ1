#include <stdlib.h>

#include "world_update.h"


void update_world_cell(World* world, int x, int y, bool *table, Rulestring rule) {
    World world_tmp = *world;
    world_tmp.table = table;
    
    int neighbours = 0;
    for (int i = y-1; i <= y+1; i++)
        for (int j = x-1; j <= x+1; j++) {
            if (i != y && j != x) {
                bool* cell_i_j = get_world_cell(world, j, i);
                if (cell_i_j != NULL && *cell_i_j)
                    neighbours++;
            }
    
    bool cell = *get_world_cell(world, j, i);
    bool alive = false;
    char state;
    for (char* c = rule; *c != '\0'; c++) {
        if (*c == 'B') state = 'B';
        else if (*c == 'S') state = 'S';
        else if ('0' <= *c && *c <= '9') {
            int v = *c-48;
            if (v == neighbours) {
                if ((!*cell && state == 'B') || (*cell && state == 'S'))
                    alive = true;
            }
                
        }
    }
    
    *get_world_cell(world_tmp, j, i) = alive;
}

void update_world(World* world, Rulestring rule) {
    bool *newtable = malloc(sizeof(bool) * world->width * world->height);
    int i, j;
    for(i=0; i<world->width; i++) {
        for(j=0; j<world->height; j++) {
            update_world_cell(world, i, j, newtable, rule);
        }
    }
    world->table = newtable;
    
    
}
