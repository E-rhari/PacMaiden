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


void choseDestination(Ghost ghost, Vector2* moveDirection){
    static int seedSpice = 0;
    if ((int)(ghost.chara.circle.center.x+ghost.chara.circle.radius)%40 < 3 
    && (int)(ghost.chara.circle.center.y+ghost.chara.circle.radius)%40 < 3){
        srand(time(0) + seedSpice);
        int random = (rand()%4) + 1;
        if(random == 1)
            *moveDirection = (Vector2){0,-1};
        if(random == 2)
            *moveDirection = (Vector2){0,1};
        if(random == 3)
            *moveDirection = (Vector2){-1, 0};
        if(random == 4)
            *moveDirection = (Vector2){1, 0};

        seedSpice++;
        if(seedSpice > 500)
            seedSpice = 0;
    }
}

bool moveGhost(Ghost* ghost, Map map){
    Vector2 destination;
    choseDestination(*ghost, &ghost->chara.moveDirection);
    return move(&ghost->chara, map);
}