#include <stdlib.h>

#include "character.h"


#define BORN_STATE 0

EntityType* entity_types[10] = {};


Entity* new_entity(Position position, State state, EntityType type) {
    Entity* entity = malloc(sizeof(entity));
    
    if(entity) {
        entity->world_position = position;
        entity->current_state = state;
        entity->type = &type;
        entity->current_hp = entity->type->stats.hp;
    }
    
    return entity;
}

void save_entity(Entity* entity, FILE* file) {
    
}

Entity* load_entity(FILE* file) {
    return NULL;
}

void free_entity(Entity* entity) {
    if(entity) {
        entity->type = NULL;
        free(entity);
    }
}

void load_types(FILE* file) {
    for(int i=0; i<10; i++) {
        entity_types[i] = load_type(file);
    }
}
void save_entity ()
{
    
}
EntityType* load_type(FILE* file) {
    int i;
    char* c;
    char line[32];
    EntityType* type = NULL;
    
    if(fgets(line, 32, file)) { //Pas à la fin du fichier
        type = malloc(sizeof(EntityType));
        
        if(type) {
            for(i=0; line[i] != '\n'; i++)
                type->name[i]=line[i];
            type->name[i] = '\0';
            
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
            
            type->markov = initMatrix(file);
            
            fgets(line, 32, file); //Ligne vide entre chaque type
        }
    }
    
    return type;
}

void free_types() {
    for(int i=0; i<10; i++)
        free_type(entity_types[i]);
}

void free_type(EntityType* type) {
    if(type) {
        if(type->name) {
            free(type->name);
        }
        if(type->markov.data) {
            closeMatrix(&(type->markov));
        }
        free(type);
    }
}