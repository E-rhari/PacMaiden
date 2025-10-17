/**
 * @brief Arquivo de configuração de constantes de tela e de funções de controle dela.
 */

#include "raylib.h"
#include<stdbool.h>

#define LARGURA 1600
#define ALTURA 800

/**
 * @brief Constante de conversão de pixels para blocos do grid. Cada bloco do grid em 40x40px
 */
#define PIX2GRID 0.025f
/**
 * @brief Constante de conversão de blocos do grid para pixels. Cada bloco do grid em 40x40px
 */
#define GRID2PIX 40.0f

/**
 * Constante de modo debug. Se estiver como true, anotações de debug irão aparecer na tela
 */
#define DEBUG_MODE true

#pragma once


/**
 * @brief Permite que o usuário feche a tela usando ctrl + c
 */
void userClose(){
    if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_C))
        CloseWindow();
}