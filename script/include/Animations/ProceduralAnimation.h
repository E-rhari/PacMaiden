/** @brief Animações procedurais são aquelas que modificam propriedades de um objeto através de funções matemáticas e códigos,
 *         sem renderizar imagens de arquivos externos e alternar entre frames.
 */


#include<raylib.h>
#include<stdlib.h>
#include<math.h>

/** @brief Struct que controla uma animação desenhada por código. */
typedef struct {
    int initTime;
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
    if(sin(frequency*PI*2*timeElapsed)<0) 
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


/** @brief Remove a opacidade de uma cor na duração especificada. */
void fadeOut(Color* color, ProceduralAnimation* animation, int duration){
    double timeElapsed = GetTime() - animation->initTime;
    color->a = (int)(255 - 255*(timeElapsed/duration));
    if(timeElapsed>=duration){
        animation->running = false;
        return;
    }
    animation->running =  true;
}


/** @brief Eleva ao máximo a opacidade de uma cor na duração específica. */
void fadeIn(Color* color, ProceduralAnimation* animation, int duration){
    double timeElapsed = GetTime() - animation->initTime;
    color->a = (int)(255*(timeElapsed/duration));
    if(timeElapsed>=duration){
        animation->running =  false;
        return;
    }
    animation->running =  true;
}