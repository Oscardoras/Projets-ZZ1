#ifndef __WORLD_H__
#define __WORLD_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct {
    bool borders;
    int width;
    int height;
    bool *table;
} World;

typedef char Rulestring[32];


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
 * @brief Loads a text file into a world.
 * 
 * @param file the text file containing the saved world.
 * @return the loaded world.
 */
World load_world(FILE* file);

/**
 * @brief Saves the world in a text file.
 * 
 * @param world a pointer to the world to save.
 * @param file the text file containing the saved world.
 */
void save_world(World* world, FILE* file);

/**
 * @brief frees world's memory.
 * 
 * @param world a pointer to the world.
 */
void free_world(World* world);

/**
 * @brief Resizes a world.
 * 
 * @param world a pointer to the world.
 * @param width the new world's width.
 * @param height the new world's height.
 */
void resize_world(World* world, int width, int height);


/**
 * @brief Gets a cell.
 * 
 * @param world a pointer to the world.
 * @param x the x coordinates.
 * @param y the y coordinates.
 * @return a pointer to the cell. Returns NULL if the cell doesn't exist.
 */
bool* get_world_cell(World* world, int x, int y);

/**
 * @brief Updates the state of a cell.
 * 
 * @param world a pointer to the world.
 * @param x the x coordinates of the cell to evoluate.
 * @param y the y coordinates of the cell to evoluate.
 * @param world2 the new world updated.
 * @param rule a rule string.
 * @return the state of the cell.
 */
bool update_world_cell(World* world, int x, int y, World* world2, Rulestring rule);

/**
 * @brief Updates the states of all the cells of a world.
 * 
 * @param world a pointer to the world.
 * @param rule a rule string.
 * @return if the world has changed.
 */
bool update_world(World* world, Rulestring rule);


#endif
