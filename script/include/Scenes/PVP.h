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
    Vector2* positions = searchInMap(map, 'F');

    ladies[0] = initGhost(positions[0], RADIUS, SPEED/2, RED, STALKER); //homura
    ladies[1] = initGhost(positions[1], RADIUS, SPEED/2, SKYBLUE, AWARE);//sora
    ladies[2] = initGhost(positions[2], RADIUS, SPEED/2, ORANGE, AWARE);//hikari
    ladies[3] = initGhost(positions[3], RADIUS, SPEED/2, PINK, AMBUSHER);//hana

    return ladies;
}
/** @brief conta a quantidade de mapas que existem*/
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

    DrawRectangle(0, 800, WIDTH, (int)GRID2PIX, BLACK);
    
    for(int i=0; i<players[0].lifes; i++)
        DrawCircle(WIDTH-(i+1)*(20)-(i*20), HEIGHT+20, 20,players[0].initialValues.color);
    for(int i=0; i<players[1].lifes; i++)
        DrawCircle((i+1)*(20)+(i*20), HEIGHT+20, 20,players[1].initialValues.color);

    DrawText(TextFormat("Pontuação: %d", players[1].points),SCOREPOSY*14, HEIGHT, SCORESIZE, RAYWHITE);
    DrawText(TextFormat("Pontuação: %d", players[0].points),WIDTH-SCOREPOSY*45, HEIGHT, SCORESIZE, RAYWHITE);
}


void drawCharactersPVP(PacMaiden* players, Ghost* ghosts){
    for(int j=0;j<2;j++)
        drawCharacterSprite(&players[j].chara);
    for(int i=0; i<4; i++)
        drawCharacterSprite(&ghosts[i].chara);
}

/** @brief Desenha todos os elementos em pvp*/
void drawPVP(Map map, Vector2** mapCellPosInSprite, PacMaiden* players, Ghost* ghosts, OptionButton* buttons){
    BeginDrawing();

    ClearBackground(BLACK);
    drawMap(map, mapCellPosInSprite);

    drawCharactersPVP(players, ghosts);
    drawHudPVP(players);
    
    if(gameState==PAUSED)
        drawOpenedMenu(buttons);
        
    EndDrawing();
}

void PVPinteractions(PacMaiden* players, Sound* effects){
    int playerArrayPosition;

    for(int i=0;i<2;i++){
        if(players[i].state==KILLER)
            killerTime(&players[i],5);

        playerArrayPosition=(i+1)%2;

        if(checkCharacterCollision(players[0].chara,players[1].chara)){
            if(players[i].state==KILLER && players[playerArrayPosition].state==NORMAL){
                hurtPacmaiden(&players[playerArrayPosition], effects[DEATH]);
                addPoints(&players[i],400);
                addPoints(&players[playerArrayPosition],-400);
            }

            players[i].canMove=false;
        }
    }
    canPlayersMove(players);
}
/** @brief rege as ações que os personagens possuem no pvp*/
void charactersPVPBehaviours(PacMaiden* players, Ghost* ghosts, Map map,int *pallets, Sound* effects){

    for(int i=0;i<2;i++){

        if(players[i].state == DYING){
            fadeOut(&players[i].chara.color, &players[i].chara.procAnimation,0);
            if(!players[i].chara.procAnimation.running){
                changePacmaidenState(&players[i], IMMORTAL);
            }
        }

        getBufferedInput(&players[i].chara.moveDirection, isCharacterInGridCenter(players[i].chara)
                                                && isCharacterInsideScreen(players[i].chara, (Vector2){0,0}),i,&players[i].bufferedInput);

        PVPinteractions(players, effects);

        pacmaidenBehaviour(&players[i], map);
        for(int j=0; j<4; j++){
            chooseDestinationByType(&ghosts[j], map, players);
            ghostBehaviour(&ghosts[j], map, &players[i], effects);
        }
        countPoints(&players[i], map, charCollided(players[i], map), pallets, effects);
    }   
}

bool isPlayersDead(PacMaiden* players){
    for(int i=0;i<2;i++)
        if(players[i].lifes<=0){
            players[i].points=0;
            return true;
        }
    return false;
}


void updatePVP(PacMaiden* players, Vector2** mapCellPosInSprite, Ghost* ghosts, Map map, OptionButton* buttons, Music* tracks, Sound* effects){

    int pallets = countPallets(map);

    while(!WindowShouldClose()){
        if(DEBUG_MODE)
            userClose(); 
    
        drawPVP(map, mapCellPosInSprite, players,ghosts,buttons);
        
        
        if(isPlayersDead(players)){
            gameState = GAMEOVER;
            changeScene(TITLE);
            return;
        }
        if(pallets<=0){
            changeScene(TITLE);
            return;
        }

        gamePause();
        handleMusic(tracks, gameState!=RUNNING);

        switch (gameState)
        {
            case PAUSED:
                isOptionButtonClicked(buttons,GetKeyPressed());
            break;
            case RUNNING:
                charactersPVPBehaviours(players, ghosts, map, &pallets, effects);
            break;
            case EXIT:
                changeScene(TITLE);
                return;
            break;
        }
    }
}
/** @brief Inicializa os elementos utilizados no pvp*/
void StartPVP(){
    srand(time(NULL));

    bool isSinglePlayer = false;
    int level,player1Spawn,player2Spawn;

    level = rand() % PVPMapsQuantity();

    gameState=STARTING;
    
    Map map=setUpMap();

    readMap(level,map);

    player2Spawn = rand() % countPallets(map);
    do{
        player1Spawn = rand() % countPallets(map);
    }while(player2Spawn==player1Spawn);

    PacMaiden player1 = initPacMaiden(searchInMap(map, '.')[0], RADIUS, SPEED+1, YELLOW, 3, 0,YELLOW_PACMAIDEN_SPRITE);
    PacMaiden player2 = initPacMaiden(searchInMap(map, '.')[player2Spawn], RADIUS, SPEED, PURPLE, 3, 0,PURPLE_PACMAIDEN_SPRITE);

    PacMaiden *players = malloc(sizeof(PacMaiden)*2);
    players[0]=player1;
    players[1]=player2;

    Ghost* ghosts = instantiateGhostsInPVP(map);
    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);

    changePacmaidenState(&players[0], IMMORTAL);
    changePacmaidenState(&players[1], IMMORTAL);

    Vector2** mapCellPosInSprite = decideMapCellsSprite(map);

    Music tracks[SONG_AMOUT];
    initiateMusic(tracks); 
    focusTrack(tracks, MAIN_THEME);
    
    Sound effects[SOUND_AMOUNT];
    initiateSFX(effects);

    if(gameState == STARTING)
        gameStartCutscene(players, mapCellPosInSprite, ghosts, map, true);
    updatePVP(players, mapCellPosInSprite, ghosts,map,buttons, tracks, effects);
    if(gameState == GAMEOVER)
        gameOverCutscene(players, mapCellPosInSprite, ghosts, map, true);


    if(gameState!=EXIT)
        winPVPCutscene(players);

    for(int i=0;i<20;i++)
        free(*(map+i));
    free(map);

    freeMusic(tracks);
    freeSFX(effects);
    freePacmaiden(&player1);
    freePacmaiden(&player2);
    freeGhosts(ghosts, 4);
    free(buttons);
    free(players);
}