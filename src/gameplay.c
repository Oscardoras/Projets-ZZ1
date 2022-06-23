#include <stdlib.h>
#include <math.h>

#include "gameplay.h"

#define PONTE_OUVR 3

void update_entity(Entity* entity) {
    forward(&(entity->type->markov), &(entity->state));
}

void set_global_behaviour(Level* level, GlobalBehavior behaviour) {
    level->states.behavior = behaviour;
}

void add_target(Level* level, int x, int y, Target target) {
    Position pos;
    pos.direction = 0;
    pos.x = x;
    pos.y = y;
    
    Entity* pheromone = new_entity(
        search_type("Pheromone"),
        target,
        0,
        pos
    );
    
    add_level_entity(level, pheromone);
}

void lay_egg_queen(Entity* queen, Level* level) {
    update_entity(queen);
    
    float rand_type = (float)(rand()%1000)/1000.0;
    
    EntityType* type =
        (rand_type < 0.5/*Où est stocker ratio*/) ?
        search_type("Ouvriere"):
        search_type("Guerriere");

    add_level_entity(level,
        new_entity(type, 0, 0, queen->position)
    );
}

void update_queen(Entity* queen, Level* level, QueenAction action) {
    if(queen->state == 2) {
        if(action == MOVE) {
            //On doit voir
        }
        if(action == LAY) {
            lay_egg_queen(queen, level);
        }
    }
    else {
        update_entity(queen);
    }
}

