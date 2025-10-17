#include "raylib.h"
#include"raymath.h"
#include<string.h>
#include<stdbool.h>

#include "../include/Character.h"
#include "../include/PacMaiden.h"
#include "../include/WindowControl.h"
#include "../include/Map/Map.h"
#include "../include/Input.h"


int main(){
    InitWindow(LARGURA, ALTURA, "Jogo Irado!!!");
    SetTargetFPS(60);

    PacMaiden pacMaiden = initPacMaiden((Vector2){400, 400}, 20, 100, YELLOW, 3, 0);
    Vector2 input = {0, 1};
    Vector2 direction = {0,0};

    // Mapa mapa=setUpMapa();
    Mapa mapa = (Mapa)malloc(sizeof(char*)*20);

    for(int i=0;i<20;i++)
        *(mapa+i) = (char*)malloc(sizeof(char)*40);

    // char mapa[20][40];

    lerMapa(1,mapa);


    while(!WindowShouldClose()){
        userClose();

        getBufferedInput(&input, (int)(pacMaiden.chara.circle.center.x+pacMaiden.chara.circle.radius)%40 < 2 
                              && (int)(pacMaiden.chara.circle.center.y+pacMaiden.chara.circle.radius)%40 < 2);

        if(!Vector2Equals(input, Vector2Zero())) 
            direction = input;

        move(&pacMaiden.chara, direction, mapa);
        portalBorders(&pacMaiden.chara);


        BeginDrawing();

        ClearBackground(BLACK);
        renderizaMapa(mapa);
        DrawCircleV(pacMaiden.chara.circle.center, pacMaiden.chara.circle.radius, pacMaiden.chara.color);

 
        EndDrawing();
    }
    
    free(mapa);
    for(int i=0;i<20;i++)
        free(*(mapa+i));

    return 0;
}