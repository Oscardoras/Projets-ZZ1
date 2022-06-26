#include <stdlib.h>

#include "ai.h"
#include "gameplay.h"


void compute_worker(Level* level, Entity* entity) {
    switch (entity->state) {
    case WORKER_SEEKING_FOOD_IN: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct EntityListCell* it;
            for (it = level->entities; it != NULL; it = it->next) {
                if (it->entity->type == ENTITY_FOOD) {
                    Location location = it->entity->location;
                    Block b = *get_level_block(level, location.x, location.y);
                    if (b == BLOCK_PATH) {
                        entity->target = it->entity;
                        break;
                    }
                }
            }
            if (it == NULL)
                entity->state = WORKER_WAITING;
        } else if (target->location.x == entity->location.x && target->location.y == entity->location.y) {
            entity->state = WORKER_TAKING_FOOD;
        } else if (!path_finding(level, &entity->location, entity->target->location)) {
            printf("Path could not be find.\n");
            entity->state = WORKER_WAITING;
        }
        break;
    }
    case WORKER_SEEKING_FOOD_OUT: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct EntityListCell* it;
            for (it = level->entities; it != NULL; it = it->next) {
                if (it->entity->type == ENTITY_FOOD) {
                    Location location = it->entity->location;
                    Block b = *get_level_block(level, location.x, location.y);
                    if (b == BLOCK_AIR || b == BLOCK_LEAVES) {
                        entity->target = it->entity;
                        break;
                    }
                }
            }
            if (it == NULL)
                entity->state = WORKER_WAITING;
        } else if (target->location.x == entity->location.x && target->location.y == entity->location.y) {
            entity->state = WORKER_TAKING_FOOD;
        } else if (!path_finding(level, &entity->location, entity->target->location)) {
            printf("Path could not be find.\n");
            entity->state = WORKER_WAITING;
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
            struct EntityListCell* it;
            for (it = level->entities; it != NULL; it = it->next) {
                if (it->entity->type == ENTITY_PHEROMONE && it->entity->state == PHEROMONE_DIG) {
                    entity->target = it->entity;
                    break;
                }
            }
            if (it == NULL)
                entity->state = WORKER_WAITING;
        } else if (target->location.x == entity->location.x && target->location.y == entity->location.y) {
            if (target->state == PHEROMONE_FILL) {
                Block* b = get_level_block(level, target->location.x, target->location.y);
                if (*b == BLOCK_PATH || *b == BLOCK_AIR) {
                    *b = BLOCK_DIRT;
                    target->state = 0;
                }
            } else if (target->state == PHEROMONE_DIG) {
                for (int i = target->location.x-1; i <= target->location.x+1; i++) {
                    for (int j = target->location.y-1; j <= target->location.y+1; j++) {
                        Block* b = get_level_block(level, i, j);
                        if (b != NULL && (*b == BLOCK_DIRT || *b == BLOCK_GRASS)) {
                            *b = BLOCK_PATH;
                        }
                    }
                }
            }
            target->state = 0;
        } else if (!path_finding(level, &entity->location, entity->target->location)) {
            printf("Path could not be find.\n");
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
            struct EntityListCell* it;
            for (it = level->entities; it != NULL; it = it->next) {
                if (it->entity->type == ENTITY_MANTIS) {
                    entity->target = it->entity;
                    break;
                }
            }
            if (it == NULL)
                entity->state = SOLDIER_WAITING;
        } else if (target->location.x == entity->location.x && target->location.y == entity->location.y) {
            entity->state = SOLDIER_FIGHTING;
        } else if (!path_finding(level, &entity->location, entity->target->location)) {
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
        EntityTypeName type = rand()%2 == 0 ? ENTITY_WORKER : ENTITY_SOLDIER;
        add_level_entity(level, new_entity(type, entity->location, WORKER_WAITING));
        level->states.food--;
    }
}

void compute_mantis(Level* level, Entity* entity) {
    switch (entity->state) {
    case SOLDIER_SEEKING_ENNEMI: {
        Entity* target = entity->target;
        if (target == NULL) {
            struct EntityListCell* it;
            for (it = level->entities; it != NULL; it = it->next) {
                if (it->entity->type == ENTITY_WORKER || it->entity->type == ENTITY_SOLDIER) {
                    entity->target = it->entity;
                    break;
                }
            }
            if (it == NULL)
                entity->state = SOLDIER_WAITING;
        } else if (target->location.x == entity->location.x && target->location.y == entity->location.y) {
            entity->state = SOLDIER_FIGHTING;
        } else if (!path_finding(level, &entity->location, entity->target->location)) {
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

bool path_finding(Level* level, Location* from, Location to) {
    unsigned int size = (level->d.max_x - level->d.min_x) * (level->d.max_y - level->d.min_y);
    Vertex *vertices = malloc(sizeof(Vertex) * size);

    for (int x = level->d.min_x; x < level->d.max_x; x++) {
        for (int y = level->d.min_y; y < level->d.max_y; y++) {
            Vertex* vertex = get_vertex(level, vertices, x, y);
            vertex->x = x;
            vertex->y = y;
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
        if (is_valid_position(level, u->x-1, u->y)) {
            Vertex* v = get_vertex(level, vertices, u->x-1, u->y);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
        
        if (is_valid_position(level, u->x+1, u->y)) {
            Vertex* v = get_vertex(level, vertices, u->x+1, u->y);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
        
        if (is_valid_position(level, u->x, u->y+1)) {
            Vertex* v = get_vertex(level, vertices, u->x, u->y+1);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
        
        if (is_valid_position(level, u->x, u->y-1)) {
            Vertex* v = get_vertex(level, vertices, u->x, u->y-1);
            relachement(u, v);
            if (!v->present) {
                list[list_size] = v;
                list_size++;
                v->present = true;
            }
        }
        
        u = &inf;
        for (unsigned int k = 0; k < list_size; k++) {
            if (!list[k]->listed && list[k]->d < u->d)
                u = list[k];
        }
        u->listed = true;
    }
    
    if (e->parent != NULL) {
        while (e->parent->parent != NULL)
            e = e->parent;
        
        Direction direction;
        if (e->x < from->x) direction = DIRECTION_LEFT;
        else if (e->x > from->x) direction = DIRECTION_RIGHT;
        else if (e->y < from->y) direction = DIRECTION_BOT;
        else if (e->y > from->y) direction = DIRECTION_TOP;

        from->x = e->x;
        from->y = e->y;
        from->direction = direction;

        free(vertices);
        return true;
    } else {
        free(vertices);
        return false;
    }
}