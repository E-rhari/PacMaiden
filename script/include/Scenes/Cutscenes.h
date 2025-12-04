#include "raylib.h"

#include "../Characters/PacMaiden.h"
#include "../Characters/Ghosts/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Audio.h"
#include "../Map/Map.h"
#include "Menu.h"


#pragma once


void drawMap(Map map);

void drawHud(PacMaiden* pacmaiden);
void drawHudPVP(PacMaiden* pacmaiden);

void drawCharacters(PacMaiden* pacmaiden, Ghost* ghosts);
void drawCharactersPVP(PacMaiden* pacmaidens, Ghost* ghosts);



void teamLogo(){
    WaitTime(1);
    Color fadeColor = BLACK;
    ProceduralAnimation fadeAnimation = {GetTime(), true};
    while(fadeAnimation.running){
        fadeOut(&fadeColor, &fadeAnimation, 2.0f);
        BeginDrawing();
        DrawText("JERB", LARGURA/2 -275, ALTURA/2 - 100, 200, RAYWHITE);
        DrawText("Jogos Eletrônicos Radicais e Brabos", LARGURA/2 -475, ALTURA/2 +75, 50, RAYWHITE);
        DrawText("João, Evandro, Renato e Binoto", LARGURA/2 -200, ALTURA/2 + 125, 25, RAYWHITE);
        DrawRectangle(0,0, LARGURA, ALTURA+ALTURAHUD, fadeColor);
        EndDrawing();

        if(IsKeyPressed(KEY_TAB)){
            fadeColor.a = 0;
            return;
        }
    }
    WaitTime(1);
    fadeAnimation = (ProceduralAnimation){GetTime(), true};
    while(fadeAnimation.running){
        fadeIn(&fadeColor, &fadeAnimation, 2.0f);
        BeginDrawing();
        DrawText("JERB", LARGURA/2 -275, ALTURA/2 - 100, 200, RAYWHITE);
        DrawText("Jogos Eletrônicos Radicais e Brabos", LARGURA/2 -475, ALTURA/2 +75, 50, RAYWHITE);
        DrawText("João, Evandro, Renato e Binoto", LARGURA/2 -200, ALTURA/2 + 125, 25, RAYWHITE);
        DrawRectangle(0,0, LARGURA, ALTURA+ALTURAHUD, fadeColor);
        
        EndDrawing();
        if(IsKeyPressed(KEY_TAB)){
            fadeColor.a = 0;
            return;
        }
    }
    WaitTime(1);
}


/** @brief Dá fade in nos fantasmas em sincronia com a música */
void gameStartCutscene(PacMaiden* pacmaiden, Ghost* ghosts, Map map, bool PVP){
    Music startTrack = LoadMusicStream(getFilePath("../../audio/Music/GameStart/GameStart.wav"));
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
        drawMap(map);
        if(PVP){
            drawCharactersPVP(pacmaiden, ghosts);
            drawHudPVP(pacmaiden);
        }
        else{
            drawCharacters(pacmaiden, ghosts);
            drawHud(pacmaiden);
        }
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
int gameOverCutscene(PacMaiden* pacmaiden, Ghost* ghosts, Map map, bool PVP){
    Music gameOverTrack = LoadMusicStream(getFilePath("../../audio/Music/GameOver/GameOver.wav"));
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
        ClearBackground(BLACK);
        drawMap(map);
        if(PVP){
            drawCharactersPVP(pacmaiden, ghosts);
            drawHudPVP(pacmaiden);
        }
        else{
            drawCharacters(pacmaiden, ghosts);
            drawHud(pacmaiden);
        }
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
        ClearBackground(BLACK);
        drawMap(map);
        if(PVP){
            drawCharactersPVP(pacmaiden, ghosts);
            drawHudPVP(pacmaiden);
        }
        else{
            drawCharacters(pacmaiden, ghosts);
            drawHud(pacmaiden);
        }
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
