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

void checkPacmaidenGhostCollision(PacMaiden* pacmaiden, Ghost* ghost, Map map){
    if(pacmaiden->state!=IMMORTAL){
        if(checkCharacterCollision(pacmaiden->chara, ghost->chara))
            hurt(pacmaiden, map);
    }
    else{
        blinkAnimation(&pacmaiden->chara.color, YELLOW, WHITE, &pacmaiden->chara.procAnimation, HURT_COOLDOWN, 2);
        if(!pacmaiden->chara.procAnimation.running)
            changeState(pacmaiden, NORMAL);
    }
}


void draw(Map map,PacMaiden* pacmaiden, Ghost* ghosts){
    BeginDrawing();

    ClearBackground(BLACK);
    drawMap(map);
    
    DrawCircleV(pacmaiden->chara.circle.center, pacmaiden->chara.circle.radius, pacmaiden->chara.color);

    DrawRectangle(0, 800, LARGURA, (int)GRID2PIX, BLACK);
    DrawText(TextFormat("Pontuação: %d", pacmaiden->points), SCOREPOSY, ALTURA, SCORESIZE, RAYWHITE);
    for(int i=0; i<pacmaiden->lifes; i++)
        DrawCircle(LARGURA-(i+1)*(20)-(i*20), ALTURA+20, 20, pacmaiden->initialValues.color);

    for(int i=0; i<4; i++)
        DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);

    EndDrawing();
}


void moveCharacters(PacMaiden* pacmaiden, Ghost* ghosts, Map map){
    if(pacmaiden->state != DYING){
        getBufferedInput(&pacmaiden->chara.moveDirection, isInGridCenter(pacmaiden->chara)
                                                    && isCharacterInsideScreen(pacmaiden->chara, (Vector2){0,0}));

        move(&pacmaiden->chara, map);
        portalBorders(&pacmaiden->chara);
        countPoints(pacmaiden, map, charCollided(*pacmaiden, map));

        for(int i=0; i<4; i++){
            moveAware(&ghosts[i], map);
            portalBorders(&ghosts[i].chara);
            checkPacmaidenGhostCollision(pacmaiden, &ghosts[i], map);
        }
    }
    else{
        fadeOut(&pacmaiden->chara.color, &pacmaiden->chara.procAnimation, 3);
        if(!pacmaiden->chara.procAnimation.running)
            changeState(pacmaiden, IMMORTAL);
    }
}


void update(PacMaiden* pacmaiden,Ghost* ghosts, Map map){
    while(!WindowShouldClose()){
        userClose();

        moveCharacters(pacmaiden, ghosts, map);

        draw(map,pacmaiden,ghosts);
    }
}

int level(){
    Map map=setUpMap();
    readMap(1,map);

    PacMaiden pacmaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 3, 0);
    Ghost* ghosts = inicializeGhost(map);

    update(&pacmaiden,ghosts,map);

    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    return 0;
}