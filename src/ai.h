#ifndef __AI_H__
#define __AI_H__


typedef struct Vertex {
    int d;
    struct Vertex* parent;
} Vertex;


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

/**
 * @brief Finds the direction for going from a point to another.
 * 
 * @param level the level.
 * @param from_x 
 * @param from_y 
 * @param to_x 
 * @param to_y 
 * @return the best direction.
 */
Direction path_finding(Level* level, int from_x, int from_y, int to_x, int to_y);


#endif