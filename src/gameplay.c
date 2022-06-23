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

        printf("Entity of type %d has state %d.\n", entity->type, entity->state);

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

bool is_valid_position(Level* level, int x, int y) {
    Block* b = get_level_block(level, x, y);

    if (b != NULL) {
        if (*b == PATH) return true;
        else if (*b == AIR) {
            Block* bot = get_level_block(level, x, y-1);
            Block* left = get_level_block(level, x, y-1);
            Block* right = get_level_block(level, x, y-1);
            Block* bot_left = get_level_block(level, x-1, y-1);
            Block* bot_right = get_level_block(level, x-1, y-1);

            if (bot != NULL && (*bot == PATH || *bot == GRASS || *bot == DIRT)) return true;
            if (left != NULL && (*left == PATH || *left == GRASS || *left == DIRT)) return true;
            if (right != NULL && (*right == PATH || *right == GRASS || *right == DIRT)) return true;
            if (bot_left != NULL && (*bot_left == PATH || *bot_left == GRASS || *bot_left == DIRT)) return true;
            if (bot_right != NULL && (*bot_right == PATH || *bot_right == GRASS || *bot_right == DIRT)) return true;
        }
    }

    return false;
}
