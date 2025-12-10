#include "raylib.h"
#include"raymath.h"
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

#include "../include/System/WindowControl.h"
#include "../include/Scenes/Levels.h"
#include "../include/Scenes/TitleScreen.h"
#include "../include/System/PacMath.h"
#include "../include/Scenes/Cutscenes.h"
#include "../include/Scenes/PVP.h"

int main(){
    inicializeWindow();
    InitAudioDevice();
    loadAllSprites();

    int file;
    currenctScene = TITLE;

    Rectangle* saveOptions = malloc(sizeof(Rectangle)*3);
    initSaveTitleButton(saveOptions);

    teamSplashScreen();

    while(!WindowShouldClose()){
        switch (currenctScene)
        {
            case TITLE:
                titleScreen();
            break;
            case NEWGAME:
                file=0;
                level(file);
            break;
            case NEXT:
                file = modulate(file+1, 2) + 1;
                level(file);
            break;
            case LOAD:
                drawTitleSaveStates(saveOptions);
                file=isSaveTitleFileClicked(saveOptions);
                if(file!=-1)
                    level(file);
            break;
            case PVP:
                StartPVP();
            break;
        }
    }
    CloseAudioDevice();
    CloseWindow();
}