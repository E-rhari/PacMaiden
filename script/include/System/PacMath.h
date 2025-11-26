#include "raylib.h"

#pragma once


typedef struct {
    int x;
    int y;
} GridVector;


bool gridVectorEquals(GridVector position1, GridVector position2){
    return position1.x == position2.x && position1.y == position2.y;
}

GridVector vector2ToGridVector(Vector2 vector){
    return (GridVector){(int)(vector.x/40), (int)(vector.y/40)};
}


/** @brief Modula um valor de 0 até module */
int modulate(int number, int module){
    while(number < 0)
        number += module;
    return number%module;
}