#ifndef __GAMEPLAY_H_
#define __GAMEPLAY_H_

#include "level.h"


typedef enum {
    FOOD,
    ENNEMI,
    DIG,
    FILL
} Target;


/**
 * @brief Sets the global behavior of the ants.
 * 
 * @param behavior the global behavior.
 */
void set_global_behavior(GlobalBehavior behavior);

/**
 * @brief Adds an target for the ants.
 * 
 * @param level the level.
 * @param x the x coordinates of the target.
 * @param y the y coordinates of the target.
 * @param target the target to add.
 */
void add_target(Level* level, int x, int y, Target target);


#endif