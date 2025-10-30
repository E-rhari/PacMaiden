#include<raylib.h>
#include<stdlib.h>


typedef struct {
    int initTime;
    bool running;
} FunctionAnimation;


void fadeOut(Color* color, FunctionAnimation* animation, int duration){
    double timeElapsed = GetTime() - animation->initTime;
    color->a = (int)(255 - 255*(timeElapsed/duration));
    if(timeElapsed>=duration)
        animation->running = false;
}


void fadeIn(Color* color, FunctionAnimation* animation, int duration){
    double timeElapsed = GetTime() - animation->initTime;
    color->a = (int)(255*(timeElapsed/duration));
    if(timeElapsed>=duration)
        animation->running = false;
}