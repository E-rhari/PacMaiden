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

#include <dirent.h>
#include <string.h>
#include <time.h>

#pragma once


Ghost* instantiateGhostsInPVP(Map map){
    Ghost *ladies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    ladies[0] = initGhost(positions[0], RADIUS, SPEED/2, RED, AWARE); //homura
    ladies[1] = initGhost(positions[1], RADIUS, SPEED/2, SKYBLUE, AWARE);//sora
    ladies[2] = initGhost(positions[2], RADIUS, SPEED/2, ORANGE, AWARE);//hikari
    ladies[3] = initGhost(positions[3], RADIUS, SPEED/2, PINK, AWARE);//hana

    return ladies;
}

int PVPMapsQuantity(){

    DIR *directory;
    struct dirent *element;
    int files = 0;
    
    #ifdef _WIN32
        directory = opendir("PacMaiden/sprites/maps/PVPMaps");

    #elif __linux__
        directory = opendir("../../sprites/maps/PVPMaps");
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

void drawPVP(Map map,PacMaiden* players, Ghost* ghosts,OptionButton* buttons){
    BeginDrawing();

    ClearBackground(BLACK);
    drawMap(map);
    DrawRectangle(0, 800, LARGURA, (int)GRID2PIX, BLACK);

    for(int j=0;j<2;j++){
        DrawCircleV(players[j].chara.circle.center, players[j].chara.circle.radius, players[j].chara.color);
        for(int i=0; i<players[j].lifes; i++)
            DrawCircle(LARGURA-(i+1)*(20)-(i*20), ALTURA+20, 20,players[j].initialValues.color);
         //DrawText(TextFormat("Pontuação: %d", pacmaiden->points), SCOREPOSY, ALTURA, SCORESIZE, RAYWHITE);
    }

    for(int i=0; i<4; i++)
        DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);

    if(gameState==PAUSED)
        drawOpenedMenu(buttons);
        
    EndDrawing();
}
void charactersPVPBehaviours(PacMaiden* players, Ghost* ghosts, Map map,int *pallets){

    for(int i=0;i<2;i++){

        if(players[i].state == DYING){
            fadeOut(&players[i].chara.color, &players[i].chara.procAnimation, 3);
            if(!players[i].chara.procAnimation.running)
                changePacmaidenState(&players[i], IMMORTAL);
            return;
        }
        
        getBufferedInput(&players[i].chara.moveDirection, isCharacterInGridCenter(players[i].chara)
                                                && isCharacterInsideScreen(players[i].chara, (Vector2){0,0}),i,&players[i].bufferedInput);

        pacmaidenBehaviour(&players[i], map);

        for(int j=0; j<4; j++)
            ghostBehaviour(&ghosts[j], map, &players[i]);

        countPoints(&players[i], map, charCollided(players[i], map),pallets);
    }
}
int updatePVP(PacMaiden* players,Ghost* ghosts, Map map, OptionButton* buttons){

    int pallets = countPallets(map);

    while(!WindowShouldClose()){
        if(DEBUG_MODE)
            userClose();
    
        drawPVP(map,players,ghosts,buttons);
        
        
        //if(isPacMaidenDead(pacmaiden))
            //return TITLE;
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
                charactersPVPBehaviours(players, ghosts, map, &pallets);
            break;
            case EXIT:
                return TITLE;
            break;
        }
    }
}

int StartPVP(){

    srand(time(NULL));

    int screen;
    bool isSinglePlayer = false;
    int level;

    level = rand() % PVPMapsQuantity();
    printf("%d",level);

    gameState=RUNNING;
    
    Map map=setUpMap();
    
    readMap(level,map,isSinglePlayer);

    PacMaiden player1 = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 3, 0);
    PacMaiden player2 = initPacMaiden(searchInMap(map, 'P')[1], RADIUS, SPEED, PURPLE, 3, 0);

    PacMaiden *players = malloc(sizeof(PacMaiden)*2);
    players[0]=player1;
    players[1]=player2;

    Ghost* ghosts = instantiateGhostsInPVP(map);
    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);

    changePacmaidenState(&players[0], IMMORTAL);
    changePacmaidenState(&players[1], IMMORTAL);
    screen=updatePVP(players,ghosts,map,buttons);

    free(map);
    for(int i=0;i<20;i++)
        free(*(map+i));

    free(ghosts);
    free(buttons);
    free(players);

    return screen;
}