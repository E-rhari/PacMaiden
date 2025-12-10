#include "raylib.h"

#pragma once

// Vetor discreto da posições em um grid. Conveniente para o controle de posições no mapa.
typedef struct {
    int x;
    int y;
} GridVector;


/** @brief Compara dois gridvectors */
bool gridVectorEquals(GridVector position1, GridVector position2){
    return position1.x == position2.x && position1.y == position2.y;
}


/** @brief Transform um Vector2 em um GridVector, fazendo a conversão de escala 1:40 */
GridVector vector2ToGridVector(Vector2 vector){
    return (GridVector){(int)(vector.x/40), (int)(vector.y/40)};
}


/** @brief Modula um valor de 0 até module */
int modulate(int number, int module){
    while(number < 0)
        number += module;
    return number%module;
}