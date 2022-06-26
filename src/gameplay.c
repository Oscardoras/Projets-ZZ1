#include <math.h>
#include <stdlib.h>

#include "ai.h"
#include "gameplay.h"

#define PONTE_OUVR 3


void game_loop_iteration(Level* level) {
    clean_level_entities(level);

    for (struct EntityListCell* it = level->entities; it != NULL; it = it->next) {
        Entity* entity = it->entity;
        
        switch (entity->type) {
        case ENTITY_WORKER:
            compute_worker(level, entity);
            break;
        case ENTITY_SOLDIER:
            compute_soldier(level, entity);
            break;
        case ENTITY_QUEEN:
            compute_queen(level, entity);
            break;
        case ENTITY_MANTIS:
            compute_mantis(level, entity);
            break;
        case ENTITY_PHEROMONE:
            compute_pheromone(level, entity);
            break;
        case ENTITY_FOOD:
            compute_food(level, entity);
            break;
        default:
            break;
        }

        printf("Entity of type %d had state %d", entity->type, entity->state);
        forward_state(entity_types[entity->type]->markov, &entity->state);
        printf(" then %d.\n", entity->state);
    }

    int random = rand() % 20;
    if (random == 0) {
        int x = level->d.min_x + rand() % (level->d.max_x - level->d.min_x);
        int y;

        for (y = level->d.max_y-1; *get_level_block(level, x, y) == BLOCK_AIR; y--);

        if (get_level_block(level, x, y) != NULL) {
            Location location;
            location.x = x;
            location.y = y+1;
            location.direction = rand() % 4;
            add_level_entity(level, new_entity(ENTITY_FOOD, location, 1));
        }
    }
}

void set_global_behaviour(Level* level, GlobalBehavior behaviour) {
    level->states.behavior = behaviour;
}

void add_pheromone(Level* level, int x, int y, PheromoneType type) {
    Location location;
    location.x = x;
    location.y = y;
    location.direction = 0;
    
    Entity* pheromone = new_entity(ENTITY_PHEROMONE, location, type);
    add_level_entity(level, pheromone);
}

bool is_valid_position(Level* level, int x, int y) {
    Block* b = get_level_block(level, x, y);

    if (b != NULL) {
        if (*b == BLOCK_PATH) return true;
        else if (*b == BLOCK_AIR) {
            Block* bot = get_level_block(level, x, y-1);
            Block* left = get_level_block(level, x-1, y);
            Block* right = get_level_block(level, x+1, y);
            Block* bot_left = get_level_block(level, x-1, y-1);
            Block* bot_right = get_level_block(level, x+1, y-1);

            if (bot != NULL && (*bot == BLOCK_PATH || *bot == BLOCK_GRASS || *bot == BLOCK_DIRT)) return true;
            if (left != NULL && (*left == BLOCK_PATH || *left == BLOCK_GRASS || *left == BLOCK_DIRT)) return true;
            if (right != NULL && (*right == BLOCK_PATH || *right == BLOCK_GRASS || *right == BLOCK_DIRT)) return true;
            if (bot_left != NULL && (*bot_left == BLOCK_PATH || *bot_left == BLOCK_GRASS || *bot_left == BLOCK_DIRT)) return true;
            if (bot_right != NULL && (*bot_right == BLOCK_PATH || *bot_right == BLOCK_GRASS || *bot_right == BLOCK_DIRT)) return true;
        }
    }

    return false;
}
