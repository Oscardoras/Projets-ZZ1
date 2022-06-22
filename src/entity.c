#include <stdlib.h>
#include <stdbool.h>

#include "entity.h"


EntityType* entity_types[10] = {};


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
    fprintf(file,
        "%d %d %d %d %d %s\n",
        entity->position.x,
        entity->position.y,
        entity->position.rotation,
        entity->hp,
        entity->state,
        entity->type->name
    );
}

Entity* load_entity(FILE* file) {
    char* line[128];
    char* split[32];
    int i_line=0;
    int i_split;
    Entity* entity = NULL;

    if(fgets(line, 128, file)) {
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
            
            type->markov = new_matrix(file);
            
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