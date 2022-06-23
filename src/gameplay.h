#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "level.h"


typedef enum {
    MOVE,
    LAY
} QueenAction;

typedef enum {
    FOOD,
    ENNEMI,
    DIG,
    FILL
} Target;


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
 * @param target the target to add.
 */
void add_target(Level* level, int x, int y, Target target);

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