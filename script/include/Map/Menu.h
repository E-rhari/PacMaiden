#include "raylib.h"
#include <stdbool.h>
#include <time.h>
#include "../System/SaveSystem.h"

#pragma once

#define BUTTONBASE (Color){10, 40, 80, 255}
#define BUTTONHOVER (Color){20, 70, 140, 255}
#define BUTTONBAR (Color){100, 180, 255, 255}

enum optionPress{// temp
    RESUME,
    SAVE,
    LOAD,
    RETURN
}; 


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
typedef struct{
    Rectangle optionBox;
    Color colorBase;
    Color colorHover;
    enum optionPress id;
} optionButton;

/**
 * @return retorna se o mouse está sobre um botão
 */
bool isMenuButtonHovered(menuButton button) {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointCircle(mousePos, button.center, button.radius);
}

bool isOptionButtonHovered(optionButton button) {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, button.optionBox);
}

/**
 * @return retorna se o mouse está sobre um botão
 */
bool isMenuButtonClicked(menuButton button) {
    return isMenuButtonHovered(button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int isOptionButtonClicked(optionButton buttons[]){
    for(int i=0;i<4;i++)
        if(isOptionButtonHovered(buttons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            return buttons[i].id;
    return -1;
}
void drawMenuButton(menuButton button) {
    bool hovered = isMenuButtonHovered(button);
    Color circleColor = hovered ? button.colorHover : button.colorBase;

    DrawCircleV(button.center, button.radius + 2, BUTTONBAR);
    DrawCircleV(button.center, button.radius, circleColor);

    float rectWidth = 30.0f;
    float rectHeight = 6.0f;
    float spacing = 10.0f;

    for (int i = -1; i <= 1; i++) {
        float x = button.center.x - rectWidth / 2.0f;
        float y = button.center.y + i * spacing - rectHeight / 2.0f;

        DrawRectangleRounded(
            (Rectangle){ x, y, rectWidth, rectHeight },
            0.5f, 6, button.barColor
        );
    }
}

int drawOptionButtons(Rectangle menuBox){
    Vector4 optionMeasures = {700, 250, 200, 50};
    
    optionButton buttons[4];
    int paddingYButton=90, gapText=50, paddingYText=90;

    const char *menuOptions[] = {"Retornar (TAB)", "Salvar (S)", "Carregar (C)", "Sair (Q)"};

    for(int i=0;i<4;i++){
        buttons[i]=(optionButton){(Rectangle){optionMeasures.x, optionMeasures.y+paddingYButton*i, optionMeasures.z, optionMeasures.w}, BUTTONBASE, BUTTONHOVER,i};

        Color optionColor = isOptionButtonHovered(buttons[i]) ? buttons[i].colorHover : buttons[i].colorBase;

        DrawRectangleRounded(buttons[i].optionBox, 0.2f, 10, optionColor);
        DrawRectangleRoundedLinesEx(buttons[i].optionBox, 0.2f, 10, 3, BUTTONBAR);

        Vector2 optionSize = MeasureTextEx(GetFontDefault(), menuOptions[i], 18, 1);
        DrawTextEx(GetFontDefault(), menuOptions[i], (Vector2){menuBox.x + (menuBox.width - optionSize.x) / 2, menuBox.y + gapText + (50 - optionSize.y) / 2}, 18, 1, BUTTONBAR);
        gapText+=paddingYText;
    }
    return isOptionButtonClicked(buttons);
}

int drawOpenedMenu() {

    Rectangle menuBox = {650, 200, 300, 400};
    DrawRectangle(0, 0, LARGURA, ALTURAHUD, (Color){0, 20, 60, 150});
    DrawRectangleRounded(menuBox, 0.1f, 10, (Color){ 30, 80, 255, 255 });

    Vector2 textSize = MeasureTextEx(GetFontDefault(), "Menu", 18, 1);
    DrawTextEx(GetFontDefault(), "Menu", (Vector2){menuBox.x + (menuBox.width - textSize.x) / 2, menuBox.y + (50 - textSize.y) / 2}, 18, 1, RAYWHITE);

    return drawOptionButtons(menuBox);
 
}

int drawSaveStates(){
    Rectangle saveBox = {600, 175, 400, 450};
    DrawRectangleRounded(saveBox, 0.1f, 10, (Color){ 30, 80, 255, 255 });
    int saveRecx = 75;
    int saveRecy = 50;
    int savePicOffset = 50;
    int padding = 75;
    Rectangle savePic[3];

    int textOffsetX = saveRecx + 20;
    int textOffsetY = 10;
    int saveButtonClicked = -1;
    for(int i = 0; i < 3; i++){
        savePic[i] = (Rectangle){saveBox.x + savePicOffset, saveBox.y + padding*(i+1) + saveRecy*i, saveRecx, saveRecy};
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
        if(CheckCollisionPointRec(mousePos, savePic[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            saveButtonClicked = i;
    }
    return saveButtonClicked;
}



Vector2 drawMenu(menuButton button, bool* menuOpen, bool* saveMenuOpen){
    int optionButtonClicked;
    int saveButtonClicked;
    if(isMenuButtonClicked(button) || IsKeyPressed(KEY_TAB)){
        *menuOpen = !(*menuOpen);
    }
    if(IsKeyPressed(KEY_S))
        *saveMenuOpen = !(*saveMenuOpen); 
    if(*menuOpen){
        optionButtonClicked = drawOpenedMenu();
        if(*saveMenuOpen)
            saveButtonClicked = drawSaveStates();
        else{
            switch(optionButtonClicked){
                case 0:
                    *menuOpen = !(*menuOpen);
                    break;
                
                case 1:
                    *saveMenuOpen = !(*saveMenuOpen);
                    break;

                case 2:     
                    *saveMenuOpen = !(*saveMenuOpen);
                    break;
                    
                case 3:
            }
        }
    }
    if(!(*menuOpen))
        *saveMenuOpen = false;
    return (Vector2){optionButtonClicked, saveButtonClicked};
}

