#include "gameplay.h"

#define PONTE_OUVR 3

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
    forward(&(queen->type->markov), &(queen->state));
    
    EntityType type =
        (queen->state == PONTE_OUVR) ?
        search_type("Ouvriere"):
        search_type("Guerriere");
    
    add_level_entity(level,
        new_entity(type, 0, 0, queen->position)
    );
}

void update_queen(Entity* queen, Level* level, QueenAction action) {
    
}