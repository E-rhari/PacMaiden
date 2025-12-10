/** @brief Animações procedurais são aquelas que modificam propriedades de um objeto através de funções matemáticas e códigos,
 *         sem renderizar imagens de arquivos externos e alternar entre frames. */


#include<raylib.h>
#include<stdlib.h>
#include<math.h>
#include"SpriteAnimation.h"

#pragma once


/** @brief Struct que controla uma animação desenhada por código. */
typedef struct {
    double initTime;
    bool running;
} ProceduralAnimation;


/** @brief Alterna entre duas cores
 * @param currentColor Referência a cor do objeto a ser alterada.
 * @param color1 Primeira cor a ser alternada. A animação sempre termina nela
 * @param color2 Segunda cor a ser alternada.
 * @param animation A struct que tomará conta da animação.
 * @param duration (s) Duração da animação.
 * @param frequency (Hz) Quantas vezes por segundo as cores irão alternar, isso é, quantas vezes a color2 será alcançada por segundo. */
void blinkAnimation(Color* currentColor, Color color1, Color color2, ProceduralAnimation* animation, int duration, float frequency){
    double timeElapsed = GetTime() - animation->initTime;
    if(cos(frequency*PI*2*timeElapsed)<=0) 
        *currentColor = color1;
    else
        *currentColor = color2;

    if(timeElapsed>=duration){
        *currentColor = color1;
        animation->running = false;
        return;
    }
    animation->running = true;
}


void spriteBlinkAnimation(Texture* sprite, Texture spriteSheet1, Texture spriteSheet2, ProceduralAnimation* animation, float duration, float frequency, float power){
    double timeElapsed = GetTime() - animation->initTime;
    if(cos(frequency*PI*2*pow(timeElapsed, power))>0) 
        *sprite = spriteSheet1;
    else
        *sprite = spriteSheet2;

    if(timeElapsed>=duration){
        *sprite = spriteSheet1;
        animation->running = false;
        return;
    }
    animation->running = true;
}


/** @brief Remove a opacidade de uma cor na duração especificada. */
void fadeOut(Color* color, ProceduralAnimation* animation, float duration){
    double timeElapsed = GetTime() - animation->initTime;
    color->a = (int)(255 - 255*(timeElapsed/duration));
    if(timeElapsed>=duration){
        color->a = 0;
        animation->running = false;
        return;
    }
    animation->running =  true;
}


/** @brief Eleva ao máximo a opacidade de uma cor na duração específica. */
void fadeIn(Color* color, ProceduralAnimation* animation, float duration){
    if(animation->running == false)
        animation->initTime = GetTime();

    animation->running =  true;
    double timeElapsed = GetTime() - animation->initTime;
    color->a = (int)(255*(timeElapsed/duration));

    if(timeElapsed>=duration){
        color->a = 255;
        animation->running =  false;
        return;
    }
}
