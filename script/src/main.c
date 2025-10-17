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

    Mapa mapa=setUpMapa();

    lerMapa(1,mapa);


    while(!WindowShouldClose()){
        userClose();

        int x=getBufferedInput(&direction, (int)(pacMaiden.chara.circle.center.x+pacMaiden.chara.circle.radius)%40 < 2 
                              && (int)(pacMaiden.chara.circle.center.y+pacMaiden.chara.circle.radius)%40 < 2);


        move(&pacMaiden.chara, direction, mapa);
        portalBorders(&pacMaiden.chara);


        BeginDrawing();

        ClearBackground(BLACK);
        renderizaMapa(mapa);
        DrawCircleV(pacMaiden.chara.circle.center, pacMaiden.chara.circle.radius, pacMaiden.chara.color);
        
        if(DEBUG_MODE){
            char txt[5];
            sprintf(txt,"%d",x);
            DrawText(txt,300,300,200,PINK);
        }
 
        EndDrawing();
    }
    
    free(mapa);
    for(int i=0;i<20;i++)
        free(*(mapa+i));

    return 0;
}