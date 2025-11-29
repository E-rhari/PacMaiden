/** @brief Estado de jogo em que ocorre a gameplay propriamente dita. */
typedef enum GameState{
    STARTING,
    RUNNING,
    PAUSED,
    SAVING,
    LOADING,
    GAMEOVER,
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

    ladies[0] = initGhost(positions[0], RADIUS, SPEED, RED, STALKER); //homura
    ladies[1] = initGhost(positions[1], RADIUS, SPEED, SKYBLUE, AMBUSHER);//sora
    ladies[2] = initGhost(positions[2], RADIUS, SPEED, ORANGE, AWARE);//hikari
    ladies[3] = initGhost(positions[3], RADIUS, SPEED, PINK, AWARE);//hana

    return ladies;
}


/** @brief Desenha todas as coisas do jogo. */
void drawLevel(Map map,PacMaiden* pacmaiden, Ghost* ghosts,OptionButton* buttons,Rectangle* saveOptions){
    // Mapa
    ClearBackground(BLACK);
    drawMap(map);
    
    // Personagens
    DrawCircleV(pacmaiden->chara.circle.center, pacmaiden->chara.circle.radius, pacmaiden->chara.color);

    for(int i=0; i<4; i++)
        DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);

    // Hud
    DrawRectangle(0, 800, LARGURA, (int)GRID2PIX, BLACK);
    DrawText(TextFormat("Pontuação: %d", pacmaiden->points), SCOREPOSY, ALTURA, SCORESIZE, RAYWHITE);
    for(int i=0; i<pacmaiden->lifes; i++)
        DrawCircle(LARGURA-(i+1)*(20)-(i*20), ALTURA+20, 20, pacmaiden->initialValues.color);

    // Menu
    if(gameState==PAUSED)
        drawOpenedMenu(buttons);
    if(gameState==SAVING || gameState==LOADING)
        drawSaveStates(saveOptions);
}

/** @brief Desenha todas as coisas do jogo e fecha a rotina de desenhar, impedindo modificações futuras na tela sem apagá-la por completo */
void restricDrawLevel(Map map,PacMaiden* pacmaiden, Ghost* ghosts,OptionButton* buttons,Rectangle* saveOptions){
    BeginDrawing();
    drawLevel(map, pacmaiden, ghosts, buttons, saveOptions);
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

    countPoints(pacmaiden, map, charCollided(*pacmaiden, map), pallets);
}


/** @brief Dá fade in nos fantasmas em sincronia com a música */
void gameStartCutscene(PacMaiden* pacmaiden, Ghost* ghosts, Map map, OptionButton* buttons, Rectangle *saveOptions){
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

        BeginDrawing();
        drawLevel(map, pacmaiden, ghosts, buttons, saveOptions);
        DrawText("TAB para pular", LARGURA/2 - 80, ALTURA+15, 20, RAYWHITE);
        EndDrawing();

        if(!IsMusicStreamPlaying(startTrack) || IsKeyPressed(KEY_TAB))
            gameState = RUNNING;
    }
    for(int i=0; i<4; i++)
        ghosts[i].chara.color.a = 255;
    pacmaiden->timePivot = GetTime();
    StopMusicStream(startTrack);
    UnloadMusicStream(startTrack);
}


/** @brief Escreve a mensagem de fim de jogo e toca a música de derrota */
int gameOverCutscene(PacMaiden* pacmaiden, Ghost* ghosts, Map map, OptionButton* buttons, Rectangle *saveOptions){
    Music gameOverTrack = LoadMusicStream("../../audio/Music/GameOver/GameOver.wav");
    PlayMusicStream(gameOverTrack);
    gameOverTrack.looping = false;
    
    BeginDrawing();
    DrawText("TAB para pular", LARGURA/2 - 80, ALTURA+15, 20, RAYWHITE);
    EndDrawing();

    WaitTime(0.5);

    Color messageColorRec = BLACK;
    Color messageColorText = RAYWHITE;
    ProceduralAnimation messageRecAnimation = {GetTime(), true};
    ProceduralAnimation messageTextAnimation = {GetTime(), true};

    while(GetMusicTimePlayed(gameOverTrack) < 5.0f){
        UpdateMusicStream(gameOverTrack);

        if(messageRecAnimation.running)
            fadeIn(&messageColorRec, &messageRecAnimation, 5);
        if(messageTextAnimation.running)
            fadeIn(&messageColorText, &messageTextAnimation, 5);

        BeginDrawing();
        drawLevel(map, pacmaiden, ghosts, buttons, saveOptions);
        DrawRectangle(0, ALTURA/2 - 100, LARGURA, 200, messageColorRec);
        DrawText("Se Fodeu", LARGURA/2 - 400, ALTURA/2 - 100, 200, messageColorText);
        DrawText("TAB para pular", LARGURA/2 - 80, ALTURA+15, 20, RAYWHITE);
        EndDrawing();

        if(IsKeyPressed(KEY_TAB)){
            StopMusicStream(gameOverTrack);
            break;
        }
    }
    
    messageColorRec.a = 255;
    messageColorText.a = 255;

    Color fadeOutColor = BLACK;
    fadeOutColor.a = 0;
    ProceduralAnimation fadeOutAnimation = {GetTime(), true};

    while(fadeOutAnimation.running){
        UpdateMusicStream(gameOverTrack);

        fadeIn(&fadeOutColor, &fadeOutAnimation, 2.0f);

        BeginDrawing();
        drawLevel(map, pacmaiden, ghosts, buttons, saveOptions);
        DrawRectangle(0, ALTURA/2 - 100, LARGURA, 200, messageColorRec);
        DrawText("Se Fodeu", LARGURA/2 - 400, ALTURA/2 - 100, 200, messageColorText);
        DrawRectangle(0,0, LARGURA, ALTURA+ALTURAHUD, fadeOutColor);
        EndDrawing();
    }

    while(IsMusicStreamPlaying(gameOverTrack))
        UpdateMusicStream(gameOverTrack);

    StopMusicStream(gameOverTrack);

    return TITLE;
}


/** @brief Roda todo frame. */
void update(PacMaiden* pacmaiden,Ghost* ghosts, Map map, OptionButton* buttons, Rectangle *saveOptions, Music* tracks){
    int fileNumber;
    int pallets = countPallets(map);
    printf("\n\n\nUpdate(%.2f, %.2f)\n\n\n", pacmaiden->chara.circle.center.x,pacmaiden->chara.circle.center.y);
    while(!WindowShouldClose()){
        fileNumber=-1;
        if(DEBUG_MODE)
            userClose();

        restricDrawLevel(map, pacmaiden, ghosts, buttons, saveOptions);

        if(pacmaiden->state == DEAD){
            gameState = GAMEOVER;
            changeScreenState(TITLE);
            return;
        }
        if(pallets<=0){
            changeScreenState(NEXT);
            return;
        }


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
                changeScreenState(TITLE);
                return;
            break;
        }
    }
}


/** @brief Roda a fase desejada */
void level(int levelNumber){

    PacMaiden pacmaiden;
    Ghost* ghosts;
    Map map=setUpMap();

    if(currentScreen==LOAD){
        gameState = RUNNING;
        ghosts = malloc(sizeof(Ghost)*4);
        load(map, &pacmaiden, ghosts, levelNumber);
    }
    else if(currentScreen==NEWGAME){
        gameState=STARTING;
        readMap(levelNumber,map);
        pacmaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 1, 0);
        ghosts = instantiateGhostsInLevel(map);
        changePacmaidenState(&pacmaiden, IMMORTAL);
    }

    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);
    Rectangle* saveOptions = malloc(sizeof(Rectangle)*3);
    initSaveButton(saveOptions);

    Music tracks[SONG_AMOUT];
    initiateMusic(tracks); 
    focusTrack(tracks, MAIN_THEME);

    if(gameState == STARTING)
        gameStartCutscene(&pacmaiden, ghosts, map, buttons, saveOptions);
    update(&pacmaiden,ghosts,map,buttons,saveOptions, tracks);
    if(gameState == GAMEOVER)
        gameOverCutscene(&pacmaiden, ghosts, map, buttons, saveOptions);

    freeMusic(tracks);
    freeMap(map);
    free(ghosts);
    free(saveOptions);
    free(buttons);
}
