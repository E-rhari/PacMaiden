#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

#include "./Ghost.h"
#include "../../Map/Map.h"

#pragma once


/** @brief Altera a propriedade moveDirection do fantasma aleatóriamente, mas considerando os seus arredores. O movimento 
 *        só é retrocedido se estritamente necessário, enquanto escolhe aleatoriamente uma posição entre as possíveis.
 * @return Se a direção foi alterada. */
bool chooseDestinationAware(Ghost* ghost, Map map){
    if(!isCharacterInsideScreen(ghost->chara, (Vector2){0,0}))
        return false;
    
    Vector2 directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    Vector2 *possibleDirections= (Vector2*)malloc(1);
    int length=0;

    for(int i=0; i<4; i++)
        // Detectas as direções possíveis, desconsiderando a retrograda
        if(readPositionInMap(ghost->chara.circle.center, map, directions[i]) != '#'
        && !Vector2Equals(directions[i], Vector2Scale(ghost->chara.moveDirection,-1))){
            possibleDirections = (Vector2*)realloc(possibleDirections, (length+1)*sizeof(Vector2));
            possibleDirections[length] = directions[i];
            length++;
        }
    if(length>0){
        // Escolhe aleatoriamente uma direção possível
        int random = (rand()%length);
        ghost->chara.moveDirection = possibleDirections[random];
    }
    else
        // Determina a direção como retrograda
        ghost->chara.moveDirection = Vector2Scale(ghost->chara.moveDirection,-1);

    return true;

}

