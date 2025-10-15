#include<raylib.h>
#include<stdbool.h>

#pragma once


/**
 * @brief Salva o próximo input válido recebido em um buffer e altera o valor de lastInput se a condição refreshCondition 
 *        for satisfeita 
 * 
 * @param lastInput Referência ao último input enviado para o personagem
 * @param refreshCondition Condição para alterar o vetor de input
 *
 * @returns Se o input foi alterado ou não.
 */
bool getBufferedInput(Vector2* lastInput, int refreshCondition){
    static Vector2 bufferedInput = {0,0};
    Vector2 recievedInput = {0,0};

    recievedInput.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    recievedInput.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);

    if(recievedInput.x != recievedInput.y)
        if(recievedInput.x != 0 || recievedInput.y != 0)    
                bufferedInput = recievedInput;
    
    if(refreshCondition){
        *lastInput = bufferedInput;
        return true;
    }
    return false;
}