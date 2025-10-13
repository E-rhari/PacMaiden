#include<raylib.h>
#include<raymath.h>
#include<string.h>
#include<stdbool.h>

#include "../include/Character.h"
#include "../include/PacMaiden.h"
#include "../include/WindowControl.h"
#include "../include/Map.h"

// #pragma once

int main(){
    InitWindow(LARGURA, ALTURA, "Jogo Irado!!!");
    SetTargetFPS(60);

    PacMaiden pacMaiden = initPacMaiden((Vector2){400, 300}, 100, 20, 20, RED, 3, 0);
    Vector2 input = {0, 0};
    Vector2 direction = (Vector2){0,0};

    while(!WindowShouldClose()){
        userClose();

        input.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
	    input.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);

        if(!Vector2Equals(input, Vector2Zero())) direction = input;

        move(&pacMaiden.chara, direction);

        BeginDrawing();

        char texto[100];
        // sprintf(texto, "%d %d", (int)pacMaidenBlockPosition.x, (int)pacMaidenBlockPosition.y);
        // DrawText(texto, 100, 100, 100, BLACK);

        ClearBackground(RAYWHITE);
        renderizaMapa(mapa, 20, 20);
        
        DrawRectangleRec(pacMaiden.chara.rec, pacMaiden.chara.color);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}