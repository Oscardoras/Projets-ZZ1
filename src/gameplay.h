#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "character.h"
enum {
    AGGRESSIVE
    DEFESIVE
    NOT_GOING_OUT
    GOING_OUT
    TARGET_FOOD
    TARGET_BUILD
    } rule_enum;
    
void changeRule(Entity * entities, unsigned int count, rule_enum rule);

#endif