/** @brief Animações de sprite renderizam de arquivos externos imagens, chamadas de "frames", alternando entre elas em um intervalo de tempo regular. */

#include<raylib.h>
#include<stdlib.h>
#include "../System/WindowControl.h"

typedef struct {
    Texture2D spriteSheet;
    int first;
    int last;
    int current;
    int rowSelect;
    int step;

    Vector2 frameSize;
    float speed;
    float timeInFrame;
    bool loop;
    bool running;
} SpriteAnimation; 
