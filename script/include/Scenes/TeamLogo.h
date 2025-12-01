#include "raylib.h"

#include "../Animations/ProceduralAnimation.h"
#include "../System/WindowControl.h"

#pragma once


void teamLogo(){
    WaitTime(1);
    Color fadeColor = BLACK;
    ProceduralAnimation fadeAnimation = {GetTime(), true};
    while(fadeAnimation.running){
        fadeOut(&fadeColor, &fadeAnimation, 2.0f);
        BeginDrawing();
        DrawText("JERB", LARGURA/2 -275, ALTURA/2 - 100, 200, RAYWHITE);
        DrawText("Jogos Eletrônicos Radicais e Brabos", LARGURA/2 -475, ALTURA/2 +75, 50, RAYWHITE);
        DrawText("João, Evandro, Renato e Binoto", LARGURA/2 -200, ALTURA/2 + 125, 25, RAYWHITE);
        DrawRectangle(0,0, LARGURA, ALTURA+ALTURAHUD, fadeColor);
        EndDrawing();

        if(IsKeyPressed(KEY_TAB)){
            fadeColor.a = 0;
            return;
        }
    }
    WaitTime(1);
    fadeAnimation = (ProceduralAnimation){GetTime(), true};
    while(fadeAnimation.running){
        fadeIn(&fadeColor, &fadeAnimation, 2.0f);
        BeginDrawing();
        DrawText("JERB", LARGURA/2 -275, ALTURA/2 - 100, 200, RAYWHITE);
        DrawText("Jogos Eletrônicos Radicais e Brabos", LARGURA/2 -475, ALTURA/2 +75, 50, RAYWHITE);
        DrawText("João, Evandro, Renato e Binoto", LARGURA/2 -200, ALTURA/2 + 125, 25, RAYWHITE);
        DrawRectangle(0,0, LARGURA, ALTURA+ALTURAHUD, fadeColor);
        
        EndDrawing();
        if(IsKeyPressed(KEY_TAB)){
            fadeColor.a = 0;
            return;
        }
    }
    WaitTime(1);
}