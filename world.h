#ifndef __WORLD_H__
#define __WORLD_H__

#include <stdbool.h>


typedef struct {
    bool borders;
    int width;
    int height;
    bool *table;
} World;

/**
 * @brief Creates a new world.
 * 
 * @param borders if the world has borders.
 * @param width the width of the world.
 * @param height the height of the world.
 * @return the created world.
 */
World new_world(bool borders, int width, int height);

/**
 * @brief free world's memory.
 * 
 * @param world a pointer to the world.
 */
void free_world(World* world);

/**
 * @brief Change world's size.
 * 
 * @param world a pointer to the world.
 * @param width the new world's width.
 * @param height the new world's height.
 */
void set_world_size(World* world, int width, int height);

bool* get_world_cell(World* world, int x, int y);


#endif