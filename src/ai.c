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
        } else if (!path_finding(level, &entity->position, entity->target->position)) {
            entity->state = SOLDIER_WAITING;
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
        } else if (!path_finding(level, &entity->position, entity->target->position)) {
            entity->state = SOLDIER_WAITING;
        }
        break;
    }
    case WORKER_TAKING_FOOD: {
        Entity* target = entity->target;
        if (target == NULL) {
            entity->state = WORKER_WAITING;
        } else {
            target->hp--;
            level->states.food++;
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
            if (target->state == PHEROMONE_FILL) {
                Block* b = get_level_block(level, target->position.x, target->position.y);
                if (*b == PATH || *b == AIR) {
                    *b = DIRT;
                    target->state = 0;
                }
            } else if (target->state == PHEROMONE_DIG) {
                for (int i = target->position.x-1; i <= target->position.x+1; i++) {
                    for (int j = target->position.y-1; j <= target->position.y+1; j++) {
                        Block* b = get_level_block(level, i, j);
                        if (b != NULL && (*b == DIRT || *b == GRASS)) {
                            *b = PATH;
                        }
                    }
                }
            }
            target->state = 0;
        } else if (!path_finding(level, &entity->position, entity->target->position)) {
            entity->state = SOLDIER_WAITING;
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
        } else if (!path_finding(level, &entity->position, entity->target->position)) {
            entity->state = SOLDIER_WAITING;
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

void compute_queen(Level* level, Entity* entity) {
    if (entity->state == 1 && level->states.food > 0) {
        add_level_entity(level, new_entity(rand()%2 == 0 ? WORKER : SOLDIER, entity->position, 0, WORKER_WAITING));
    }
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
        } else if (!path_finding(level, &entity->position, entity->target->position)) {
            entity->state = SOLDIER_WAITING;
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
    bool present;
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

bool path_finding(Level* level, Position* from, Position to) {
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
            vertex->present = false;
            vertex->listed = false;
        }
    }
    
    Vertex* list[1024];
    unsigned int list_size = 1;
    list[0] = get_vertex(level, vertices, from->x, from->y);
    list[0]->d = 0;
    list[0]->present = true;
    list[0]->listed = false;

    Vertex* e = get_vertex(level, vertices, to.x, to.y);
    Vertex inf;
    inf.d = __INT_MAX__;
    
    Vertex* u = list[0];
    while (u != &inf) {
        u = &inf;
        for (unsigned int k = 0; k < list_size; k++) {
            if (!list[k]->listed && list[k]->d < u->d)
                u = list[k];
        }
        u->listed = true;
        
        if (is_valid_position(level, u->position.x-1, u->position.y)) {
            Vertex* v = get_vertex(level, vertices, u->position.x-1, u->position.y);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
        
        if (is_valid_position(level, u->position.x+1, u->position.y)) {
            Vertex* v = get_vertex(level, vertices, u->position.x+1, u->position.y);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
        
        if (is_valid_position(level, u->position.x, u->position.y+1)) {
            Vertex* v = get_vertex(level, vertices, u->position.x, u->position.y+1);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
        
        if (is_valid_position(level, u->position.x, u->position.y-1)) {
            Vertex* v = get_vertex(level, vertices, u->position.x, u->position.y-1);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
    }
    
    if (e->parent != NULL) {
        while (e->parent->parent != NULL)
            e = e->parent;
        
        Direction direction;
        if (e->position.x < from->x) direction = LEFT;
        else if (e->position.x > from->x) direction = RIGHT;
        else if (e->position.y < from->y) direction = BOT;
        else if (e->position.y > from->y) direction = TOP;

        *from = e->position;
        from->direction = direction;
        return true;
    } return false;
}