#include <math.h>
#include <stdlib.h>

#include "ai.h"
#include "gameplay.h"

#define PONTE_OUVR 3


void game_loop_iteration(Level* level) {
    struct ListCell* cell = level->entities;
    while (cell != NULL) {
        Entity* entity = cell->entity;
        Entity* target = entity->target;
        
        if (entity->target == NULL || (target->position.x == entity->position.x && target->position.y == entity->position.y)) {
            forward_state(&entity->type->markov, &entity->state);
            switch (entity->type) {
            case /* constant-expression */:
                /* code */
                break;
            
            default:
                break;
            }
        } else {
            Direction direction = path_finding(level, entity->position.x, entity->position.y, target->position.x, target->position.y);
        }
        
        cell = cell->next;
    }
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
    );update_entity
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

void update_list(ListUpdate* list) {
    ListUpdate* cour = list;
    ListUpdate* temp = NULL;
    while(cour) {
        update_entity(cour->entity);
        temp = cour;
        cour = cour->next;
        free(temp);
    }
}

void attack(Entity* entity) {
    if(entity && entity->target) {
        entity->target->hp -= entity->type->stats.atk;
        
        if(entity->target->hp <= 0) {
            
        }
    }
}

void attack_list(ListUpdate* list) {
    ListUpdate* cour = list;
    ListUpdate* temp = NULL;
    while(cour) {
        attack(cour->entity);
        temp = cour;
        cour = cour->next;
        free(temp);
    }
}