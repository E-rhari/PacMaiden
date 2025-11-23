/** @brief Estado de jogo em que ocorre a gameplay propriamente dita. */


#include "../Characters/Character.h"
#include "../Characters/PacMaiden.h"
#include "../Characters/Ghosts/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Input.h"
#include "../Map/Map.h"

#include "../Characters/Ghosts/StalkerGhost.h"
#include "../Characters/Ghosts/AwareGhost.h"
#include "../Characters/Ghosts/UnawareGhost.h"

#pragma once



Ghost* instantiateGhostsInLevel(Map map){
    Ghost *ladies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    ladies[0] = initGhost(positions[0], RADIUS, SPEED, RED, STALKER); //homura
    ladies[1] = initGhost(positions[1], RADIUS, SPEED, SKYBLUE, AWARE);//sora
    ladies[2] = initGhost(positions[2], RADIUS, SPEED, ORANGE, STALKER);//hikari
    ladies[3] = initGhost(positions[3], RADIUS, SPEED, PINK, STALKER);//hana

    return ladies;
}


/** @brief Desenha todas as coisas do jogo. */
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


/** @brief Realiza todas as funções de movimento dos personagens */
void charactersBehaviours(PacMaiden* pacmaiden, Ghost* ghosts, Map map){
    if(pacmaiden->state == DYING){
        fadeOut(&pacmaiden->chara.color, &pacmaiden->chara.procAnimation, 3);
        if(!pacmaiden->chara.procAnimation.running)
            changePacmaidenState(pacmaiden, IMMORTAL);
        return;
    }

    getBufferedInput(&pacmaiden->chara.moveDirection, isCharacterInGridCenter(pacmaiden->chara)
                                                   && isCharacterInsideScreen(pacmaiden->chara, (Vector2){0,0}));
    pacmaidenBehaviour(pacmaiden, map);
    for(int i=0; i<4; i++)
        ghostBehaviour(&ghosts[i], map, pacmaiden);

    countPoints(pacmaiden, map, charCollided(*pacmaiden, map));
}

bool isPacMaidenDead(PacMaiden* PacMaiden){
    if(PacMaiden->lifes<=0)
        return true;
    return false;
}

/** @brief Roda todo frame. */
int update(PacMaiden* pacmaiden,Ghost* ghosts, Map map){
    while(!WindowShouldClose()){
        if(DEBUG_MODE)
            userClose();

        charactersBehaviours(pacmaiden, ghosts, map);

        draw(map,pacmaiden,ghosts);
        if(isPacMaidenDead(pacmaiden))
            return TITLE;
    }
}


/** @brief Roda a fase desejada */
int level(int levelNumber){
    int screen;
    Map map=setUpMap();
    readMap(levelNumber,map);

    PacMaiden pacmaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 3, 0);
    Ghost* ghosts = instantiateGhostsInLevel(map);

    screen=update(&pacmaiden,ghosts,map);

    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    return screen;
}