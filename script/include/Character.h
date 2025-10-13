#include<raylib.h>
#include<stdbool.h>

#include "./WindowControl.h"
#include "./Map.h"

#pragma once


typedef struct tChara {
    Rectangle rec;
    float speed;
    Color color;
} Character;


Character initCharacter(Vector2 position, int speed, float width, float height, Color color){
    Rectangle characterRec = {position.x, position.y, width, height};
    return (Character){characterRec, speed, color};
}


bool move(Character* character, Vector2 direction){
    Vector2 dest = (Vector2){character->rec.x + direction.x * character->speed*GetFrameTime(),
                             character->rec.y + direction.y * character->speed*GetFrameTime()};
    Vector2 movingBound = {dest.x + character->rec.width * (1.0f/2.0f + direction.x/2),
                           dest.y + character->rec.height * (1.0f/2.0f + direction.y/2)};
    Vector2 gridBound = Vector2Scale(movingBound, PIX2GRID);

    char texto[100];
    sprintf(texto, "x: %.2f, y: %.2f.", gridBound.x, gridBound.y);
    DrawText(texto, 100, 100, 50, BLACK);
    DrawCircleV(movingBound, 5, BLACK);
    
    if (mapa[(int)gridBound.y][(int)gridBound.x] == '@') return false;

    character->rec.x = dest.x;
    character->rec.y = dest.y;
    return true;
}

void portalBorders(Character* chara){
    if(chara->rec.x < 0 - chara->rec.width)
        chara->rec.x = LARGURA + chara->rec.width;
    else if(chara->rec.x > LARGURA)
        chara->rec.x = 0 - chara->rec.width;
    if(chara->rec.y < 0 - chara->rec.height)
        chara->rec.y = LARGURA + chara->rec.height;
    else if(chara->rec.y > LARGURA)
        chara->rec.y = 0 - chara->rec.height;   
}