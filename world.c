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

World load_world(FILE* file) {
    World world;
    
    {
        char header[64];
        fgets(header, 64, file);
        
        char value[64];
        int k = 0;
        
        int a = 0;
        for (; header[k] != ' '; k++) {
            value[a] = header[k];
            a++;
        }
        value[a] = '\0';
        world.borders = atoi(value);
        k++;
        
        int b = 0;
        for (; header[k] != ' '; k++) {
            value[b] = header[k];
            b++;
        }
        value[b] = '\0';
        world.width = atoi(value);
        k++;
        
        int c = 0;
        for (; header[k] != '\n'; k++) {
            value[c] = header[k];
            c++;
        }
        value[c] = '\0';
        world.height = atoi(value);
    }
    
    world.table = malloc(sizeof(bool) * world.width*world.height);
    char line[16];
    int k = 0;
    while (fgets(line, 16, file)) {
        char* c;
        for (c = line; !(*c == '\n' || *c == '\0'); c++);
        *c = '\0';
        world.table[k] = atoi(line);
        k++;
    }
    
    return world;
}

void save_world(World* world, FILE* file) {
    fprintf(file, "%d %d %d\n", world->borders, world->width, world->height);
    
    for (int k = 0; k < world->width * world->height; k++)
        fprintf(file, "%d\n", world->table[k]);
}

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
            if (y < 0) y += world->height;
            
            return &world->table[y*world->width + x];
        }
    } else return NULL;
}