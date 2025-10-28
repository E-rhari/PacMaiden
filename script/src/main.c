#include "raylib.h"
#include"raymath.h"
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

#include "../include/System/WindowControl.h"
#include "../include/Scenes/Levels.h"

enum Screen{
    TITLE,
    LEVEL,
    DEATH,
    MENU //temporário
};

int main(){
    inicializeWindown();

    enum Screen myScreen = LEVEL;

    while(!WindowShouldClose()){
        switch (myScreen)
        {
            case TITLE:
                //
            break;
            case LEVEL:
                level();
            break;
            case DEATH:
                //
            break;
        }
    }

}