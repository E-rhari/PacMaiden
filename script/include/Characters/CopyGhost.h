/* TEMPORÁRIO */

#include "raylib.h"
#include<stdbool.h>

#include "./Character.h"
#include "./PacMaiden.h"
#include "../Map/Map.h"

#include "Ghost.h"
#pragma once
void copyPacmaidenMovement(Ghost* ghost, Map map, PacMaiden* pacmaiden){
    ghost->chara.moveDirection = pacmaiden->chara.moveDirection;
    move(&ghost->chara, map);
}