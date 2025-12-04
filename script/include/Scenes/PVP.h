GameState gameState;

#include "../Characters/Character.h"
#include "../Characters/PacMaiden.h"
#include "../Characters/Ghosts/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Input.h"
#include "../System/Audio.h"
#include "../Map/Map.h"
#include "Menu.h"
#include "Levels.h"

#include <dirent.h>
#include <string.h>
#include <time.h>

#pragma once


Ghost* instantiateGhostsInPVP(Map map){
    Ghost *ladies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    ladies[0] = initGhost(positions[0], RADIUS, 0, RED, AWARE); //homura
    ladies[1] = initGhost(positions[1], RADIUS, 0, SKYBLUE, AWARE);//sora
    ladies[2] = initGhost(positions[2], RADIUS, 0, ORANGE, AWARE);//hikari
    ladies[3] = initGhost(positions[3], RADIUS, 0, PINK, AWARE);//hana

    return ladies;
}

int PVPMapsQuantity(){

    DIR *directory;
    struct dirent *element;
    int files = 0;
    
    #ifdef _WIN32
        directory = opendir("PacMaiden/sprites/maps");

    #elif __linux__
        directory = opendir("../../sprites/maps");
    #endif

    if(!directory){
        printf("Diretório não encontrado");
        return -1;
    }

    while((element = readdir(directory))!=NULL){
        int len = strlen(element->d_name);
        if(strcmp(element->d_name+(len-3),"txt")==0)
            files++;
    }
    free(directory);
    free(element);
    return files;
}


void drawHudPVP(PacMaiden* players){
    for(int i=0; i<players[0].lifes; i++)
        DrawCircle(LARGURA-(i+1)*(20)-(i*20), ALTURA+20, 20,players[0].initialValues.color);
    for(int i=0; i<players[1].lifes; i++)
        DrawCircle((i+1)*(20)+(i*20), ALTURA+20, 20,players[1].initialValues.color);
    DrawRectangle(0, 800, LARGURA, (int)GRID2PIX, BLACK);
    DrawText(TextFormat("Pontuação: %d", players[1].points),SCOREPOSY*14, ALTURA, SCORESIZE, RAYWHITE);
    DrawText(TextFormat("Pontuação: %d", players[0].points),LARGURA-SCOREPOSY*45, ALTURA, SCORESIZE, RAYWHITE);
}


void drawCharactersPVP(PacMaiden* players, Ghost* ghosts){
    for(int j=0;j<2;j++)
        DrawCircleV(players[j].chara.circle.center, players[j].chara.circle.radius, players[j].chara.color);
    for(int i=0; i<4; i++)
        DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);
}


void drawPVP(Map map,PacMaiden* players, Ghost* ghosts, OptionButton* buttons){
    BeginDrawing();

    ClearBackground(BLACK);
    drawMap(map);

    drawCharactersPVP(players, ghosts);
    drawHudPVP(players);
    
    if(gameState==PAUSED)
        drawOpenedMenu(buttons);
        
    EndDrawing();
}

void PVPinteractions(PacMaiden* players){
    int playerArrayPosition;

    for(int i=0;i<2;i++){
        if(players[i].state==KILLER)
            killerTime(&players[i],5);

        playerArrayPosition=(i+1)%2;

        if(checkCharacterCollision(players[0].chara,players[1].chara)){
            if(players[i].state==KILLER && players[playerArrayPosition].state==NORMAL){
                hurtPacmaiden(&players[playerArrayPosition]);
                addPoints(&players[i],400);
                addPoints(&players[playerArrayPosition],-400);
            }

            players[i].canMove=false;
        }
    }
    canPlayersMove(players);
}

void charactersPVPBehaviours(PacMaiden* players, Ghost* ghosts, Map map,int *pallets, Sound* effects){

    for(int i=0;i<2;i++){

        if(players[i].state == DYING){
            fadeOut(&players[i].chara.color, &players[i].chara.procAnimation,1);
            if(!players[i].chara.procAnimation.running)
                changePacmaidenState(&players[i], IMMORTAL);
        
        }

        getBufferedInput(&players[i].chara.moveDirection, isCharacterInGridCenter(players[i].chara)
                                                && isCharacterInsideScreen(players[i].chara, (Vector2){0,0}),i,&players[i].bufferedInput);

        PVPinteractions(players);

        pacmaidenBehaviour(&players[i], map);
        for(int j=0; j<4; j++)
            ghostBehaviour(&ghosts[j], map, &players[i], effects[EAT_GHOST]);

        countPoints(&players[i], map, charCollided(players[i], map), pallets, effects);
    }   
}

bool isPlayersDead(PacMaiden* players){
    for(int i=0;i<2;i++)
        if(players[i].lifes<=0)
            return true;
    return false;
}


int updatePVP(PacMaiden* players,Ghost* ghosts, Map map, OptionButton* buttons, Sound* effects){

    int pallets = countPallets(map);

    while(!WindowShouldClose()){
        if(DEBUG_MODE)
            userClose(); 
    
        drawPVP(map,players,ghosts,buttons);
        
        
        if(isPlayersDead(players))
            return TITLE;
        if(pallets<=0)
            return TITLE;
        if(IsKeyPressed(KEY_TAB))
            gameState=PAUSED;

        switch (gameState)
        {
            case PAUSED:
                isOptionButtonClicked(buttons,GetKeyPressed());
            break;
            case RUNNING:
                charactersPVPBehaviours(players, ghosts, map, &pallets, effects);
            break;
            case EXIT:
                return TITLE;
            break;
        }
    }
}

void StartPVP(){
    srand(time(NULL));

    bool isSinglePlayer = false;
    int level,player1Spawn,player2Spawn;

    level = rand() % PVPMapsQuantity();

    gameState=RUNNING;
    
    Map map=setUpMap();
    readMap(level,map);

    player2Spawn = rand() % countPallets(map);
    do{
        player1Spawn = rand() % countPallets(map);
    }while(player2Spawn==player1Spawn);

    PacMaiden player1 = initPacMaiden(searchInMap(map, '.')[0], RADIUS, SPEED, YELLOW, 3, 0);
    PacMaiden player2 = initPacMaiden(searchInMap(map, '.')[player2Spawn], RADIUS, SPEED, GREEN, 3, 0);

    PacMaiden *players = malloc(sizeof(PacMaiden)*2);
    players[0]=player1;
    players[1]=player2;

    Ghost* ghosts = instantiateGhostsInPVP(map);
    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);

    changePacmaidenState(&players[0], IMMORTAL);
    changePacmaidenState(&players[1], IMMORTAL);
    
    Sound effects[SOUND_AMOUNT];
    initiateSFX(effects);

    updatePVP(players,ghosts,map,buttons, effects);

    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    free(ghosts);
    free(buttons);
    free(players);
}