#include "character.h"

Entity* new_entity(Position birth_position, EntityType type) {
    Entity* entity = malloc(sizeof(entity));
    
    if(entity) {
        entity->world_position = birth_position;
        entity->current_state = 0;
        entity->type = &type;
        entity->current_hp = entity->type->stats.hp;
    }
    
    return entity;
}