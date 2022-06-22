#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdio.h>
#include <stdbool.h>

#include "character.h"


typedef enum {
    Air,
    Dirt,
    Path,
    Grass,
    Rock,
    Fruit,
    Leaves
} Block;

typedef struct {
    int seed;
    int min_x; //included
    int max_x; //excluded
    int min_y; //included
    int max_y; //excluded
    Block *blocks;
    struct ListCell {
        Entity* entity;
        struct ListCell* next;
    }* entities;
} Level;


/**
 * @brief Creates a new level from the seed.
 * 
 * @param min_x
 * @param max_x
 * @param min_y
 * @param max_y
 * @param seed a seed for generation.
 * @return the created level.
 */
Level* new_level(int min_x, int max_x, int min_y, int max_y, int seed);

/**
 * @brief Frees a level.
 * 
 * @param level the level to free.
 */
void free_level(Level* level);

/**
 * @brief Saves a level into a file.
 * 
 * @param level the level to save.
 * @param file the save file.
 */
void save_level(Level* level, FILE* file);

/**
 * @brief Loads a level from a file.
 * 
 * @param file the save file.
 * @return the loaded level.
 */
Level* load_level(FILE* file);

/**
 * @brief Resizes a level.
 * 
 * @param min_x
 * @param max_x
 * @param min_y
 * @param max_y
 * @param height the new height of the level.
 */
bool resize_level(Level* level, int min_x, int max_x, int min_y, int max_y);

/**
 * @brief Gets a pointer to a block in a level.
 * 
 * @param level the level.
 * @param x the x coordinates.
 * @param y the y coordinates.
 * @return a pointer to the block.
 */
Block* get_level_block(Level* level, int x, int y);

/**
 * @brief Adds an entity in a level.
 * 
 * @param level a level.
 * @param entity an entity.
 * @return if the entity has correctly been added.
 */
bool add_level_entity(Level* level, Entity* entity);

/**
 * @brief Remove deleted entities from a level.
 * 
 * @param level the level.
 */
void clean_level_entities(Level* level);


#endif
