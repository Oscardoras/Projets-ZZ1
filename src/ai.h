#ifndef __AI_H__
#define __AI_H__


/**
 * @brief Computes the behavior of a worker ant.
 * 
 * @param level the level.
 * @param entity the worker ant.
 */
void worker(Level* level, Entity* entity);

/**
 * @brief Computes the behavior of a soldier ant.
 * 
 * @param level the level.
 * @param entity the soldier ant.
 */
void soldier(Level* level, Entity* entity);


#endif