#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "level.h"


typedef enum {
    MOVE,
    LAY
} QueenAction;

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
 * @brief Determines if a position is valid to walk on.
 * 
 * @param level the level.
 * @param x
 * @param y
 * @return if it is possible to walk on this position.
 */
bool is_valid_position(Level* level, int x, int y);


/**
 * @brief Makes the queen give birth to a new ant.
 * 
 * @param queen the ant queen.
 * @param level the level it's in.
 */
void lay_egg_queen(Entity* queen, Level* level);

/**
 * @brief Makes the queen move or lay depending of "action".
 * 
 * @param queen the ant queen.
 * @param level the level it's in.
 * @param action the action wanted.
 */
void update_queen(Entity* queen, Level* level, QueenAction action);


#endif