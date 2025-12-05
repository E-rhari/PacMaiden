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


void runSpriteAniation(SpriteAnimation* animation){
    float timePerFrame = 1/animation->speed;
    animation->timeInFrame += GetFrameTime();

    if(animation->timeInFrame >= timePerFrame){
        animation->current += animation->step;

        if(animation->current >= animation->last){
            if(animation->loop)
                animation->current %= animation->last; 
            else{
                animation->current = animation->last;
                animation->running = false;
            }
        }
        animation->timeInFrame = 0;
    }
}


Rectangle getSpriteFrame(SpriteAnimation* animation){
    float x = animation->current*animation->frameSize.x;
    float y = animation->rowSelect*animation->frameSize.y;
    return (Rectangle){x, y, animation->frameSize.x, animation->frameSize.y};
}