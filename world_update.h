#ifndef __WORLD_UPDATE_H__
#define __WORLD_UPDATE_H__

#include "world.h"


typedef char Rulestring[32];

/**
 * @brief Updates the state of a cell.
 * 
 * @param world a pointer to the world.
 * @param x the x coordinates of the cell to evoluate.
 * @param y the y coordinates of the cell to evoluate.
 * @param table the new cell table.
 * @param rule a rule string.
 * @return the state of the cell.
 */
bool update_world_cell(World* world, int x, int y, bool *table, Rulestring rule);

/**
 * @brief Updates the states of all the cells of a world.
 * 
 * @param world a pointer to the world.
 * @param rule a rule string.
 * @return if the world has change.
 */
bool update_world(World* world, Rulestring rule);


#endif
