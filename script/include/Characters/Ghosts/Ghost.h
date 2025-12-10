#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

#include "../Character.h"
#include "../PacMaiden.h"
#include "../../Map/Map.h"
#include "../../Map/AStar.h"

#pragma once


typedef enum {
    SPOOKY,
    VULNERABLE,
    SPAWNING
} GhostState;


typedef enum {
    STALKER,
    AMBUSHER,
    AWARE,
    UNAWARE,
} GhostType;


/** @brief Inimigos do jogador */
typedef struct TGhost{
    Character chara;
    Character initialValues;
    bool canChooseDestination;
    GhostState state;
    GhostType type;
} Ghost;


/** @brief Cria a instância do fantasma
 * 
 * @param position Vector de posição.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param color Cor do personagem a partir das definições da Raylib. */
Ghost initGhost(Vector2 position, int radius, float speed, Color color, GhostType type){
    Character chara = initCharacter((Vector2){position.x, position.y}, speed, radius, color, TextFormat("../../sprites/ghosts/Ghost%d.png", type));
    return (Ghost){chara, chara, true, SPOOKY, type};
}


bool choseDestinationUnaware(Ghost* ghost);
bool chooseDestinationAware(Ghost* ghost, Map map);
NodeList stalkPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden);
void ambushPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden, int blocksAhead);



void changeGhostState(Ghost* ghost, GhostState state){
    ghost->state = state;
    ghost->chara.sprite.tint = ghost->initialValues.sprite.tint;
    ghost->chara.procAnimation.initTime = GetTime();

    switch(state){
        case SPOOKY:
                    ghost->chara.color = ghost->initialValues.color;
                    ghost->chara.speed = ghost->initialValues.speed;
                    changeSprite(&ghost->chara.sprite, getFilePath(TextFormat("../../sprites/ghosts/Ghost%d.png", ghost->type)), true);
                    break;
        case VULNERABLE:
                    ghost->chara.speed = ghost->initialValues.speed/2;
                    changeSprite(&ghost->chara.sprite, getFilePath("../../sprites/ghosts/WeakGhost.png"), true);
                    ghost->chara.sprite.tint = GRAY;
                    break;
        case SPAWNING:
                    ghost->canChooseDestination = true;
                    break;
    }
}


/** @brief Mata o fantasma, reinicializando-o */
void hurtGhost(Ghost* ghost){
    ghost->chara.circle.center = ghost->initialValues.circle.center;
    changeGhostState(ghost, SPAWNING);
}


/** @brief Trata de toda a colisão da pacmaiden com os fantasmas, levando em consideração o seu estado e posição */
void ghostAttackPacmaiden(PacMaiden* pacmaiden, Ghost* ghost, Map map){
    if(pacmaiden->state != IMMORTAL)
        if(checkCharacterCollision(pacmaiden->chara, ghost->chara))
            hurtPacmaiden(pacmaiden);
}


void chooseDestinationByType(Ghost* ghost, Map map, PacMaiden* pacmaiden){
    if(ghost->canChooseDestination && isCharacterInGridCenter(ghost->chara)){
        if(ghost->state == VULNERABLE){
            recklessEscape(&ghost->chara, pacmaiden->chara, map);
            return;
        }
        switch (ghost->type){
            case UNAWARE:   choseDestinationUnaware(ghost); break;
            case AWARE:     chooseDestinationAware(ghost, map); break;
            case STALKER:   stalkPacmaiden(ghost, map, pacmaiden); break;
            case AMBUSHER:  ambushPacmaiden(ghost, map, pacmaiden, 4); break;
        }
        ghost->canChooseDestination = false;
    }

    if (!isCharacterInGridCenter(ghost->chara))
        ghost->canChooseDestination=true;
}


/** @brief Todas as ações de comportamento de um fantasma genérico que devem ser rodadas por frame */
void ghostBehaviour(Ghost* ghost, Map map, PacMaiden* pacmaiden, Sound dyingEffect){
    if(ghost->state == SPAWNING){
        spriteBlinkAnimation(&ghost->chara.sprite.spriteSheet, ghost->initialValues.sprite.spriteSheet , ghost->chara.sprite.mask, &ghost->chara.procAnimation, HURT_COOLDOWN, 1.0f/4.0f, 5.0f);
        if(!ghost->chara.procAnimation.running)
            changeGhostState(ghost, SPOOKY);
        return;
    }

    chooseDestinationByType(ghost, map, pacmaiden);
    move(&ghost->chara, map);
    portalBorders(&ghost->chara);

    if(checkPowerPellet(pacmaiden, map))
        changeGhostState(ghost, VULNERABLE);

    if(ghost->state == VULNERABLE){
        spriteBlinkAnimation(&ghost->chara.sprite.spriteSheet, ghost->initialValues.sprite.spriteSheet , ghost->chara.sprite.mask, &ghost->chara.procAnimation, 5.0f, 1.0f/300.0f, 5.0f);
        if(!ghost->chara.procAnimation.running)
            changeGhostState(ghost, SPOOKY);
        
        if(checkCharacterCollision(pacmaiden->chara, ghost->chara) && pacmaiden->state==KILLER){
            PlaySound(dyingEffect);
            hurtGhost(ghost);
            addPoints(pacmaiden, 100);
        }
    }
    else
        ghostAttackPacmaiden(pacmaiden, ghost, map);
}
