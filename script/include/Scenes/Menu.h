#include "raylib.h"
#include <stdbool.h>
#include <time.h>
#include "./TitleScreen.h"
#include "../System/SaveSystem.h"

#pragma once

#define BUTTONBASE (Color){10, 40, 80, 255}
#define BUTTONHOVER (Color){20, 70, 140, 255}
#define BUTTONBAR (Color){100, 180, 255, 255}

/** @brief representa a ação que um botão faz ao ser clicado*/
enum optionPress{
    RESUME,
    SAVE,
    LOADSAVE,
    RETURN
}; 
/** @brief mapeamento do keycode das teclas, para, quando clicadas, desempenharam uma ação*/
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

/**
 * @return retorna se o mouse está sobre um botão
 */


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

int isSaveFileClicked(Rectangle* save){

  for(int i=0;i<3;i++)
     if(isSaveFileHovered(save[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return i;
    return -1;
}

/** @brief inicializa os botões do menu */
void initOptionButton(OptionButton *buttons){
    Vector4 optionMeasures = {700, 250, 200, 50};
    int paddingYButton=90;

    buttons[0]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*0, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,0,gameStateRunning};
    buttons[1]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*1, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,1,gameStateSaving};
    buttons[2]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*2, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,2,gameStateLoading};
    buttons[3]=(OptionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*3, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,3,gameStateExit};
}
/** @brief inicializa os botões do save */
void initSaveButton(Rectangle *save){
    Rectangle saveBox = {600, 175, 400, 450};
    int saveRecx = 75;
    int saveRecy = 50;
    int savePicOffset = 50;
    int padding = 75;
    
    for(int i=0; i<3;i++)
        save[i] = (Rectangle){saveBox.x + savePicOffset, saveBox.y + padding*(i+1) + saveRecy*i, saveRecx, saveRecy};
}

void drawOptionButtons(Rectangle menuBox, OptionButton *buttons, int buttonSelected){
    int  gapText=50, paddingYText=90;

    const char *menuOptions[] = {"Retornar (TAB)", "Salvar (S)", "Carregar (C)", "Sair (Q)"};

    for(int i=0;i<4;i++){
        
        Color optionColor = isOptionButtonHovered(buttons[i], buttonSelected) ? buttons[i].colorHover : buttons[i].colorBase;

        DrawRectangleRounded(buttons[i].optionBox, 0.2f, 10, optionColor);
        DrawRectangleRoundedLinesEx(buttons[i].optionBox, 0.2f, 10, 3, BUTTONBAR);

        Vector2 optionSize = MeasureTextEx(GetFontDefault(), menuOptions[i], 18, 1);
        DrawTextEx(GetFontDefault(), menuOptions[i], (Vector2){menuBox.x + (menuBox.width - optionSize.x) / 2, menuBox.y + gapText + (50 - optionSize.y) / 2}, 18, 1, BUTTONBAR);
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
    DrawRectangle(0, 0, WIDTH, HUDHEIGHT, (Color){0, 20, 60, 150});
    DrawRectangleRounded(menuBox, 0.1f, 10, (Color){ 30, 80, 255, 255 });
    Vector2 textSize = MeasureTextEx(GetFontDefault(), "Menu", 18, 1);
    DrawTextEx(GetFontDefault(), "Menu", (Vector2){menuBox.x + (menuBox.width - textSize.x) / 2, menuBox.y + (50 - textSize.y) / 2}, 18, 1, RAYWHITE);
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

void drawSaveStates(Rectangle* savePic){
    Rectangle saveBox = {600, 175, 400, 450};
    DrawRectangleRounded(saveBox, 0.1f, 10, (Color){ 30, 80, 255, 255 });

    int textOffsetX = 95;
    int textOffsetY = 10;

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
}

