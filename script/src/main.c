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
#include "../include/Map/Menu.h"
#include "../include/System/SaveSystem.h"


int main(){
    InitWindow(LARGURA, ALTURAHUD, "Jogo Irado!!!");
    SetTargetFPS(60);
    srand(time(NULL));

    PacMaiden pacMaiden = initPacMaiden((Vector2){40*15, 40*10}, 20, 160, YELLOW, 3, 0);

    Ghost homura = initGhost((Vector2){40*10,40*10}, 20, 161, RED);
    Ghost sora   = initGhost((Vector2){40*5, 40*5}, 20, 162, BLUE);
    Ghost hikari = initGhost((Vector2){40*35,40*15}, 20, 163, GOLD);
    Ghost hana   = initGhost((Vector2){40*25,40*5 }, 20, 164, PINK);
    Ghost ghosts[4] = {homura, sora, hikari, hana};
    menuButton button = {(Vector2){800, 820}, 25, BUTTONBASE, BUTTONHOVER, BUTTONBAR};
    bool menuOpen = false;
    bool saveMenuOpen = false;

    Map map=setUpMap();
    readMap(1,map);

   
    while(!WindowShouldClose()){
            userClose();
            if(!menuOpen){
            getBufferedInput(&pacMaiden.chara.moveDirection, isInGridCenter(pacMaiden.chara)
                                                        && isInsideScreen(pacMaiden.chara, (Vector2){0,0}));

            move(&pacMaiden.chara, map);
            portalBorders(&pacMaiden.chara);
            countPoints(&pacMaiden, map, charCollided(pacMaiden, map));

            for(int i=0; i<4; i++){
                moveAware(&ghosts[i], map);
                portalBorders(&ghosts[i].chara);
                if(checkCharacterCollision(pacMaiden.chara, ghosts[i].chara))
                    hurt(&pacMaiden);
            }
        }

            BeginDrawing();

            ClearBackground(BLACK);
            drawMap(map);
            
            DrawCircleV(pacMaiden.chara.circle.center, pacMaiden.chara.circle.radius, pacMaiden.chara.color);
            DrawRectangle(0, 800, LARGURA, 40, DARKBLUE);

            DrawText(TextFormat("Pontuação: %d", pacMaiden.points), 10, 800, 40, RAYWHITE);
    
            for(int i=0; i<sizeof(ghosts)/sizeof(Ghost); i++)
                DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);
        
        drawMenuButton(button);
        if(drawMenu(button, &menuOpen, &saveMenuOpen)==1)
            save(map,pacMaiden,ghosts,0);
        
        EndDrawing();
    }
    
    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    return 0;
}