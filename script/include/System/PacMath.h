/** @brief Funções e recursos matemáticos que não tinham em nehuma biblioteca padrão */


#include "raylib.h"

#pragma once

/** @brief Vetor discreto da posições em um grid. Conveniente para o controle de posições no mapa. */
typedef struct {
    int x;
    int y;
} GridVector;


/** @brief Compara dois gridvectors */
bool gridVectorEquals(GridVector position1, GridVector position2){
    return position1.x == position2.x && position1.y == position2.y;
}


/** @brief Transforma um Vector2 em um GridVector, fazendo a conversão de escala 1:40 */
GridVector vector2ToGridVector(Vector2 vector){
    return (GridVector){(int)(vector.x*PIX2GRID), (int)(vector.y*PIX2GRID)};
}


/** @brief Transforma um GridVector em um Vector2, fazendo a conversão de escala 1:40 */
Vector2 gridVectorToVector2(Vector2 vector){
    return (Vector2){(float)(vector.x*GRID2PIX), (float)(vector.y*GRID2PIX)};
}


/** @brief Modula um number de 0 até module-1 */
int modulate(int number, int module){
    while(number < 0)
        number += module;
    return number%module;
}