#include "raylib.h"
#include"raymath.h"
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

#include "../include/Characters/Character.h"
#include "../include/Characters/PacMaiden.h"
#include "../include/Characters/Ghost.h"
#include "../include/System/WindowControl.h"
#include "../include/System/Input.h"
#include "../include/Map/Map.h"


int main(){
    InitWindow(LARGURA, ALTURA, "Jogo Irado!!!");
    SetTargetFPS(60);
    srand(time(NULL));

    PacMaiden pacMaiden = initPacMaiden((Vector2){40*15, 40*10}, 20, 160, YELLOW, 3, 0);

    Ghost homura = initGhost((Vector2){40*10,40*10}, 20, 160, RED);
    Ghost sora   = initGhost((Vector2){40*5, 40*5}, 20, 160, BLUE);
    Ghost hikari = initGhost((Vector2){40*35,40*15}, 20, 160, GOLD);
    Ghost hana   = initGhost((Vector2){40*25,40*5 }, 20, 160, PINK);
    Ghost ghosts[4] = {homura, sora, hikari, hana};

    Map map=setUpMap();
    readMap(1,map);


    while(!WindowShouldClose()){
        userClose();

        getBufferedInput(&pacMaiden.chara.moveDirection, isInGridCenter(pacMaiden.chara)
                                                      && isInsideScreen(pacMaiden.chara, (Vector2){0,0}));

        move(&pacMaiden.chara, map);
        portalBorders(&pacMaiden.chara);

        for(int i=0; i<sizeof(ghosts)/sizeof(Ghost); i++){
            moveAware(&ghosts[i], map);
            portalBorders(&ghosts[i].chara);
            if(checkCharacterCollision(pacMaiden.chara, ghosts[i].chara))
                hurt(&pacMaiden);
        }


        BeginDrawing();

        ClearBackground(BLACK);
        drawMap(map);
        
        DrawCircleV(pacMaiden.chara.circle.center, pacMaiden.chara.circle.radius, pacMaiden.chara.color);
        for(int i=0; i<sizeof(ghosts)/sizeof(Ghost); i++)
            DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);

        EndDrawing();
    }
    
    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    return 0;
}