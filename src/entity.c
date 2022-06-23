#include <stdlib.h>
#include <string.h>

#include "entity.h"

#define ENTITY_TYPES 10


EntityType* entity_types[ENTITY_TYPES] = {};


Entity* new_entity(EntityTypeName type, Position position, int hp, State state) {
    Entity* entity = malloc(sizeof(Entity));
    
    if (entity != NULL) {
        entity->position = position;
        entity->type = type;
        entity->hp = (hp != 0) ? hp : entity_types[type]->stats.hp;
        entity->state = state;
        entity->target = NULL;
    }
    
    return entity;
}

void save_entity(Entity* entity, FILE* file) {
    fprintf(file, "%d %d %d %d %d %d\n",
        entity->type,
        entity->position.x,
        entity->position.y,
        entity->position.direction,
        entity->hp,
        entity->state
    );
}

Entity* load_entity(FILE* file) {
    Entity* entity = NULL;

    char line[128];
    char split[32];
    int i_line = 0;
    int i_split;

    if (fgets(line, 128, file)) {
        EntityTypeName type;
        Position position;
        int hp;
        State state;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        type = atoi(split);
        i_line++;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        position.x = atoi(split);
        i_line++;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        position.y = atoi(split);
        i_line++;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        position.direction = atoi(split);
        i_line++;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        hp = atoi(split);
        i_line++;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        state = atoi(split);
        i_line++;

        entity = new_entity(type, position, hp, state);
    }
    
    return entity;
}

void free_entity(Entity* entity) {
    if (entity != NULL)
        free(entity);
}
    
EntityType* load_type(FILE* file) {
    EntityType* type = NULL;

    char *c;
    char line[32];
    
    if (fgets(line, 32, file)) { //Pas à la fin du fichier
        type = malloc(sizeof(EntityType));
        
        if (type != NULL) {
            fgets(line, 32, file);
            for(c = line; *c != '\n'; c++);
            *c = '\0';
            type->stats.hp = atoi(line);
            
            fgets(line, 32, file);
            for(c = line; *c != '\n'; c++);
            *c = '\0';
            type->stats.atk = atoi(line);
            
            fgets(line, 32, file);
            for(c = line; *c != '\n'; c++);
            *c = '\0';
            type->stats.atk_speed = atoi(line);
            
            fgets(line, 32, file);
            for(c = line; *c != '\n'; c++);
            *c = '\0';
            type->stats.speed = atoi(line);
            
            type->markov = load_matrix(file);
            
            fgets(line, 32, file); //Ligne vide entre chaque type
        }
    }
    
    return type;
}

void free_type(EntityType* type) {
    if (type != NULL) {
        free_matrix(&type->markov);
        free(type);
    }
}

void load_types(FILE* file) {
    for (int i = 0; i < ENTITY_TYPES; i++)
        entity_types[i] = load_type(file);
}

void free_types() {
    for (int i = 0; i < ENTITY_TYPES; i++)
        free_type(entity_types[i]);
}
