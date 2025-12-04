#include <stdbool.h>
#include "raylib.h"

#include "../System/WindowControl.h"

#pragma once

#define BUTTONTITLEBASE (Color){10, 40, 80, 255}
#define BUTTONTITLEHOVER (Color){20, 70, 140, 255}
#define BUTTONTITLEBAR (Color){100, 180, 255, 255}



typedef struct {
    Rectangle optionBox;
    Color colorBase;
    Color colorHover;
    enum screenBehavior id;
} titleButton;


bool isTitleButtonHovered(titleButton button){
    Vector2 mousePosition = GetMousePosition();
    return CheckCollisionPointRec(mousePosition,button.optionBox);
}

void isTitleButtonClicked(titleButton *button){
    for(int i=0;i<4;i++)
        if(isTitleButtonHovered(button[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            changeScene(button[i].id);
}


void drawTitleScreen(){
    titleButton buttons[4];
    char *optionsText[]={"New Game","Load","PVP","Creditos"};
    int paddingYButton=110, paddingYTxt=110;

    Vector4 optionMeasures = {450,200,700,70};
    Rectangle background = {300,50,1000,750};

    ClearBackground(BLACK);
    DrawRectangleRec(background,DARKBLUE);

    for(int i=0;i<4;i++){
        buttons[i] = (titleButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*i, optionMeasures.z, optionMeasures.w},BUTTONTITLEBASE, BUTTONTITLEHOVER,i};
        Color optionColor = isTitleButtonHovered(buttons[i])? buttons[i].colorHover : buttons[i].colorBase;
        DrawRectangleRounded(buttons[i].optionBox, 0.2f, 10, optionColor);

        Vector2 optionSize = MeasureTextEx(GetFontDefault(), optionsText[i], 18, 1);
        DrawTextEx(GetFontDefault(), optionsText[i], (Vector2){background.x + (background.width - optionSize.x) / 2, background.y + paddingYTxt*i + 180}, 18, 1, BUTTONTITLEBAR);
    }

    isTitleButtonClicked(buttons);
}


void titleScreen(){

    Music titleTheme = LoadMusicStream(getFilePath("../../audio/Music/Title/Title.wav"));
    PlayMusicStream(titleTheme);


    Color fadeColor = BLACK;
    ProceduralAnimation fadeAnimation = {GetTime(), true};

    while(currenctScene == TITLE){
        if(fadeAnimation.running)
            fadeOut(&fadeColor, &fadeAnimation, 1.0f);

        UpdateMusicStream(titleTheme);

        BeginDrawing();
        drawTitleScreen();
        DrawRectangle(0,0, LARGURA, ALTURA, fadeColor);
        EndDrawing();
    }
    fadeAnimation = (ProceduralAnimation){GetTime(), true};
    while(fadeAnimation.running){
        fadeIn(&fadeColor, &fadeAnimation, 2.0f);

        UpdateMusicStream(titleTheme);

        BeginDrawing();
        drawTitleScreen();
        DrawRectangle(0,0, LARGURA, ALTURA, fadeColor);
        EndDrawing();
    }
    WaitTime(1);

    StopMusicStream(titleTheme);
    UnloadMusicStream(titleTheme);
}


void initSaveTitleButton(Rectangle *save){
    Rectangle saveBox = {600, 175, 400, 450};
    int saveRecx = 75;
    int saveRecy = 50;
    int savePicOffset = 50;
    int padding = 75;
    
    for(int i=0; i<3;i++)
        save[i] = (Rectangle){saveBox.x + savePicOffset, saveBox.y + padding*(i+1) + saveRecy*i, saveRecx, saveRecy};
}

bool isSaveTitleFileHovered(Rectangle save){
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, save);
}

int isSaveTitleFileClicked(Rectangle* save){

  for(int i=0;i<3;i++)
     if(isSaveTitleFileHovered(save[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        return i;
     }
    return -1;
}

void drawTitleSaveStates(Rectangle*savePic){
    Rectangle saveBox = {600, 175, 400, 450};
    DrawRectangleRounded(saveBox, 0.1f, 10, (Color){ 30, 80, 255, 255 });

    int textOffsetX = 95;
    int textOffsetY = 10;
    BeginDrawing();
    for(int i = 0; i < 3; i++){
        DrawRectangleRounded(savePic[i], 0.1f, 10, BLACK);
        DrawRectangleRoundedLinesEx(savePic[i], 0.1f, 10, 3, BUTTONBAR);

        time_t now = time(NULL);
        struct tm *infoTime = localtime(&now);
        char saveMoment[32];
        strftime(saveMoment, sizeof(saveMoment), "%d/%m/%Y %H:%M", infoTime);
        char saveText[64];
        snprintf(saveText, sizeof(saveText), "Save %d - %s", i + 1, saveMoment);
        DrawTextEx(GetFontDefault(), saveText,
                   (Vector2){savePic[i].x + textOffsetX, savePic[i].y + textOffsetY},
                   18, 1, RAYWHITE);

        Vector2 mousePos = GetMousePosition();
    }
    EndDrawing();
}