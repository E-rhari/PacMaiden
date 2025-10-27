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

void drawOpenedMenu(void) {
    Rectangle menuBox = {650, 200, 300, 400};
    DrawRectangleRounded(menuBox, 0.1f, 10, (Color){ 20, 40, 80, 230 });
    DrawText("Menu", menuBox.x + (menuBox.x / 5) , menuBox.y + 40, 20, RAYWHITE);
    Rectangle optionBox = {700, 300, 200, 50};
    for(int i = 0; i < 3; i++){
        DrawRectangleRounded(optionBox, 0.1f, 10, (Color){ 0, 0, 0, 230 });
        optionBox.y += 90;
    }

    DrawText("Retomar (TAB)", menuBox.x + 40, menuBox.y + 100, 18, RAYWHITE);
    DrawText("Salvar (S)", menuBox.x + 40, menuBox.y + 140, 18, RAYWHITE);
    DrawText("Sair (Q)", menuBox.x + 40, menuBox.y + 180, 18, RAYWHITE);
}

void drawMenu(menuButton button, bool* menuOpen){
    if(isMenuButtonClicked(button) || IsKeyPressed(KEY_TAB))
        *menuOpen = !(*menuOpen);
    
    if(*menuOpen)
        drawOpenedMenu();
}
