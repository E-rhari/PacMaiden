#include "raylib.h"
#include"raymath.h"
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

#include "../include/System/WindowControl.h"
#include "../include/Scenes/Levels.h"
#include "../include/Scenes/TitleScreen.h"

int main(){
    inicializeWindow();

    enum screenBehavior myScreen = TITLE;

    while(!WindowShouldClose()){
        switch (myScreen)
        {
            case TITLE:
                myScreen=drawTitleScreen();
            break;
            case NEWGAME:
                myScreen=level(2);
            break;
        }
    }

}