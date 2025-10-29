#include "../Characters/Character.h"
#include "../Characters/PacMaiden.h"
#include "../Characters/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Input.h"
#include "../Map/Map.h"

#pragma once


Ghost* inicializeGhost(Map map){
    Ghost *laidies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    laidies[0] = initGhost(positions[0], RADIUS, SPEED, RED); //homura
    laidies[1] = initGhost(positions[1], RADIUS, SPEED, BLUE);//sora
    laidies[2] = initGhost(positions[2], RADIUS, SPEED, GOLD);//hikari
    laidies[3] = initGhost(positions[3], RADIUS, SPEED, PINK);//hana

    return laidies;
}

void draw(Map map,PacMaiden* pacMaiden,Ghost* ghosts){
    BeginDrawing();

    ClearBackground(BLACK);
    drawMap(map);
    
    DrawCircleV(pacMaiden->chara.circle.center, pacMaiden->chara.circle.radius, pacMaiden->chara.color);
    DrawRectangle(0, 800, LARGURA, (int)GRID2PIX, DARKBLUE);

    DrawText(TextFormat("Pontuação: %d", pacMaiden->points), SCOREPOSY, ALTURA, SCORESIZE, RAYWHITE);

    for(int i=0; i<4; i++)
        DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);

    EndDrawing();
}

void update(PacMaiden* pacMaiden,Ghost* ghosts, Map map){
    while(!WindowShouldClose()){
        userClose();

        getBufferedInput(&pacMaiden->chara.moveDirection, isInGridCenter(pacMaiden->chara)
                                                      && isInsideScreen(pacMaiden->chara, (Vector2){0,0}));

        move(&pacMaiden->chara, map);
        portalBorders(&pacMaiden->chara);
        countPoints(pacMaiden, map, charCollided(*pacMaiden, map));

        for(int i=0; i<4; i++){
            moveAware(&ghosts[i], map);
            portalBorders(&ghosts[i].chara);
            if(checkCharacterCollision(pacMaiden->chara, ghosts[i].chara))
                hurt(pacMaiden);
        }

        draw(map,pacMaiden,ghosts);
    }
}



int level(){
    Map map=setUpMap();
    readMap(1,map);

    PacMaiden pacMaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED+1, YELLOW, 3, 0);
    Ghost* ghosts = inicializeGhost(map);

    update(&pacMaiden,ghosts,map);

    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    return 0;
}