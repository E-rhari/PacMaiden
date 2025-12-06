/** @brief Animações de sprite renderizam de arquivos externos imagens, chamadas de "frames", alternando entre elas em um intervalo de tempo regular. */

#include "raylib.h"
#include <stdlib.h>

#include "../System/WindowControl.h"


#pragma once


/** @brief Cuida das propriedades de uma animação baseada numa imagem sprite sheet.*/
typedef struct {
    Texture2D spriteSheet;  // Textura do spritesheet completo.
    Vector2 frameSize;      // Tamanho de um único frame.

    int first;              // Index do primeiro frame da animação.
    int last;               // Index do último frame da animação.
    int current;            // Index do frame atual.
    int lineSelect;         // Index da linha de animação a ser percorrida.
    int step;               // Quantidade de frames a ser pulados a cada iteração da animação.

    float speed;            // Velocidade da animação. Quantas vezes por segundo a animação muda de frame.
    float timeInFrame;      // Quantidade de tempo que a animação está no frame atual.
    bool loop;              // Se a animação reinicia quando termina.
    bool running;           // Se a animação deve correr.
} SpriteAnimation; 


/** @brief Instancia uma Sprite Animation
 * @param spriteSheet Caminho para a imagem a ser usada como spritesheet.
 * @param frameSize Tamanho de um único frame.
 * @param speed Velocidade da animação. Quantas vezes por segundo a animação muda de frame.
 * @param loop Se a animação reinicia quando termina. */
SpriteAnimation innitSpriteAnimation(char spriteSheet[], Vector2 frameSize, float speed, bool loop){
    Texture spriteSheetTexture=LoadTexture(spriteSheet); 
    return (SpriteAnimation) {
        .timeInFrame=0,
        .current=0,
        .first=0,
        .last=(spriteSheetTexture.width/frameSize.x)-1,
        .lineSelect=0,
        .running=true,
        .step=1,

        .loop=loop,
        .speed=speed,
        .spriteSheet=LoadTexture(spriteSheet),
        .frameSize=frameSize,
    };
}

/** Desinstancia uma Sprite Animation */
void closeSpriteAnimation(SpriteAnimation* spriteAnimation){
    UnloadTexture(spriteAnimation->spriteSheet);
}


void stopSpriteAnimation(SpriteAnimation* animation){
    animation->running = false;
}


void playSpriteAnimation(SpriteAnimation* animation, int animationRow){
    animation->lineSelect = animationRow;
    animation->running = true;
}


/** @brief Seleciona a região da spritesheet que se encontra o frame atual */
Rectangle getSpriteFrame(SpriteAnimation* animation){
    float x = animation->current*animation->frameSize.x;
    float y = animation->lineSelect*animation->frameSize.y;
    return (Rectangle){x, y, animation->frameSize.x, animation->frameSize.y};
}


void updateSpriteAnimation(SpriteAnimation* animation){
    if(!animation->running)
        return;

    float timePerFrame = 1/animation->speed;
    animation->timeInFrame += GetFrameTime();

    if(animation->timeInFrame >= timePerFrame){
        animation->current += animation->step;

        if(animation->current > animation->last){
            if(animation->loop)
                animation->current = (animation->current%animation->last)+animation->first-1; 
            else{
                animation->current = animation->last;
                animation->running = false;
            }
        }
        animation->timeInFrame = 0;
    }
}


void drawSpriteAnimation(SpriteAnimation* animation, Vector2 position, Vector2 scale){
    Vector2 spriteVirtualSize = {animation->frameSize.x*scale.x, animation->frameSize.y*scale.y};
    DrawTexturePro(animation->spriteSheet, getSpriteFrame(animation), (Rectangle){position.x, position.y,  spriteVirtualSize.x, spriteVirtualSize.y}, (Vector2){0,0}, 0, WHITE);
}