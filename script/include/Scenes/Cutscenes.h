#include "raylib.h"

#include "../Characters/PacMaiden.h"
#include "../Characters/Ghosts/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Audio.h"
#include "../Map/Map.h"
#include "Menu.h"

#include "../Animations/SpriteAnimation.h"

#pragma once


void drawHud(PacMaiden* pacmaiden);
void drawHudPVP(PacMaiden* pacmaiden);

void drawCharacters(PacMaiden* pacmaiden, Ghost* ghosts);
void drawCharactersPVP(PacMaiden* pacmaidens, Ghost* ghosts);


static void drawTeamSplashScreen(Color fadeColor){
    ClearBackground(BLACK);
    DrawText("JERB", WIDTH/2 -275, HEIGHT/2 - 100, 200, RAYWHITE);
    DrawText("Jogos Eletrônicos Radicais e Brabos", WIDTH/2 -475, HEIGHT/2 +75, 50, RAYWHITE);
    DrawText("João, Evandro, Renato e Binoto", WIDTH/2 -200, HEIGHT/2 + 125, 25, RAYWHITE);
    DrawRectangle(0,0, WIDTH, HEIGHT+HUDHEIGHT, fadeColor);
}


/** @brief Splash sreen inicial do jogo, mostrando o nome da equipe */
void teamSplashScreen(){
    WaitTime(1);
    Color fadeColor = BLACK;
    ProceduralAnimation fadeAnimation = {GetTime(), true};

    while(fadeAnimation.running){
        fadeOut(&fadeColor, &fadeAnimation, 2.0f);
        BeginDrawing();
        drawTeamSplashScreen(fadeColor);
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
        drawTeamSplashScreen(fadeColor);
        EndDrawing();

        if(IsKeyPressed(KEY_TAB)){
            fadeColor.a = 0;
            return;
        }
    }
    WaitTime(1);
}


/** @brief Desenha tudo que a cutscene de início precisa */
static void drawStartCutsceneElements(PacMaiden* pacmaiden, Vector2** mapCellPosInSprite, Ghost* ghosts, Map map, bool PVP){
    ClearBackground(BLACK);
    drawMap(map, mapCellPosInSprite);
    if(PVP){
        drawCharactersPVP(pacmaiden, ghosts);
        drawHudPVP(pacmaiden);
    }
    else{
        drawCharacters(pacmaiden, ghosts);
        drawHud(pacmaiden);
    }
    DrawText("TAB para pular", WIDTH/2 - 80, HEIGHT+15, 20, RAYWHITE);
}


/** @brief Dá fade in nos fantasmas em sincronia com a música */
void gameStartCutscene(PacMaiden* pacmaiden, Vector2** mapCellPosInSprite, Ghost* ghosts, Map map, bool PVP){
    Music startTrack = LoadMusicStream(getFilePath("../../audio/Music/GameStart/GameStart.wav"));
    PlayMusicStream(startTrack);
    startTrack.looping = false;
    
    for(int i=0; i<4; i++){
        ghosts[i].chara.sprite.tint.a = 0;
        ghosts[i].chara.procAnimation.running = false;
    }
    
    while(gameState == STARTING){
        if(DEBUG_MODE)
        userClose();
        
        UpdateMusicStream(startTrack);

        for(int i=0; i<4; i++)
            if(GetMusicTimePlayed(startTrack) >= 1.35f*i && ghosts[i].chara.sprite.tint.a < 255)
                fadeIn(&(ghosts[i].chara.sprite.tint), &(ghosts[i].chara.procAnimation), 2);

        BeginDrawing();
        drawStartCutsceneElements(pacmaiden, mapCellPosInSprite, ghosts, map, PVP);
        EndDrawing();

        if(!IsMusicStreamPlaying(startTrack) || IsKeyPressed(KEY_TAB))
            gameState = RUNNING;
    }
    for(int i=0; i<4; i++)
        ghosts[i].chara.sprite.tint.a = 255;
    pacmaiden->timePivot = GetTime();
    StopMusicStream(startTrack);
    UnloadMusicStream(startTrack);
}


static void drawGameOverCutsceneElements(PacMaiden* pacmaiden, Vector2** mapCellPosInSprite, Ghost* ghosts, Map map, bool PVP, Color messageColorRec, Color messageColorText){
    ClearBackground(BLACK);
    drawMap(map, mapCellPosInSprite);
    if(PVP){
        drawCharactersPVP(pacmaiden, ghosts);
        drawHudPVP(pacmaiden);
    }
    else{
        drawCharacters(pacmaiden, ghosts);
        drawHud(pacmaiden);
    }

    char* messageText = "Game Over";
    DrawRectangle(0, HEIGHT/2 - 100, WIDTH, 200, messageColorRec);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), messageText, 200, 20);
    DrawTextEx(GetFontDefault(), messageText, (Vector2){(WIDTH - textSize.x) / 2, ((HEIGHT/2 - 100) + (525 - textSize.y)) /2}, 200, 20, messageColorText);
    DrawText("TAB para pular", WIDTH/2 - 80, HEIGHT+15, 20, RAYWHITE);
}


/** @brief Escreve a mensagem de fim de jogo e toca a música de derrota */
void gameOverCutscene(PacMaiden* pacmaiden, Vector2** mapCellPosInSprite, Ghost* ghosts, Map map, bool PVP){
    Music gameOverTrack = LoadMusicStream(getFilePath("../../audio/Music/GameOver/GameOver.wav"));
    PlayMusicStream(gameOverTrack);
    gameOverTrack.looping = false;
    
    BeginDrawing();
    DrawText("TAB para pular", WIDTH/2 - 80, HEIGHT+15, 20, RAYWHITE);
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
        drawGameOverCutsceneElements(pacmaiden, mapCellPosInSprite, ghosts, map, PVP, messageColorRec, messageColorText);
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
        drawGameOverCutsceneElements(pacmaiden, mapCellPosInSprite, ghosts, map, PVP, messageColorRec, messageColorText);
        DrawRectangle(0,0, WIDTH, HEIGHT+HUDHEIGHT, fadeOutColor);
        EndDrawing();
    }

    while(IsMusicStreamPlaying(gameOverTrack))
        UpdateMusicStream(gameOverTrack);

    StopMusicStream(gameOverTrack);
}
