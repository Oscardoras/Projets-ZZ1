#include <stdlib.h>

#include "world.h"


World new_world(bool borders, int width, int height) {
    World world;
    
    world.borders = borders;
    world.width = width;
    world.height = height;
    
    world.table = malloc(sizeof(bool) * width*height);
    if (world.table != NULL) {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                world.table[i*width+j] = false;
    } else {
        world.borders = false;
        world.width = 0;
        world.height = 0;
    }
    
    return world;
}

/*
World load_world(FILE* file) {
    World new_world;
    bool borders;
    int width, height;
    
}
*/

void free_world(World* world) {
    if (world->table != NULL) {
        world->borders = false;
        world->width = 0;
        world->height = 0;
        free(world->table);
        world->table = NULL;
    }
}

void set_world_size(World* world, int width, int height) {
    bool *table = malloc(sizeof(bool) * width*height);
    if (world->table != NULL) {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                if (i < world->height && j < world->width)
                    table[i*width+j] = world->table[i*world->width+j];
                else
                    table[i*width+j] = false;
        
        world->width = width;
        world->height = height;
        if (world->table != NULL)
            free(world->table);
        world->table = table;
    } else {
        world->borders = false;
        world->width = 0;
        world->height = 0;
        if (world->table != NULL) {
            free(world->table);
            world->table = NULL;
        }
    }
}

bool* get_world_cell(World* world, int x, int y) {
    if (world->width > 0 && world->height > 0 && world->table != NULL) {
        if (world->borders) {
            if (0 <= x && x < world->width && 0 <= y && y < world->height) {
                return &world->table[y*world->width + x];
            } else return NULL;
        } else {
            x = x % world->width;
            if (x < 0) x += world->width;
            y = y % world->height;
            if (y < 0) x += world->height;
            
            return &world->table[y*world->width + x];
        }
    } else return NULL;
}