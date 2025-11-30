#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

#include "./Ghost.h"
#include "../../Map/Map.h"

#pragma once


/**  @brief Escolhe uma direção aleatória para a movimentação do fantasma. Esse função não vê nenhum problema em bater de cara na 
 *        parede, em retroceder o movimento derrepente ou em andar em círculos.
 * @return Se a direção foi alterada. */
bool choseDestinationUnaware(Ghost* ghost){
    if (isCharacterInGridCenter(ghost->chara) && isCharacterInsideScreen(ghost->chara, (Vector2){0,0})){
        int random = (rand()%4) + 1;
        if(random == 1)
            ghost->chara.moveDirection = (Vector2){0,-1};
        else if(random == 2)
            ghost->chara.moveDirection = (Vector2){0,1};
        else if(random == 3)
            ghost->chara.moveDirection = (Vector2){-1, 0};
        else if(random == 4)
            ghost->chara.moveDirection = (Vector2){1, 0};
        return true;
    }
    return false;
}
