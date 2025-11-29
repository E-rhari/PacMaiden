/** @brief Estado de jogo em que ocorre a gameplay propriamente dita. */
typedef enum GameState{
    STARTING,
    RUNNING,
    PAUSED,
    SAVING,
    LOADING,
    EXIT
}GameState;


GameState gameState;

#include "../Characters/Character.h"
#include "../Characters/PacMaiden.h"
#include "../Characters/Ghosts/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Input.h"
#include "../System/Audio.h"
#include "../Map/Map.h"
#include "Menu.h"

#include "../Characters/Ghosts/StalkerGhost.h"
#include "../Characters/Ghosts/AwareGhost.h"
#include "../Characters/Ghosts/UnawareGhost.h"
#include "../Characters/Ghosts/AmbusherGhost.h"

#pragma once




Ghost* instantiateGhostsInLevel(Map map){
    Ghost *ladies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    ladies[0] = initGhost(positions[0], RADIUS, SPEED, RED, AMBUSHER); //homura
    ladies[1] = initGhost(positions[1], RADIUS, SPEED, SKYBLUE, AMBUSHER);//sora
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
    if(gameState==SAVING || gameState==LOADING)
        drawSaveStates(saveOptions);
    if(gameState==STARTING)
        DrawText("TAB para pular", LARGURA/2 - 80, ALTURA+15, 20, RAYWHITE);
        
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


void startCutscene(PacMaiden* pacmaiden, Ghost* ghosts, Map map, OptionButton* buttons, Rectangle *saveOptions){
    Music startTrack = LoadMusicStream("../../audio/Music/GameStart/GameStart.wav");
    PlayMusicStream(startTrack);
    startTrack.looping = false;

    for(int i=0; i<4; i++){
        ghosts[i].chara.color.a = 0;
        ghosts[i].chara.procAnimation.running = false;
    }

    while(gameState == STARTING){
        if(DEBUG_MODE)
            userClose();
            
        UpdateMusicStream(startTrack);

        for(int i=0; i<4; i++)
            if(GetMusicTimePlayed(startTrack) >= 1.35f*i && ghosts[i].chara.color.a < 255)
                fadeIn(&(ghosts[i].chara.color), &(ghosts[i].chara.procAnimation), 2);

        draw(map, pacmaiden, ghosts, buttons, saveOptions);

        if(GetMusicTimePlayed(startTrack) >= 10 || IsKeyPressed(KEY_TAB))
            gameState = RUNNING;
    }
    for(int i=0; i<4; i++)
        ghosts[i].chara.color.a = 255;
    StopMusicStream(startTrack);
    UnloadMusicStream(startTrack);
}


/** @brief Roda todo frame. */
int update(PacMaiden* pacmaiden,Ghost* ghosts, Map map, OptionButton* buttons, Rectangle *saveOptions, Music* tracks){
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

        if(IsKeyPressed(KEY_TAB)){
            if(gameState==PAUSED)    
               gameState=RUNNING;
            else 
                gameState=PAUSED;
        }

        handleMusic(tracks, gameState!=RUNNING);

        switch (gameState)
        {
            case PAUSED:
                isOptionButtonClicked(buttons, GetKeyPressed());
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
    gameState=STARTING;

    Map map=setUpMap();
    readMap(levelNumber,map);

    PacMaiden pacmaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 3, 0);
    Ghost* ghosts = instantiateGhostsInLevel(map);
    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);
    Rectangle* saveOptions = malloc(sizeof(Rectangle)*3);
    initSaveButton(saveOptions);

    Music tracks[SONG_AMOUT];
    initiateMusic(tracks); 
    focusTrack(tracks, MAIN_THEME);

    changePacmaidenState(&pacmaiden, IMMORTAL);

    if(gameState == STARTING)
        startCutscene(&pacmaiden, ghosts, map, buttons, saveOptions);
    screen=update(&pacmaiden,ghosts,map,buttons,saveOptions, tracks);

    freeMusic(tracks);
    free(map);
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


    Music tracks[SONG_AMOUT];
    initiateMusic(tracks); 
    focusTrack(tracks, MAIN_THEME);

    screen=update(&pacmaiden,ghosts,map,buttons,saveOptions, tracks);

    freeMusic(tracks);
    freeMap(map);
    free(ghosts);
    free(saveOptions);
    free(buttons);
    return screen;
}