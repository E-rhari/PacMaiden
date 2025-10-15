#include<raylib.h>
#include<raymath.h>
#include<string.h>
#include<stdbool.h>

#include "../include/Character.h"
#include "../include/PacMaiden.h"
#include "../include/WindowControl.h"
#include "../include/Map.h"


int main(){
    InitWindow(LARGURA, ALTURA, "Jogo Irado!!!");
    SetTargetFPS(60);

    PacMaiden pacMaiden = initPacMaiden((Vector2){400, 400}, 20, 100, RED, 3, 0);
    Vector2 input = {0, 0};
    Vector2 bufferedInput = {0,0};
    Vector2 direction = {0,0};

    while(!WindowShouldClose()){
        userClose();

        input.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
	    input.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);

        // if((int)pacMaiden.chara.rec.x%40 < 10 && (int)pacMaiden.chara.rec.y%40 < 10)
        //     input = bufferedInput;

        if(!Vector2Equals(input, Vector2Zero())) 
            direction = input;

        move(&pacMaiden.chara, direction);

        BeginDrawing();

        ClearBackground(RAYWHITE);
        renderizaMapa(mapa, 20, 20);
        
        DrawCircleV(pacMaiden.chara.collider.center, pacMaiden.chara.collider.radius, pacMaiden.chara.color);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}