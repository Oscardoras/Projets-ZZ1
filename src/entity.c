#include <stdlib.h>

#include "entity.h"

#define ENTITY_TYPES 10


EntityType* entity_types[ENTITY_TYPES] = {};


Entity* new_entity(EntityType* type, State state, int hp, Position position) {
    Entity* entity = malloc(sizeof(entity));
    
    if (entity) {
        entity->type = type;
        entity->hp = (hp != 0) ? hp : entity->type->stats.hp;
        entity->state = state;
        entity->position = position;
    }
    
    return entity;
}

void save_entity(Entity* entity, FILE* file) {
    fprintf(file, "%d %d %d %d %d %s\n",
        entity->position.x,
        entity->position.y,
        entity->position.rotation,
        entity->hp,
        entity->state,
        entity->type->name
    );
}

Entity* load_entity(FILE* file) {
    Entity* entity = NULL;

    char line[128];
    char split[32];
    int i_line = 0;
    int i_split;

    if (fgets(line, 128, file)) {
        Position position;
        State state;
        int hp;
        EntityType* type;

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
        position.rotation = atoi(split);

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        state = atoi(split);
        i_line++;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        hp = atoi(split);
        i_line++;

        for(i_split=0; line[i_line] != ' '; i_line++, i_split++)
            split[i_split] = line[i_line];
        split[i_split] = '\0';
        type = search_type(split);

        entity = new_entity(type, state, hp, position);
    }
    
    return entity;
}

void free_entity(Entity* entity) {
    if (entity != NULL)
        free(entity);
}

void load_types(FILE* file) {
    for (int i = 0; i < ENTITY_TYPES; i++)
        entity_types[i] = load_type(file);
}
    
EntityType* load_type(FILE* file) {
    EntityType* type = NULL;

    int i;
    char* c;
    char line[32];
    
    if (fgets(line, 32, file)) { //Pas à la fin du fichier
        type = malloc(sizeof(EntityType));
        
        if (type != NULL) {
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
            
            type->markov = load_matrix(file);
            
            fgets(line, 32, file); //Ligne vide entre chaque type
        }
    }
    
    return type;
}

bool compare_str(char* str1, char* str2) {
    bool identical = true;
    if(str1 && str2) {
        int i=0;
        while(str1[i] != '\0') {
            identical = (str1[i] == str2[i]);
        }
        identical = (str2[i] = '\0');
    }
    else identical = (!str1 && !str2);

    return identical;
}

EntityType* search_type(char* name) {
    int i;

    for(i=0; !compare_str(name, entity_types[i]->name); i++);

    return entity_types[i];
}

void free_types() {
    for (int i = 0; i < ENTITY_TYPES; i++)
        free_type(entity_types[i]);
}

void free_type(EntityType* type) {
    if (type != NULL) {
        free_matrix(&type->markov);
        free(type);
    }
}