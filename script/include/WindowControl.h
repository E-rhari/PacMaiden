#include<raylib.h>

#pragma once

#define LARGURA 800
#define ALTURA 800

#define PIX2GRID 0.025f
#define GRID2PIX 40.0f

void userClose(){
    if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_C))
        CloseWindow();
}