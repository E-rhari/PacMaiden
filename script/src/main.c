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
    int file;
    enum screenBehavior myScreen = TITLE;

    Rectangle* saveOptions = malloc(sizeof(Rectangle)*3);
    initSaveTitleButton(saveOptions);

    while(!WindowShouldClose()){
        switch (myScreen)
        {
            case TITLE:
                myScreen=drawTitleScreen();
            break;
            case NEWGAME:
                file=2;
                myScreen=level(file);
            break;
            case NEXT:
                file++;
                myScreen=level(file);
            break;
            case LOAD:
                drawTitleSaveStates(saveOptions);
                file=isSaveTitleFileClicked(saveOptions);
                if(file!=-1)
                    myScreen=loadLevel(file);
            break;
        }
    }

}