#include "raylib.h"
#include <stdbool.h>
#include <time.h>
#include "../System/SaveSystem.h"
#include "./TitleScreen.h"

#pragma once

#define BUTTONBASE BLACK
#define BUTTONHOVER WHITE
#define BUTTONBAR BLACK

enum optionPress{
    RESUME,
    SAVE,
    LOADSAVE,
    RETURN
}; 

typedef enum KeyCode{
    S=83,
    C=67,
    TAB=258,
    Q=81
}KeyCode;
/**
 * @brief Estrutura do icone de menu
 * @param center centro do circulo
 * @param radius raio do circulo 
 * @param colorBase cor base
 * @param colorHover cor quando mause passe por cima
 * @param barColor cor das tres barras
 */

typedef struct MenuButton {
    Vector2 center;
    float radius;
    Color colorBase;
    Color colorHover;   
    Color barColor;
} menuButton;



/**
 * @brief Estrutura das opções do menu
 * @param optionBox retangulo de de cada opcção
 * @param colorBase cor base
 * @param colorHover cor quando mause passe por cima
 */
typedef struct OptionButton{
    Rectangle optionBox;
    Color colorBase;
    Color colorHover;
    enum optionPress id;
    void(*action)(void);
} OptionButton;


void gameStateRunning(){
    gameState=RUNNING;
}
void gameStateSaving(){
    gameState=SAVING;
}
void gameStateLoading(){
    gameState=LOADING;
}
void gameStateExit(){
    gameState=EXIT;
}


bool isOptionButtonHovered(OptionButton button, int buttonSelected) {
    if (isMouseMode()){
        Vector2 mousePos = GetMousePosition();
        return CheckCollisionPointRec(mousePos, button.optionBox);
    } else
        return (button.id == buttonSelected);
}

bool isSaveFileHovered(Rectangle save){
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, save);
}



int isSaveFileClicked(Rectangle* save)
{
    for (int i = 0; i < 3; i++)
        if (isSaveFileHovered(save[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            return i;
    return -1;
}

void isGameSaved(Rectangle* save){
    static bool showSaved = false;
    static float timer = 0.0f;

    float dt = GetFrameTime();
    const float duration = 3.0f;
    const float fadeTime = 0.3f;

    if (showSaved)
    {
        timer -= dt;
        if (timer <= 0.0f)
        {
            timer = 0.0f;
            showSaved = false;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (isSaveFileHovered(save[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            showSaved = true;
            timer = duration;
        }
    }

    if (showSaved)
    {
        float alpha = 1.0f;
        if(timer > duration - fadeTime){
            float t = (duration - timer) / fadeTime;
            alpha = t;
        }
        else if(timer < fadeTime){
            float t = timer / fadeTime;
            alpha = t;
        }
        unsigned char finalAlpha = (unsigned char)(alpha * 255.0f);
        Rectangle saved = {WIDTH/2 - 100, HEIGHT/2 - 50, 200, 100};
        DrawRectangleRounded(saved, 0.3f, 10, (Color){0, 0, 0, finalAlpha});
        DrawRectangleRoundedLines(saved, 0.3f, 10, RAYWHITE);
        const char* savedText = "Jogo salvo";
        Vector2 textSize = MeasureTextEx(GetFontDefault(), savedText, 18, 5);
        DrawTextEx(GetFontDefault(), savedText, (Vector2){saved.x+(saved.width - textSize.x) / 2, saved.y+(saved.height - textSize.y)/2}, 18, 5, (Color){255, 255, 255, finalAlpha});
    }
}

void initOptionButton(OptionButton *buttons){
    Vector4 optionMeasures = {700, 250, 200, 50};
    int paddingYButton=90;

    buttons[0]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*0, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,0,gameStateRunning};
    buttons[1]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*1, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,1,gameStateSaving};
    buttons[2]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*2, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,2,gameStateLoading};
    buttons[3]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*3, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,3,gameStateExit};
}

void initSaveButton(Rectangle *save){
    Rectangle saveBox = {650, 200, 300, 400};
    int saveRecx = 75;
    int saveRecy = 50;
    int savePicOffset = 50;
    int padding = 50;
    
    for(int i=0; i<3;i++)
        save[i] = (Rectangle){saveBox.x + savePicOffset, saveBox.y + padding*(i+1) + saveRecy*i, saveRecx, saveRecy};
}

void drawOptionButtons(Rectangle menuBox, OptionButton *buttons, int buttonSelected){
    int  gapText=50, paddingYText=90;
    Color textColor;
    Color optionColor;

    const char *menuOptions[] = {"Retornar (TAB)", "Salvar (S)", "Carregar (C)", "Sair (Q)"};

    for(int i=0;i<4;i++){
        
        if(isOptionButtonHovered(buttons[i], buttonSelected)){
            optionColor = buttons[i].colorHover;
            textColor = BUTTONBASE;
        }
        else{
            optionColor = buttons[i].colorBase;
            textColor = BUTTONHOVER;
        }

        DrawRectangleRounded(buttons[i].optionBox, 0.2f, 10, optionColor);

        Vector2 optionSize = MeasureTextEx(GetFontDefault(), menuOptions[i], 18, 1);
        DrawTextEx(GetFontDefault(), menuOptions[i], (Vector2){menuBox.x + (menuBox.width - optionSize.x) / 2, menuBox.y + gapText + (50 - optionSize.y) / 2}, 18, 1, textColor);
        gapText+=paddingYText;
    }

}

void drawOpenedMenu(OptionButton *buttons) {
    static int buttonSelected = -1;
    buttonSelected += IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) - IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    if (buttonSelected >= 3)
        buttonSelected = 3;
    else if (buttonSelected <= 0)
        buttonSelected = 0;
    
    switch (GetKeyPressed())
    {
        case S:
            gameState=SAVING;
        break;
        case Q:
            gameState=EXIT;
        break;
        case C:
            gameState=LOADING;
        break;
    }
   
    Rectangle menuBox = {650, 200, 300, 400};
    DrawRectangleRounded(menuBox, 0.1f, 10, BLACK);
    DrawRectangleRoundedLines(menuBox, 0.1f, 10, RAYWHITE);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), "Menu", 18, 1);
    DrawTextEx(GetFontDefault(), "Menu", (Vector2){menuBox.x + (menuBox.width - textSize.x) / 2, menuBox.y + (50 - textSize.y) / 2}, 20, 3, RAYWHITE);
    drawOptionButtons(menuBox, buttons, buttonSelected);

    if (isMouseMode()){
        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            return;}
    } else if (!IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        return;
    for(int i=0;i<4;i++)
        if(isOptionButtonHovered(buttons[i], buttonSelected))
            buttons[i].action();
}

void drawSaveStates(Rectangle*savePic){
    Rectangle saveBox = {650, 200, 300, 400};
    DrawRectangleRounded(saveBox, 0.1f, 10, BLACK);
    DrawRectangleRoundedLines(saveBox, 0.1f, 10, RAYWHITE);

    const char* backText = "Pressione TAB para voltar";
    Vector2 textSize = MeasureTextEx(GetFontDefault(), backText, 18, 1);
    DrawTextEx(GetFontDefault(), backText, (Vector2){saveBox.x + (saveBox.width - textSize.x) / 2, saveBox.y + saveBox.height - textSize.y - 20}, 18, 1, RAYWHITE);
    int textOffsetX = 95;
    int textOffsetY = 10;

    Vector2 mousePos = GetMousePosition();

    for(int i = 0; i < 3; i++){
        bool hovered = CheckCollisionPointRec(mousePos, savePic[i]);
        Color saveColorHovered = hovered ? GRAY : BLACK;
        DrawRectangleRounded(savePic[i], 0.1f, 10, saveColorHovered);
        DrawRectangleRoundedLinesEx(savePic[i], 0.1f, 10, 3, BUTTONHOVER);

        char saveText[64];
        snprintf(saveText, sizeof(saveText), "Save %d", i + 1);
        DrawTextEx(GetFontDefault(), saveText,
                   (Vector2){savePic[i].x + textOffsetX, savePic[i].y + textOffsetY},
                   18, 1, RAYWHITE);

        isGameSaved(&savePic[i]);
    }
}