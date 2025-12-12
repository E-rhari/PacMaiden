#include <stdbool.h>
#include "raylib.h"

#include "../System/WindowControl.h"


#pragma once


/** @brief Struct que compoem os elementos básicos dos botões da tela de inicio */
typedef struct {
    Rectangle optionBox;
    Color colorBase;
    Color colorHover;
    Scenes id;
} titleButton;

/** @brief alterna entre comandos do controle e comandos do mause */
bool isMouseMode() {
    static bool isMouseMode = true;
    if (isMouseMode){
        if(GetGamepadButtonPressed())
            isMouseMode = false;
    } else if(GetMouseDelta().x || GetMouseDelta().y)
        isMouseMode = true;
    
    return isMouseMode;
}

bool isTitleButtonHovered(titleButton button, int buttonSelected){
    if (isMouseMode()){
        Vector2 mousePosition = GetMousePosition();
        return CheckCollisionPointRec(mousePosition,button.optionBox);
    } else{
        return (button.id == buttonSelected);
    }
}

void isTitleButtonClicked(titleButton *button, int buttonSelected){
    if (isMouseMode()){
        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            return;}
    } else if (!IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        return;
    for(int i=0;i<3;i++)
        if(isTitleButtonHovered(button[i], buttonSelected))
            changeScene(button[i].id);
}

void gamepadNav(int* buttonSelected){
    *buttonSelected += IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) - IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    if (*buttonSelected >= 2)
        *buttonSelected = 2;
    else if (*buttonSelected <= 0)
        *buttonSelected = 0;
}

/** @brief Desenha a tela de inicio e realiza animações que contem ela. Muito de seus parametros são partes que são animadas*/
void drawTitleScreen(Texture pacmaidenIllustration, Vector2* pacmaidenIllustrationPosition, Texture title, Vector2* titlePosition, Texture ghostPartsBack[], Texture ghostPartsFront[], int ghostPartsAmout, Vector2* ghostPartsPosition,  ProceduralAnimation* animation) {
    ClearBackground(BLACK);
    Rectangle source = {0, 0, 200, 105};
    Rectangle dest;

    for(int i=0; i<ghostPartsAmout; i++){
        sineWiggle(animation, &ghostPartsPosition[i], (Vector2){0,0}, 0.1f, 30, 4.0f*(i+1)*pow(-1, i), (Vector2){0,1});
        dest = (Rectangle){ghostPartsPosition[i].x, ghostPartsPosition[i].y, WIDTH, HEIGHT + HUDHEIGHT};
        DrawTexturePro(ghostPartsBack[i], source, dest, (Vector2){0,0}, 0, WHITE);
    }

    sineWiggle(animation, pacmaidenIllustrationPosition, (Vector2){0,10}, 0.1f, 10, 1.0f, (Vector2){0,1});
    dest = (Rectangle){pacmaidenIllustrationPosition->x, pacmaidenIllustrationPosition->y, WIDTH, HEIGHT + HUDHEIGHT};
    DrawTexturePro(pacmaidenIllustration, source, dest, (Vector2){0,0}, 0, WHITE);
    
    sineWiggle(animation, titlePosition, (Vector2){0,20}, -0.1f, 10, 0, (Vector2){0,1});
    dest = (Rectangle){titlePosition->x, titlePosition->y, WIDTH, HEIGHT + HUDHEIGHT};
    DrawTexturePro(title, source, dest, (Vector2){0,0}, 0, WHITE);

    for(int i=0; i<ghostPartsAmout; i++){
        dest = (Rectangle){ghostPartsPosition[i].x, ghostPartsPosition[i].y, WIDTH, HEIGHT + HUDHEIGHT};
        DrawTexturePro(ghostPartsFront[i], source, dest, (Vector2){0,0}, 0, WHITE);
    }

    static int buttonSelected = -1;
    titleButton buttons[3];
    char *optionsText[] = {"New Game", "Load", "PVP"};
    Vector4 optionMeasures = {1000, 350, 300, 100};
    int gapY = 10;
    Color textColor;
    Color optionColor;
    
    gamepadNav(&buttonSelected);

    for (int i = 0; i < 3; i++) {
        float posY = optionMeasures.y + (optionMeasures.w + gapY) * i;

        buttons[i] = (titleButton){
            (Rectangle){optionMeasures.x, posY, optionMeasures.z, optionMeasures.w},
            BLACK,
            WHITE,
            i
        };
        if(isTitleButtonHovered(buttons[i], buttonSelected)){
            optionColor = buttons[i].colorHover;
            textColor = BLACK;
        }
        else{
            optionColor = buttons[i].colorBase;
            textColor = WHITE;
        }

        DrawRectangleRounded(buttons[i].optionBox, 0.2f, 10, optionColor);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), optionsText[i], 50, 5);
        DrawTextEx(GetFontDefault(), optionsText[i],(Vector2){optionMeasures.x + (optionMeasures.z - textSize.x) / 2,posY + (optionMeasures.w - textSize.y) / 2}, 50, 5, textColor);
    }

    isTitleButtonClicked(buttons, buttonSelected);
}

/** @brief função base da tela de inicio*/
void titleScreen(){
    Music titleTheme = LoadMusicStream(getFilePath("../../audio/Music/Title/Title.wav"));
    PlayMusicStream(titleTheme);

    Color fadeColor = BLACK;
    ProceduralAnimation fadeAnimation = {GetTime(), true};

    ProceduralAnimation titleAnimation = {GetTime(), true};

    Texture pacmaidenIlustration = LoadTexture(getFilePath("../../sprites/title/PacmaidenTitle.png"));
    Vector2 pacmaidenIllustrationPosition = {0,0};
    Texture title = LoadTexture(getFilePath("../../sprites/title/Title.png"));
    Vector2 titlePosition = {0,0};

    int ghostPartsAmount = 4;
    Texture ghostPartsBack[ghostPartsAmount];
    Texture ghostPartsFront[ghostPartsAmount];
    Vector2 ghostPartsPosition[ghostPartsAmount];
    for(int i=0; i<ghostPartsAmount; i++){
        char path[100];
        sprintf(path, getFilePath("../../sprites/title/DeadGhostParts/Ghost%dBack.png"), i);
        ghostPartsBack[i] = LoadTexture(path);
        sprintf(path, getFilePath("../../sprites/title/DeadGhostParts/Ghost%dFront.png"), i);
        ghostPartsFront[i] = LoadTexture(path);
        ghostPartsPosition[i] = (Vector2){0,0};
    }

    while(currenctScene == TITLE){
        UpdateMusicStream(titleTheme);

        if(fadeAnimation.running)
            fadeOut(&fadeColor, &fadeAnimation, 2.0f);

        BeginDrawing();
        drawTitleScreen(pacmaidenIlustration, &pacmaidenIllustrationPosition, title, &titlePosition, ghostPartsBack, ghostPartsFront, ghostPartsAmount, ghostPartsPosition, &titleAnimation);
        DrawRectangle(0,0, WIDTH, HEIGHT+HUDHEIGHT, fadeColor);
        EndDrawing();
    }
    fadeAnimation = (ProceduralAnimation){GetTime(), true};
    while(fadeAnimation.running){
        fadeIn(&fadeColor, &fadeAnimation, 2.0f);

        UpdateMusicStream(titleTheme);

        BeginDrawing();
        drawTitleScreen(pacmaidenIlustration, &pacmaidenIllustrationPosition, title, &titlePosition, ghostPartsBack, ghostPartsFront, ghostPartsAmount, ghostPartsPosition, &titleAnimation);
        DrawRectangle(0,0, WIDTH, HEIGHT+HUDHEIGHT, fadeColor);
        EndDrawing();
    }
    WaitTime(1);

    StopMusicStream(titleTheme);
    UnloadMusicStream(titleTheme);
    
    UnloadTexture(pacmaidenIlustration);
    UnloadTexture(title);
    for(int i=0; i<ghostPartsAmount; i++){
        UnloadTexture(ghostPartsBack[i]);
        UnloadTexture(ghostPartsFront[i]);
    }
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
        DrawRectangleRoundedLinesEx(savePic[i], 0.1f, 10, 3, (Color){100, 180, 255, 255});

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