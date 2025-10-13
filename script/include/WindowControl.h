/**
 * @brief Arquivo de configuração de constantes de tela e de funções de controle dela.
 */

#include<raylib.h>

#pragma once

#define LARGURA 800
#define ALTURA 800

#define PIX2GRID 0.025f
#define GRID2PIX 40.0f


/**
 * @brief Permite que o usuário feche a tela usando ctrl + c
 */
void userClose(){
    if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_C))
        CloseWindow();
}