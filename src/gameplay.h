#ifndef __GAMEPLAY_H_
#define __GAMEPLAY_H_

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

typedef struct updateCell {
    Entity* entity;
    updateCell* next;
} ListUpdate;


/**
 * @brief Changes the state of an entity with Markov.
 * 
 * @param entity the entity to change.
 */
void update_state_entity(Entity* entity);

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

/**
 * @brief Updates the state of every entity which state has to be changed.
 * 
 * @param list the list of the entities to update
 */
void gameloop(ListUpdate* list) {
    ListUpdate* cour = list;
    ListUpdate* temp = NULL;
    while(cour) {
        update_entity(cour->entity);
        temp = cour;
        cour = cour->next;
        free(temp);
    }
}

#endif