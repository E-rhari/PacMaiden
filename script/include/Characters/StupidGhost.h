/* TEMPORÁRIO */

#include "raylib.h"
#include<stdbool.h>

#include "./Character.h"
#include "./PacMaiden.h"
#include "../Map/Map.h"

#include "Ghost.h"
#pragma once
void stupidMove(Ghost* ghost, Map map, ProceduralAnimation* animation){
    blinkAnimation(&ghost->chara.color, ghost->initialValues.color, WHITE, animation, 3, 1);
    if(!ghost->chara.procAnimation.running)
        ghost->chara.procAnimation.running = true;
    moveUnaware(ghost, map);
}