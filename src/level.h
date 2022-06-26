#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "entity.h"


typedef enum {
    WEATHER_SUN,
    WEATHER_RAIN,
    WEATHER_WARM,
    WEATHER_COLD
} Weather;

typedef enum {
    BEHAVIOR_DEFAULT,
    BEHAVIOR_AGGRESSIVE,
    BEHAVIOR_DEFESIVE,
    BEHAVIOR_NOT_GOING_OUT,
    BEHAVIOR_GOING_OUT,
    BEHAVIOR_TARGET_FOOD,
    BEHAVIOR_TARGET_BUILD
} GlobalBehavior;

typedef enum {
    BLOCK_AIR,
    BLOCK_DIRT,
    BLOCK_PATH,
    BLOCK_GRASS,
    BLOCK_ROCK,
    BLOCK_LEAVES
} Block;
#define BLOCK_TYPES 6

/**
 * @brief Represents a level.
 * 
 */
typedef struct {
    struct {
        int min_x; //included
        int max_x; //excluded
        int min_y; //included
        int max_y; //excluded
    } d; //The dimensions of the level.
    int seed;
    struct {
        unsigned int score;
        Weather weather;
        GlobalBehavior behavior;
        int spawn_speed;
        unsigned int food;
    } states;
    Block *blocks;
    struct EntityListCell {
        Entity* entity;
        struct EntityListCell* next;
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
 * @brief Saves a level in a save file.
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
 * @param level the level to resize.
 * @param min_x
 * @param max_x
 * @param min_y
 * @param max_y
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
 * @brief Adds an entity into a level.
 * 
 * @param level the level.
 * @param entity the entity to add.
 * @return if the entity has been added correctly.
 */
bool add_level_entity(Level* level, Entity* entity);

/**
 * @brief Removes deleted entities from a level.
 * 
 * @param level the level.
 */
void clean_level_entities(Level* level);

/**
 * @brief Genrerates procedurally a part of a level.
 * 
 * @param level the level to generate.
 * @param min_x 
 * @param max_x 
 * @param min_y
 * @param max_y
 * @param seed the seed used for generation.
 */
void generate_level(Level* level, int min_x, int max_x, int min_y, int max_y, int seed);


#endif
