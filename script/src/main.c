#include "raylib.h"
#include"raymath.h"
#include<string.h>
#include<stdbool.h>

#include "../include/Characters/Character.h"
#include "../include/Characters/PacMaiden.h"
#include "../include/System/WindowControl.h"
#include "../include/System/Input.h"
#include "../include/Map/Map.h"


int main(){
    InitWindow(LARGURA, ALTURA, "Jogo Irado!!!");
    SetTargetFPS(60);

    PacMaiden pacMaiden = initPacMaiden((Vector2){400, 400}, 20, 100, YELLOW, 3, 0);
    Vector2 direction = {0,0};

    Map map=setUpMap();

    readMap(1,map);


    while(!WindowShouldClose()){
        userClose();

        getBufferedInput(&direction, (int)(pacMaiden.chara.circle.center.x+pacMaiden.chara.circle.radius)%40 < 2 
                                  && (int)(pacMaiden.chara.circle.center.y+pacMaiden.chara.circle.radius)%40 < 2);


        move(&pacMaiden.chara, direction, map);
        portalBorders(&pacMaiden.chara);


        BeginDrawing();

        ClearBackground(BLACK);
        drawMap(map);
        DrawCircleV(pacMaiden.chara.circle.center, pacMaiden.chara.circle.radius, pacMaiden.chara.color);
 
        EndDrawing();
    }
    
    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    return 0;
}