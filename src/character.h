#ifndef __CHARACTER_H__
#define __CHARACTER_H__


#include <stdio.h>

#include "matrix.h"


typedef struct {
    int x;
    int y;
    int rotation;
} Position;

typedef struct {
    int hp;
    int atk;
    int atk_speed;
    int speed;
} InitStats;

typedef struct {
    char* name;
    InitStats stats;
    matrix_t behaviour;
    
} ToE; //Type of entity

typedef struct {
    int current_hp;
    int current_state;

    ToE* type;

    Position world_position;
} Entity;


extern ToE *ToE_array;

/**
 * @brief Creates a new entity in the world.
 * 
 * @param birth_position his position in the world.
 * @param type the type of entity created.
 * @return a pointer to the new entity created.
 */
Entity* new_entity (Position birth_position, ToE type);

/**
 * @brief Loads an entity from the save-file.
 * 
 * @param file the save-file.
 * @return Entity*, a pointer to the laoded entity.
 */
Entity* load_entity(FILE* file);

/**
 * @brief Loads all the ToE written in the type-file.
 * 
 * @param file the type-file.
 * @return ToE*, the ToE-array.
 */
ToE* load_types(FILE* file);

/**
 * @brief Loads a type from the type-file.
 * 
 * @param file the type-file.
 * @return ToE, the loaded ToE
 */
ToE load_type(FILE* file);

/**
 * @brief Frees the entity given by a pointer.
 * 
 * @param pEntity a pointer to an entity.
 */
void free_entity(Entity* pEntity);

/**
 * @brief Frees the ToE-array.
 * 
 * @param ToEarray the ToE-array.
 */
void free_types(ToE* ToEarray);

/**
 * @brief Frees a ToE in the ToE-array.
 * 
 * @param pToE a pointer to the ToE.
 */
void free_type(ToE* pToE);


#endif