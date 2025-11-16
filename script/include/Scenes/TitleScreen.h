#include <stdbool.h>
#include "raylib.h"

#include "../System/WindowControl.h"

#pragma once

#define BUTTONBASE (Color){10, 40, 80, 255}
#define BUTTONHOVER (Color){20, 70, 140, 255}
#define BUTTONBAR (Color){100, 180, 255, 255}



typedef struct{
    Rectangle optionBox;
    Color colorBase;
    Color colorHover;
    enum screenBehavior id;
} optionButton;


bool isOptionButtonHovered(optionButton button){
    Vector2 mousePosition = GetMousePosition();
    return CheckCollisionPointRec(mousePosition,button.optionBox);
}

int isOptionButtonClicked(optionButton *button){
    for(int i=0;i<4;i++)
        if(isOptionButtonHovered(button[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            return button[i].id;
    return 4;
}

int drawTitleScreen(){
    optionButton buttons[4];
    char *optionsText[]={"New Game","Load","PVP","Creditos"};
    int paddingYButton=110, paddingYTxt=110;


    Vector4 optionMeasures = {450,200,700,70};
    Rectangle background = {300,50,1000,750};

    BeginDrawing();

    ClearBackground(BLACK);
    DrawRectangleRec(background,DARKBLUE);

    for(int i=0;i<4;i++){

        buttons[i] = (optionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*i, optionMeasures.z, optionMeasures.w},BUTTONBASE, BUTTONHOVER,i};
        Color optionColor = isOptionButtonHovered(buttons[i])? buttons[i].colorHover : buttons[i].colorBase;
        DrawRectangleRounded(buttons[i].optionBox, 0.2f, 10, optionColor);

        Vector2 optionSize = MeasureTextEx(GetFontDefault(), optionsText[i], 18, 1);
        DrawTextEx(GetFontDefault(), optionsText[i], (Vector2){background.x + (background.width - optionSize.x) / 2, background.y + paddingYTxt*i + 180}, 18, 1, BUTTONBAR);
    }

    EndDrawing();
    return isOptionButtonClicked(buttons);
}