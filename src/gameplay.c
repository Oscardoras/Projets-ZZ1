#include "gameplay.h"

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