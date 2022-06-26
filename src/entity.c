#include <stdlib.h>
#include <string.h>

#include "entity.h"


EntityType* entity_types[ENTITY_TYPES] = {};


Entity* new_entity(EntityTypeName type, Location location, State state) {
    Entity* entity = malloc(sizeof(Entity));
    
    if (entity != NULL) {
        entity->type = type;
        entity->location = location;
        entity->hp = entity_types[type]->stats.hp;
        entity->state = state;
        entity->target = NULL;
    }
    
    return entity;
}

void save_entity(Entity* entity, FILE* file) {
    fprintf(file, "%d %d %d %d %d %d\n",
        entity->type,
        entity->location.x,
        entity->location.y,
        entity->location.direction,
        entity->state,
        entity->hp
    );
}

Entity* load_entity(FILE* file) {
    Entity* entity = malloc(sizeof(Entity));

    if (entity != NULL) {
        int n = fscanf(file, "%d %d %d %d %d %d",
            &entity->type,
            &entity->location.x,
            &entity->location.y,
            &entity->location.direction,
            &entity->state,
            &entity->hp
        );

        if (n != 6) {
            free(entity);
            entity = NULL;
        }
    }
    
    return entity;
}

void free_entity(Entity* entity) {
    if (entity != NULL)
        free(entity);
}
    
EntityType* load_type(FILE* file) {
    EntityType* type = malloc(sizeof(EntityType));

    if (type != NULL) {
        char buffer[256];
        fgets(buffer, 256, file); //Type name, ignored.
        int n = fscanf(file, "%d %d\n",
            &type->stats.hp,
            &type->stats.atk
        );

        if (n == 2) {
            type->markov = load_matrix(file);
            if (type->markov == NULL) {
                free(type);
                type = NULL;
            }
        } else {
            free(type);
            type = NULL;
        }
    }

    return type;
}

void free_type(EntityType* type) {
    if (type != NULL) {
        free_matrix(type->markov);
        free(type);
    }
}

void load_types(FILE* file) {
    for (unsigned int i = 0; i < ENTITY_TYPES; i++)
        entity_types[i] = load_type(file);
}

void free_types() {
    for (unsigned int i = 0; i < ENTITY_TYPES; i++)
        free_type(entity_types[i]);
}
