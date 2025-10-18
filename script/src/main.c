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

    PacMaiden pacMaiden = initPacMaiden((Vector2){40*15, 40*10}, 20, 160, YELLOW, 3, 0);

    Map map=setUpMap();

    readMap(1,map);


    while(!WindowShouldClose()){
        userClose();

        getBufferedInput(&pacMaiden.chara.moveDirection, (int)(pacMaiden.chara.circle.center.x+pacMaiden.chara.circle.radius)%40 < 3 
                                                      && (int)(pacMaiden.chara.circle.center.y+pacMaiden.chara.circle.radius)%40 < 3);

        move(&pacMaiden.chara, map);
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