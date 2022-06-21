#include <stdlib.h>

#include "world_update.h"


bool update_world_cell(World* world, int x, int y, World* world2, Rulestring rule) {
    int neighbours = 0;
    for (int i = y-1; i <= y+1; i++)
        for (int j = x-1; j <= x+1; j++)
            if (i != y || j != x) {
                bool* cell_i_j = get_world_cell(world, j, i);
                if (cell_i_j != NULL && *cell_i_j)
                    neighbours++;
            }
    
    bool cell = *get_world_cell(world, x, y);
    bool alive = false;
    char state;
    for (char* c = rule; *c != '\0'; c++) {
        if (*c == 'B') state = 'B';
        else if (*c == 'S') state = 'S';
        else if ('0' <= *c && *c <= '9') {
            int v = *c - '0';
            if (v == neighbours) {
                if ((!cell && state == 'B') || (cell && state == 'S'))
                    alive = true;
            }

        }
    }
    
    *get_world_cell(world2, x, y) = alive;
    return alive;
}

bool update_world(World* world, Rulestring rule) {
    bool modified = false;
    World world2 = new_world(world->borders, world->width, world->height);
    
    for (int x = 0; x < world->width; x++)
        for (int y = 0; y < world->height; y++) {
            bool state1 = *get_world_cell(world, x, y);
            update_world_cell(world, x, y, &world2, rule);
            bool state2 = *get_world_cell(&world2, x, y);
            if (state1 != state2)
                modified = true;
        }
    
    free(world->table);
    world->table = world2.table;
    
    return modified;
}
