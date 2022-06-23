#include <math.h>
#include <stdlib.h>

#include "ai.h"
#include "gameplay.h"

#define PONTE_OUVR 3


void game_loop_iteration(Level* level) {
    clean_level_entities(level);

    for (struct ListCell* cell = level->entities; cell != NULL; cell = cell->next) {
        Entity* entity = cell->entity;
        
        switch (entity->type) {
        case WORKER:
            compute_worker(level, entity);
            break;
        case SOLDIER:
            compute_soldier(level, entity);
            break;
        case MANTIS:
            compute_mantis(level, entity);
            break;
        case PHEROMONE:
            compute_pheromone(level, entity);
            break;
        case FOOD:
            compute_food(level, entity);
            break;
        default:
            break;
        }

        forward_state(&entity_types[entity->type]->markov, &entity->state);
    }
}

void set_global_behaviour(Level* level, GlobalBehavior behaviour) {
    level->states.behavior = behaviour;
}

void add_pheromone(Level* level, int x, int y, PheromoneType type) {
    Position pos;
    pos.direction = 0;
    pos.x = x;
    pos.y = y;
    
    Entity* pheromone = new_entity(PHEROMONE, pos, 0, type);
    add_level_entity(level, pheromone);
}


void lay_egg_queen(Entity* queen, Level* level) {
    float rand_type = (rand() % 1000) / 1000.;
    
    EntityTypeName type = (rand_type < 0.5/*Où est stocker ratio*/) ? WORKER : SOLDIER;

    add_level_entity(level, new_entity(type, queen->position, 0, 0));
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
}
