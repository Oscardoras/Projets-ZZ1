#ifndef __AI_H__
#define __AI_H__

#include "level.h"

enum WorkerStates {
    WORKER_VOID,
    WORKER_WAITING,
    WORKER_DEAD,
    WORKER_SEEKING_FOOD_OUT,
    WORKER_SEEKING_FOOD_IN,
    WORKER_TAKING_FOOD,
    WORKER_BUILDING,
};
void compute_worker(Level* level, Entity* entity);

enum SoldierStates {
    SOLDIER_VOID,
    SOLDIER_WAITING,
    SOLDIER_DEAD,
    SOLDIER_SEEKING_ENNEMI,
    SOLDIER_FIGHTING,
};
void compute_soldier(Level* level, Entity* entity);

void compute_queen(Level* level, Entity* entity);

void compute_mantis(Level* level, Entity* entity);

void compute_pheromone(Level* level, Entity* entity);

void compute_food(Level* level, Entity* entity);


typedef struct Vertex {
    Position position;
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
 * @param from a pointer to the current position.
 * @param to the destination.
 * @return if there is a path.
 */
bool path_finding(Level* level, Position* from, Position to);


#endif