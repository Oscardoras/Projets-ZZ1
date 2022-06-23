#ifndef __AI_H__
#define __AI_H__

#include "level.h"


typedef struct Vertex {
    int x;
    int y;
    int d;
    struct Vertex* parent;
    bool listed;
} Vertex;


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