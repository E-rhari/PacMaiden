/** @brief Estado de jogo em que ocorre a gameplay propriamente dita. */


#include "../Characters/Character.h"
#include "../Characters/PacMaiden.h"
#include "../Characters/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Input.h"
#include "../Map/Map.h"

#pragma once



Ghost* instanciateGhostsInLevel(Map map){
    Ghost *laidies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    laidies[0] = initGhost(positions[0], RADIUS, SPEED, RED); //homura
    laidies[1] = initGhost(positions[1], RADIUS, SPEED, SKYBLUE);//sora
    laidies[2] = initGhost(positions[2], RADIUS, SPEED, ORANGE);//hikari
    laidies[3] = initGhost(positions[3], RADIUS, SPEED, PINK);//hana

    return laidies;
}


/** @brief DEsenha todas as coisas do jogo. */
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
    if(pacmaiden->state != DYING){
        getBufferedInput(&pacmaiden->chara.moveDirection, isCharacterInGridCenter(pacmaiden->chara)
                                                       && isCharacterInsideScreen(pacmaiden->chara, (Vector2){0,0}));
        pacmaidenBehaviour(pacmaiden, map);
        
        for(int i=0; i<4; i++)
            ghostBehaviour(&ghosts[i], map, pacmaiden);

        countPoints(pacmaiden, map, charCollided(*pacmaiden, map));
    }
    else{
        fadeOut(&pacmaiden->chara.color, &pacmaiden->chara.procAnimation, 3);
        if(!pacmaiden->chara.procAnimation.running)
            changePacmaidenState(pacmaiden, IMMORTAL);
    }
}


/** @brief Roda todo frame. */
void update(PacMaiden* pacmaiden,Ghost* ghosts, Map map){
    while(!WindowShouldClose()){
        if(DEBUG_MODE)
            userClose();

        charactersBehaviours(pacmaiden, ghosts, map);

        draw(map,pacmaiden,ghosts);
    }
}


/** @brief Roda a fase desejada */
int level(int levelNumber){
    Map map=setUpMap();
    readMap(levelNumber,map);

    PacMaiden pacmaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 3, 0);
    Ghost* ghosts = instanciateGhostsInLevel(map);

    update(&pacmaiden,ghosts,map);

    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    return 0;
}