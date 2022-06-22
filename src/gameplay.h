#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "character.h"


enum {
    AGGRESSIVE,
    DEFESIVE,
    NOT_GOING_OUT,
    GOING_OUT,
    TARGET_FOOD,
    TARGET_BUILD
} rule_enum;
    
void change_rule(Level* level, rule_enum rule);


#endif