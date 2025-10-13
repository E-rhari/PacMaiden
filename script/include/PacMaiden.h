#include<raylib.h>
#include<stdbool.h>

#include "./Character.h"

#pragma once


typedef struct {
    Character chara;
    int lifes;
    int ponits;
} PacMaiden;


PacMaiden initPacMaiden(Vector2 position, int speed, float width, float height, Color color, int lifes, int points){
    Rectangle characterRec = {position.x, position.y, width, height};
    Character chara = (Character){characterRec, speed, color};
    return (PacMaiden){chara, lifes, points};
}