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


void choseDestinationAware(Ghost* ghost, Map map){
    if (isInGridCenter(ghost->chara) && validadePosition(ghost->chara, (Vector2){0,0})){
        Vector2 directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        
        Vector2 *possibleDirections= (Vector2*)malloc(1);
        int length=0;

        for(int i=0; i<4; i++){
            if(readPositionInMap(ghost->chara.circle.center, map, directions[i]) != '#'
            && !Vector2Equals(directions[i], Vector2Scale(ghost->chara.moveDirection,-1))){
                possibleDirections = (Vector2*)realloc(possibleDirections, length+1);
                possibleDirections[length] = directions[i];
                length++;
            }
        }
        if(length>0){
            int random = (rand()%length);
            ghost->chara.moveDirection = possibleDirections[random];
        }
        else
            ghost->chara.moveDirection = Vector2Scale(ghost->chara.moveDirection,-1);
    }
}


bool moveAware(Ghost* ghost, Map map){
    choseDestinationAware(ghost, map);
    return move(&ghost->chara, map);
}


void choseDestinationUnaware(Ghost* ghost){
    if (isInGridCenter(ghost->chara) && validadePosition(ghost->chara, (Vector2){0,0})){
        int random = (rand()%4) + 1;
        if(random == 1)
            ghost->chara.moveDirection = (Vector2){0,-1};
        else if(random == 2)
            ghost->chara.moveDirection = (Vector2){0,1};
        else if(random == 3)
            ghost->chara.moveDirection = (Vector2){-1, 0};
        else if(random == 4)
            ghost->chara.moveDirection = (Vector2){1, 0};
    }
}

bool moveUnaware(Ghost* ghost, Map map){
    choseDestinationUnaware(ghost);
    return move(&ghost->chara, map);
}