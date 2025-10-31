#include "raylib.h"
#include <stdbool.h>

#define BUTTONBASE (Color){10, 40, 80, 255}
#define BUTTONHOVER (Color){20, 70, 140, 255}
#define BUTTONBAR (Color){100, 180, 255, 255}
#pragma once

typedef struct MenuButton {
    Vector2 center;
    float radius;
    Color colorBase;
    Color colorHover;
    Color barColor;
} menuButton;

typedef struct{
    Rectangle optionBox;
    Color colorBase;
    Color colorHover;
} optionButton;

bool isMenuButtonHovered(menuButton button) {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointCircle(mousePos, button.center, button.radius);
}

bool isMenuButtonClicked(menuButton button) {
    return isMenuButtonHovered(button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
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

bool isOptionButtonHovered(optionButton button) {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, button.optionBox);
}

void drawOpenedMenu(void) {
    Rectangle menuBox = {650, 200, 300, 400};
    DrawRectangleRounded(menuBox, 0.1f, 10, (Color){ 30, 80, 255, 150 });
    DrawRectangle(0, 0, LARGURA, ALTURAHUD, (Color){0, 20, 60, 10});
    Vector2 textSize = MeasureTextEx(GetFontDefault(), "Menu", 18, 1);
    DrawTextEx(GetFontDefault(), "Menu", (Vector2){menuBox.x + (menuBox.width - textSize.x) / 2, menuBox.y + (50 - textSize.y) / 2}, 18, 1, RAYWHITE);
    optionButton buttons[] = {(optionButton){(Rectangle){700, 250, 200, 50}, BUTTONBASE, BUTTONHOVER}, (optionButton){(Rectangle){700, 340, 200, 50}, BUTTONBASE, BUTTONHOVER}, (optionButton){(Rectangle){700, 430, 200, 50}, BUTTONBASE, BUTTONHOVER}, (optionButton){(Rectangle){700, 520, 200, 50}, BUTTONBASE, BUTTONHOVER}};
    for(int i = 0; i < 4; i++){
        bool hovered = isOptionButtonHovered(buttons[i]);
        Color optionColor = hovered ? buttons[i].colorHover : buttons[i].colorBase;
        DrawRectangleRec(buttons[i].optionBox, optionColor);
    }
    int gap = 50;
    const char *menuOptions[] = {"Retornar (TAB)", "Salvar (S)", "Carregar (C)", "Sair (Q)"};
    for(int i = 0; i < 4; i++){
        Vector2 optionSize = MeasureTextEx(GetFontDefault(), menuOptions[i], 18, 1);
        DrawTextEx(GetFontDefault(), menuOptions[i], (Vector2){menuBox.x + (menuBox.width - optionSize.x) / 2, menuBox.y + gap + (50 - optionSize.y) / 2}, 18, 1, RAYWHITE);
        gap += 90;
    }
}

void drawMenu(menuButton button, bool* menuOpen){
    if(isMenuButtonClicked(button) || IsKeyPressed(KEY_TAB))
        *menuOpen = !(*menuOpen);
    
    if(*menuOpen)
        drawOpenedMenu();
}
