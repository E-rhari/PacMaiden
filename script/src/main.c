#include<raylib.h>
#include<raymath.h>
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

    Mapa mapa=setUpMapa();
    lerMapa(1,mapa);
    

    while(!WindowShouldClose()){
        userClose();

        getBufferedInput(&input, (int)(pacMaiden.chara.circle.center.x+pacMaiden.chara.circle.radius)%40 < 2 
                              && (int)(pacMaiden.chara.circle.center.y+pacMaiden.chara.circle.radius)%40 < 2);

        if(!Vector2Equals(input, Vector2Zero())) 
            direction = input;

        move(&pacMaiden.chara, direction, mapa);
        portalBorders(&pacMaiden.chara);

        printf("\n ----------- Chirashizushi ------------- \n");


        BeginDrawing();

        ClearBackground(RAYWHITE);
        renderizaMapa(mapa);
        
        DrawCircleV(pacMaiden.chara.circle.center, pacMaiden.chara.circle.radius, pacMaiden.chara.color);

        if(DEBUG_MODE)
            for(int i=0; i<LARGURA/40; i++)
                for(int j=0; j<ALTURA/40; j++)
                    DrawCircle(i*40, j*40, 3, BLACK);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}