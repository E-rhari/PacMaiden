/** @brief Arquivo de configuração de constantes de tela e de funções de controle dela. */

#include "raylib.h"
#include<stdbool.h>
#include<time.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1600
#define HEIGHT 800
#define HUDHEIGHT 40

#define SCORESIZE 40
#define SCOREPOSY 10

/** @brief Constante de conversão de pixels para blocos do grid. Cada bloco do grid em 40x40px */
#define PIX2GRID 0.025f
/** @brief Constante de conversão de blocos do grid para pixels. Cada bloco do grid em 40x40px */
#define GRID2PIX 40.0f

/** Constante de modo debug. Se estiver como true, anotações de debug irão aparecer na tela */
#define DEBUG_MODE true

#pragma once


typedef enum {
    NEWGAME,
    LOAD,
    PVP,
    CREDITS,
    TITLE,
    NEXT
} Scenes; 
Scenes currenctScene; // Controla a cena em que o jogo atualmente se encontra


typedef enum GameState{
    STARTING,
    RUNNING,
    PAUSED,
    SAVING,
    LOADING,
    GAMEOVER,
    EXIT
}GameState;
GameState gameState; // Controla o estado de levels


void changeScene(Scenes screen){
    currenctScene = screen;

    switch (screen)
    {
        case NEWGAME: break;
        case LOAD: break;
        case PVP: break;
        case CREDITS: break;
        case TITLE: break;
        case NEXT: break;
    }
}


/** @brief Permite que o usuário feche a tela usando ctrl + c */
void userClose(){
    if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_C))
        CloseWindow();
}


void inicializeWindow(){
    InitWindow(WIDTH, HEIGHT + HUDHEIGHT, "-= PacMaiden =-");
    SetTargetFPS(60);
    srand(time(NULL));
}


/** @brief Verifica se a posição está dentro da tela do jogo */
bool isPositionInsideScreen(Vector2 position,Vector2 displacement){
    return (int)position.y+(int)displacement.y>=0 && (int)position.y+(int)displacement.y<HEIGHT
        && (int)position.x+(int)displacement.x>=0 && (int)position.x+(int)displacement.x<WIDTH;
}