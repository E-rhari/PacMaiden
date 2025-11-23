#include "raylib.h"
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>

#pragma once


/** @brief Salva o próximo input válido recebido em um buffer e altera o valor de lastInput se a condição refreshCondition 
 *        for satisfeita. Outra condição para a modificação do valor é se o input recebido for do meios eixo (x ou y) do
 *        lastInput.
 * @param lastInput Referência ao último input enviado para o personagem
 * @param refreshCondition Condição para alterar o vetor de input
 * @returns Se o input foi alterado ou não. */
bool getBufferedInput(Vector2* lastInput, bool refreshCondition){
    static Vector2 bufferedInput = {0,0};
    Vector2 recievedInput = {0,0};
    bool moved = false;

    recievedInput.x = fmaxf(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT), IsKeyDown(KEY_RIGHT))
        - fmaxf(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT), IsKeyDown(KEY_LEFT));
    recievedInput.y = fmaxf(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN), IsKeyDown(KEY_DOWN))
        - fmaxf(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP), IsKeyDown(KEY_UP));

    if(abs((int)recievedInput.x) != abs((int)recievedInput.y))
        if(recievedInput.x != 0 || recievedInput.y != 0)    
            bufferedInput = recievedInput;

    if(refreshCondition || (lastInput->x!=0 && bufferedInput.x!=0) || (lastInput->y!=0 && bufferedInput.y!=0)){
        *lastInput = bufferedInput;
        moved = true;
    }
    return moved;
}