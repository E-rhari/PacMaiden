#include "../include/mapa/map.h"


void start(Mapa mapa){
    InitWindow(1600, 840, "Teclas");
    lerMapa(1,mapa);
}

void update(Mapa mapa){
  

    while(!WindowShouldClose()){
        BeginDrawing();
        renderizaMapa(mapa);
        EndDrawing();
        if(IsKeyDown(KEY_D))
            return;    
    }

}

int main(){
    Mapa mapa=setUpMapa();

    start(mapa);
    update(mapa);
    CloseWindow();
    
}