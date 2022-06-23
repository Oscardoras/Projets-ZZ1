#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "level.h"


typedef enum {
    PHEROMONE_FOOD,
    PHEROMONE_ENNEMI,
    PHEROMONE_DIG,
    PHEROMONE_FILL
} PheromoneType;


/**
 * @brief Makes one iteration of the game loop.
 * 
 * @param level the level to update.
 */
void game_loop_iteration(Level* level);

/**
 * @brief Sets the global behavior of the ants.
 * 
 * @param level the level to change the global behaviour.
 * @param behavior the global behavior.
 */
void set_global_behavior(Level* level, GlobalBehavior behavior);

/**
 * @brief Adds an target for the ants.
 * 
 * @param level the level.
 * @param x the x coordinates of the target.
 * @param y the y coordinates of the target.
 * @param type the type of pheromone to add.
 */
void add_pheromone(Level* level, int x, int y, PheromoneType type);

/**
 * @brief Adds an ant.
 */
void new_ant(Level* level);

/**
 * @brief Determines if a position is valid to walk on.
 * 
 * @param level the level.
 * @param x
 * @param y
 * @return if it is possible to walk on this position.
 */
bool is_valid_position(Level* level, int x, int y);


#endif