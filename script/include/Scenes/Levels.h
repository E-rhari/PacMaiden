/** @brief Estado de jogo em que ocorre a gameplay propriamente dita. */
typedef enum GameState{
    RUNNING,
    PAUSED,
    SAVING,
    LOADING,
    EXIT
}GameState;


GameState gameState;

#include "../Characters/Character.h"
#include "../Characters/PacMaiden.h"
#include "../Characters/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Input.h"
#include "../Map/Map.h"
#include "Menu.h"

#include "../Characters/CopyGhost.h"
#include "../Characters/StupidGhost.h"

#pragma once




Ghost* instantiateGhostsInLevel(Map map){
    Ghost *ladies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    ladies[0] = initGhost(positions[0], RADIUS, SPEED, RED, COPY); //homura
    ladies[1] = initGhost(positions[1], RADIUS, SPEED, SKYBLUE, STUPID);//sora
    ladies[2] = initGhost(positions[2], RADIUS, SPEED, ORANGE, AWARE);//hikari
    ladies[3] = initGhost(positions[3], RADIUS, SPEED, PINK, AWARE);//hana

    return ladies;
}


/** @brief Desenha todas as coisas do jogo. */
void draw(Map map,PacMaiden* pacmaiden, Ghost* ghosts,OptionButton* buttons,Rectangle* saveOptions){
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

    if(gameState==PAUSED)
        drawOpenedMenu(buttons);
    if(gameState==SAVING ||gameState==LOADING)
        drawSaveStates(saveOptions);
        
    EndDrawing();
}


/** @brief Realiza todas as funções de movimento dos personagens */
void charactersBehaviours(PacMaiden* pacmaiden, Ghost* ghosts, Map map,int *pallets){
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

    countPoints(pacmaiden, map, charCollided(*pacmaiden, map),pallets);
}

bool isPacMaidenDead(PacMaiden* PacMaiden){
    if(PacMaiden->lifes<=0)
        return true;
    return false;
}

/** @brief Roda todo frame. */
int update(PacMaiden* pacmaiden,Ghost* ghosts, Map map, OptionButton* buttons,Rectangle *saveOptions){
    int fileNumber;
    int pallets = countPallets(map);

    while(!WindowShouldClose()){
        fileNumber=-1;
        if(DEBUG_MODE)
            userClose();
    
        draw(map,pacmaiden,ghosts,buttons,saveOptions);
        
        
        if(isPacMaidenDead(pacmaiden))
            return TITLE;
        if(pallets<=0)
            return NEXT;
        if(IsKeyPressed(KEY_TAB))
            gameState=PAUSED;

        switch (gameState)
        {
            case PAUSED:
                isOptionButtonClicked(buttons,GetKeyPressed());
            break;
            case RUNNING:
                charactersBehaviours(pacmaiden, ghosts, map, &pallets);
            break;
            case SAVING:
                fileNumber=isSaveFileClicked(saveOptions);
                if(fileNumber!=-1)
                    save(map,*pacmaiden,ghosts,fileNumber);
            break;
            case LOADING:
                fileNumber=isSaveFileClicked(saveOptions);
                if(fileNumber!=-1)
                    load(map,pacmaiden,ghosts,fileNumber);
            break;
            case EXIT:
                return TITLE;
            break;
        }
    }
}


/** @brief Roda a fase desejada */
int level(int levelNumber){

    int screen;
    gameState=RUNNING;

    Map map=setUpMap();
    readMap(levelNumber,map);

    PacMaiden pacmaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 3, 0);
    Ghost* ghosts = instantiateGhostsInLevel(map);
    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);
    Rectangle* saveOptions = malloc(sizeof(Rectangle)*3);
    initSaveButton(saveOptions);

    changePacmaidenState(&pacmaiden, IMMORTAL);
    screen=update(&pacmaiden,ghosts,map,buttons,saveOptions);

    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    free(ghosts);
    free(saveOptions);
    free(buttons);

    return screen;
}

int loadLevel(int levelNumber){
    int screen;
    gameState=RUNNING;


    Map map=setUpMap();
    PacMaiden pacmaiden;
    Ghost* ghosts = malloc(sizeof(Ghost)*4);

    load(map,&pacmaiden,ghosts,levelNumber);
    
    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);
    Rectangle* saveOptions = malloc(sizeof(Rectangle)*3);
    initSaveButton(saveOptions);

    screen=update(&pacmaiden,ghosts,map,buttons,saveOptions);

    free(map);
    for(int i=0;i<20;i++)
         free(*(map+i));

    free(ghosts);
    free(saveOptions);
    free(buttons);
    return screen;
}