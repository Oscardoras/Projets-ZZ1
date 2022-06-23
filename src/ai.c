#include <stdlib.h>

#include "ai.h"
#include "gameplay.h"


void compute_worker(Level* level, Entity* entity) {
    switch (entity->state) {
    case WORKER_SEEKING_FOOD_IN: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct ListCell* cell;
            for (cell = level->entities; cell != NULL; cell = cell->next) {
                if (cell->entity->type == FOOD) {
                    Position position = cell->entity->position;
                    Block b = *get_level_block(level, position.x, position.y);
                    if (b == AIR || b == LEAVES) {
                        entity->target = cell->entity;
                        break;
                    }
                }
            }
            if (cell == NULL)
                entity->state = WORKER_WAITING;
        } else if (target->position.x == entity->position.x && target->position.y == entity->position.y) {
            entity->state = WORKER_TAKING_FOOD;
        } else {
            entity->position = path_finding(level, entity->position, entity->target->position);
        }
        break;
    }
    case WORKER_SEEKING_FOOD_OUT: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct ListCell* cell;
            for (cell = level->entities; cell != NULL; cell = cell->next) {
                if (cell->entity->type == FOOD) {
                    Position position = cell->entity->position;
                    Block b = *get_level_block(level, position.x, position.y);
                    if (b == PATH) {
                        entity->target = cell->entity;
                        break;
                    }
                }
            }
            if (cell == NULL)
                entity->state = WORKER_WAITING;
        } else if (target->position.x == entity->position.x && target->position.y == entity->position.y) {
            entity->state = WORKER_TAKING_FOOD;
        } else {
            entity->position = path_finding(level, entity->position, entity->target->position);
        }
        break;
    }
    case WORKER_TAKING_FOOD: {
        Entity* target = entity->target;
        if (target == NULL) {
            entity->state = WORKER_WAITING;
        } else {
            //Increase food level.
        }
        break;
    }
    case WORKER_BUILDING: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct ListCell* cell;
            for (cell = level->entities; cell != NULL; cell = cell->next) {
                if (cell->entity->type == PHEROMONE && cell->entity->state == PHEROMONE_DIG) {
                    entity->target = cell->entity;
                    break;
                }
            }
            if (cell == NULL)
                entity->state = WORKER_WAITING;
        } else if (target->position.x == entity->position.x && target->position.y == entity->position.y) {
            Block* b = get_level_block(level, target->position.x, target->position.y);
            if (target->state == PHEROMONE_DIG && *b == DIRT) {
                *b = PATH;
                target->state = 0;
            } else if (target->state == PHEROMONE_DIG && *b == GRASS) {
                *b = GRASS;
                target->state = 0;
            } else if (target->state == PHEROMONE_FILL && (*b == PATH || *b == AIR)) {
                *b = DIRT;
                target->state = 0;
            }
        } else {
            entity->position = path_finding(level, entity->position, entity->target->position);
        }
        break;
    }
    default:
        break;
    }

    if (entity->hp <= 0)
        entity->state = WORKER_DEAD;
}

void compute_soldier(Level* level, Entity* entity) {
    switch (entity->state) {
    case SOLDIER_SEEKING_ENNEMI: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct ListCell* cell;
            for (cell = level->entities; cell != NULL; cell = cell->next) {
                if (cell->entity->type == MANTIS) {
                    entity->target = cell->entity;
                    break;
                }
            }
            if (cell == NULL)
                entity->state = SOLDIER_WAITING;
        } else if (target->position.x == entity->position.x && target->position.y == entity->position.y) {
            entity->state = SOLDIER_FIGHTING;
        } else {
            entity->position = path_finding(level, entity->position, entity->target->position);
        }
        break;
    }
    case SOLDIER_FIGHTING:
        entity->target->hp -= entity_types[entity->type]->stats.atk;
        break;
    default:
        break;
    }

    if (entity->hp <= 0)
        entity->state = SOLDIER_DEAD;
}

void compute_mantis(Level* level, Entity* entity) {
    switch (entity->state) {
    case SOLDIER_SEEKING_ENNEMI: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct ListCell* cell;
            for (cell = level->entities; cell != NULL; cell = cell->next) {
                if (cell->entity->type == WORKER || cell->entity->type == SOLDIER) {
                    entity->target = cell->entity;
                    break;
                }
            }
            if (cell == NULL)
                entity->state = SOLDIER_WAITING;
        } else if (target->position.x == entity->position.x && target->position.y == entity->position.y) {
            entity->state = SOLDIER_FIGHTING;
        } else {
            entity->position = path_finding(level, entity->position, entity->target->position);
        }
        break;
    }
    case SOLDIER_FIGHTING:
        entity->target->hp -= entity_types[entity->type]->stats.atk;
        break;
    default:
        break;
    }

    if (entity->hp <= 0)
        entity->state = SOLDIER_DEAD;
}

void compute_pheromone(__attribute__((unused)) Level* level, Entity* entity) {
    entity->hp--;

    if (entity->hp <= 0)
        entity->state = 0;
}

void compute_food(__attribute__((unused)) Level* level, Entity* entity) {
    if (entity->hp <= 0)
        entity->state = 0;
}


typedef struct Vertex {
    Position position;
    int d;
    struct Vertex* parent;
    bool listed;
} Vertex;


Vertex* get_vertex(Level* level, Vertex *vertices, int x, int y) {
    if (level->d.min_x <= x && x < level->d.max_x && level->d.min_y <= y && y < level->d.max_y)
        return &vertices[(y - level->d.min_y)*(level->d.max_x - level->d.min_x) + (x - level->d.min_x)];
    else
        return NULL;
}

void relachement(Vertex* u, Vertex* v) {
    if (v->d > u->d + 1) {
        v->d = u->d + 1;
        v->parent = u;
    }
}

Position path_finding(Level* level, Position from, Position to) {
    unsigned int width = level->d.max_x - level->d.min_x;
    unsigned int height = level->d.max_y - level->d.min_y;
    unsigned int size = width * height;
    Vertex *vertices = malloc(sizeof(Vertex) * size);
    for (int x = level->d.min_x; x < level->d.max_x; x++) {
        for (int y = level->d.min_y; y < level->d.max_y; y++) {
            Vertex* vertex = get_vertex(level, vertices, x, y);
            vertex->position.x = x;
            vertex->position.y = y;
            vertex->d = __INT_MAX__;
            vertex->parent = NULL;
            vertex->listed = false;
        }
    }
    
    Vertex* list[1024];
    unsigned int list_size = 1;
    list[0] = get_vertex(level, vertices, from.x, from.y);
    list[0]->d = 0;
    
    unsigned int old_size = 0;
    while (old_size < list_size) {
        old_size = size;
        
        Vertex* u = list[0];
        for (unsigned int k = 0; k < list_size; k++) {
            if (list[k]->d < u->d)
                u = list[k];
        }
        
        Block* left = get_level_block(level, u->position.x-1, u->position.y);
        if (left != NULL && (*left == AIR || *left == PATH)) {
            list[list_size] = get_vertex(level, vertices, u->position.x-1, u->position.y);
            list_size++;
            list[list_size]->listed = true;
            relachement(u, list[list_size]);
        }
        
        Block* right = get_level_block(level, u->position.x+1, u->position.y);
        if (right != NULL && (*right == AIR || *right == PATH)) {
            list[list_size] = get_vertex(level, vertices, u->position.x+1, u->position.y);
            list_size++;
            list[list_size]->listed = true;
            relachement(u, list[list_size]);
        }
        
        Block* top = get_level_block(level, u->position.x, u->position.y+1);
        if (top != NULL && (*top == AIR || *top == PATH)) {
            list[list_size] = get_vertex(level, vertices, u->position.x, u->position.y+1);
            list_size++;
            list[list_size]->listed = true;
            relachement(u, list[list_size]);
        }
        
        Block* bot = get_level_block(level, u->position.x, u->position.y-1);
        if (bot != NULL && (*bot == AIR || *bot == PATH)) {
            list[list_size] = get_vertex(level, vertices, u->position.x, u->position.y-1);
            list_size++;
            list[list_size]->listed = true;
            relachement(u, list[list_size]);
        }
    }
    
    Vertex* v = get_vertex(level, vertices, to.x, to.y);
    while (!(v->parent->position.x == from.x && v->parent->position.y == from.y))
        v = v->parent;
    
    return v->position;
}