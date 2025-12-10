/** @brief Estado de jogo em que ocorre a gameplay propriamente dita. */
#include "../Characters/Character.h"
#include "../Characters/PacMaiden.h"
#include "../Characters/Ghosts/Ghost.h"
#include "../System/WindowControl.h"
#include "../System/Input.h"
#include "../System/Audio.h"
#include "../Map/Map.h"
#include "Menu.h"
#include "Cutscenes.h"

#include "../Characters/Ghosts/StalkerGhost.h"
#include "../Characters/Ghosts/AwareGhost.h"
#include "../Characters/Ghosts/UnawareGhost.h"
#include "../Characters/Ghosts/AmbusherGhost.h"

#pragma once



Ghost* instantiateGhostsInLevel(Map map){
    Ghost *ladies = malloc(sizeof(Ghost)*4); 
    Vector2* positions = searchInMap(map, 'f');

    ladies[0] = initGhost(positions[0], RADIUS, SPEED, RED, STALKER); //homura
    ladies[1] = initGhost(positions[1], RADIUS, SPEED, SKYBLUE, AMBUSHER);//sora
    ladies[2] = initGhost(positions[2], RADIUS, SPEED, ORANGE, AWARE);//hikari
    ladies[3] = initGhost(positions[3], RADIUS, SPEED, PINK, AWARE);//hana

    return ladies;
}


void drawCharacters(PacMaiden* pacmaiden, Ghost* ghosts){
    // DrawCircleV(pacmaiden->chara.circle.center, pacmaiden->chara.circle.radius, pacmaiden->chara.color);
    drawCharacterSprite(&pacmaiden->chara);
    for(int i=0; i<4; i++){
        // DrawCircleV(ghosts[i].chara.circle.center, ghosts[i].chara.circle.radius, ghosts[i].chara.color);
        drawCharacterSprite(&ghosts[i].chara);
    }
}


void drawHud(PacMaiden* pacmaiden){
    DrawRectangle(0, 800, WIDTH, (int)GRID2PIX, BLACK);
    DrawText(TextFormat("Pontuação: %d", pacmaiden->points), SCOREPOSY, HEIGHT, SCORESIZE, RAYWHITE);
    for(int i=0; i<pacmaiden->lifes; i++)
        DrawCircle(WIDTH-(i+1)*(20)-(i*20), HEIGHT+20, 20, pacmaiden->initialValues.color);
}


void drawMenu(OptionButton* buttons, Rectangle* saveOptions){
    if(gameState==PAUSED)
        drawOpenedMenu(buttons);
    if(gameState==SAVING || gameState==LOADING)
        drawSaveStates(saveOptions);
}


/** @brief Desenha todas as coisas do jogo. */
void drawLevel(Map map, Vector2** mapCellPosInSprite, PacMaiden* pacmaiden, Ghost* ghosts, OptionButton* buttons, Rectangle* saveOptions){
    ClearBackground(BLACK);
    drawMap(map, mapCellPosInSprite);
    drawCharacters(pacmaiden, ghosts);
    drawHud(pacmaiden);
    drawMenu(buttons, saveOptions);
}

/** @brief Desenha todas as coisas do jogo e fecha a rotina de desenhar, impedindo modificações futuras na tela sem apagá-la por completo */
void restricDrawLevel(Map map, Vector2** mapCellPosInSprite, PacMaiden* pacmaiden, Ghost* ghosts, OptionButton* buttons,Rectangle* saveOptions){
    BeginDrawing();
    drawLevel(map, mapCellPosInSprite, pacmaiden, ghosts, buttons, saveOptions);
    EndDrawing();
}


/** @brief Realiza todas as funções de movimento dos personagens */
void charactersBehaviours(PacMaiden* pacmaiden, Ghost* ghosts, Map map,int *pallets, Sound* effects){
    if(pacmaiden->state == DYING){
        fadeOut(&pacmaiden->chara.sprite.tint, &pacmaiden->chara.procAnimation, 3);
        if(!pacmaiden->chara.procAnimation.running)
            changePacmaidenState(pacmaiden, IMMORTAL);
        return;
    }

    killerTime(pacmaiden,5);

    getBufferedInput(&pacmaiden->chara.moveDirection, isCharacterInGridCenter(pacmaiden->chara)
                                                   && isCharacterInsideScreen(pacmaiden->chara, (Vector2){0,0}),0,&pacmaiden->bufferedInput);
    pacmaidenBehaviour(pacmaiden, map);
    for(int i=0; i<4; i++){
        ghostBehaviour(&ghosts[i], map, pacmaiden, &effects[EAT_GHOST]);
        chooseDestinationByType(&ghosts[i], map, pacmaiden);
    }


    countPoints(pacmaiden, map, charCollided(*pacmaiden, map), pallets, effects);
}


/** @brief Pause ou despausa o jogo caso o jogador aperte tab */
void gamePause(){
    if(IsKeyPressed(KEY_TAB)){
        if(gameState==PAUSED)    
            gameState=RUNNING;
        else 
            gameState=PAUSED;
    }
}


/** @brief Roda todo frame. */
void update(PacMaiden* pacmaiden, Vector2** mapCellPosInSprite, Ghost* ghosts, Map map, OptionButton* buttons, Rectangle *saveOptions, Music* tracks, Sound* effects){
    int fileNumber;
    int pallets = countPallets(map);

    while(!WindowShouldClose()){
        fileNumber=-1;
        if(DEBUG_MODE)
            userClose();

        restricDrawLevel(map, mapCellPosInSprite, pacmaiden, ghosts, buttons, saveOptions);

        if(pacmaiden->state == DEAD){
            gameState = GAMEOVER;
            changeScene(TITLE);
            return;
        }
        if(pallets<=0){
            changeScene(NEXT);
            return;
        }

        gamePause();

        handleMusic(tracks, gameState!=RUNNING);

        switch (gameState)
        {
            case PAUSED:
                isOptionButtonClicked(buttons, GetKeyPressed());
            break;
            case RUNNING:
                charactersBehaviours(pacmaiden, ghosts, map, &pallets, effects);
            break;
            case SAVING:
                fileNumber=isSaveFileClicked(saveOptions);
                if(fileNumber!=-1)
                    save(map,*pacmaiden,ghosts,fileNumber);
            break;
            case LOADING:
                fileNumber=isSaveFileClicked(saveOptions);
                if(fileNumber!=-1)
                    load(map,pacmaiden,ghosts,fileNumber);
            break;
            case EXIT:
                changeScene(TITLE);
                return;
            break;
        }
    }
}


/** @brief Roda a fase desejada */
void level(int levelNumber){
    bool isSinglePlayer = true;

    PacMaiden pacmaiden;
    Ghost* ghosts;
    Map map=setUpMap();

    if(currenctScene==LOAD){
        gameState = RUNNING;
        ghosts = malloc(sizeof(Ghost)*4);
        load(map, &pacmaiden, ghosts, levelNumber);
    }
    else if(currenctScene==NEWGAME || currenctScene==NEXT){
        gameState=STARTING;
        readMap(levelNumber,map);
        pacmaiden = initPacMaiden(searchInMap(map, 'P')[0], RADIUS, SPEED, YELLOW, 3, 0);
        ghosts=instantiateGhostsInLevel(map);
        changePacmaidenState(&pacmaiden, IMMORTAL);
    }
    Vector2** mapCellPosInSprite = decideMapCellsSprite(map);
    OptionButton *buttons = malloc(sizeof(OptionButton)*4);
    initOptionButton(buttons);
    Rectangle* saveOptions = malloc(sizeof(Rectangle)*3);
    initSaveButton(saveOptions);

    Music tracks[SONG_AMOUT];
    initiateMusic(tracks); 
    focusTrack(tracks, MAIN_THEME);

    Sound effects[SOUND_AMOUNT];
    initiateSFX(effects);

    if(gameState == STARTING)
        gameStartCutscene(&pacmaiden, mapCellPosInSprite, ghosts, map, false);
    update(&pacmaiden, mapCellPosInSprite, ghosts,map,buttons,saveOptions, tracks, effects);
    if(gameState == GAMEOVER)
        gameOverCutscene(&pacmaiden, mapCellPosInSprite, ghosts, map, false);
        
    
    freeMusic(tracks);
    freeMap(map);
    free(ghosts);
    free(saveOptions);
    free(buttons);

}
