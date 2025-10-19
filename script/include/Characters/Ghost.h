#include "raylib.h"
#include<stdbool.h>

#include "./Character.h"
#include "../Map/Map.h"

#pragma once


typedef struct {
    Character chara;
} Ghost;


Ghost initGhost(Vector2 position, int radius, float speed, Color color){
    Circle characterRec = {(Vector2){position.x+radius, position.y+radius}, radius};
    Character chara = (Character){characterRec, speed, color};
    return (Ghost){chara};
}


void choseDestinationUnaware(Ghost* ghost){
    static int seedSpice = 0;
    if ((int)(ghost->chara.circle.center.x+ghost->chara.circle.radius)%40 < 3 
    && (int)(ghost->chara.circle.center.y+ghost->chara.circle.radius)%40 < 3){
        int random = (rand()%4) + 1;
        if(random == 1)
            ghost->chara.moveDirection = (Vector2){0,-1};
        if(random == 2)
            ghost->chara.moveDirection = (Vector2){0,1};
        if(random == 3)
            ghost->chara.moveDirection = (Vector2){-1, 0};
        if(random == 4)
            ghost->chara.moveDirection = (Vector2){1, 0};
    }
}

bool moveUnaware(Ghost* ghost, Map map){
    Vector2 destination;
    choseDestinationUnaware(ghost);
    return move(&ghost->chara, map);
}