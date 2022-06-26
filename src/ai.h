#ifndef __AI_H__
#define __AI_H__

#include "level.h"


void compute_worker(Level* level, Entity* entity);

void compute_soldier(Level* level, Entity* entity);

void compute_queen(Level* level, Entity* entity);

void compute_mantis(Level* level, Entity* entity);

void compute_pheromone(Level* level, Entity* entity);

void compute_food(Level* level, Entity* entity);


typedef struct Vertex {
    int x;
    int y;
    int d;
    struct Vertex* parent;
    bool present;
    bool listed;
} Vertex;
Vertex* get_vertex(Level* level, Vertex *vertices, int x, int y);
void relachement(Vertex* u, Vertex* v);

/**
 * @brief Finds the next step of a path.
 * 
 * @param level the level.
 * @param from a pointer to the current location.
 * @param to the destination.
 * @return if there is a path.
 */
bool path_finding(Level* level, Location* from, Location to);


#endif