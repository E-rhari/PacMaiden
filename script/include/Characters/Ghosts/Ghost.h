#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

#include "../Character.h"
#include "../PacMaiden.h"
#include "../../Map/Map.h"
#include "../../Map/AStar.h"
#include "../../System/Audio.h"

#pragma once


typedef enum {
    SPOOKY,
    VULNERABLE,
    SPAWNING
} GhostState;


typedef enum {
    STALKER,
    AMBUSHER,
    GREEDY,
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
    Character chara = initCharacter((Vector2){position.x, position.y}, speed, radius, color, RED_GHOST_SPRITE+type);
    return (Ghost){chara, chara, true, SPOOKY, type};
}


bool choseDestinationUnaware(Ghost* ghost);
bool chooseDestinationAware(Ghost* ghost, Map map);
NodeList stalkPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden);
void ambushPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden, int blocksAhead);
void getState(Ghost* ghost, Map map);



void changeGhostState(Ghost* ghost, GhostState state){
    ghost->state = state;
    ghost->chara.sprite.tint = ghost->initialValues.sprite.tint;
    ghost->chara.procAnimation.initTime = GetTime();

    switch(state){
        case SPOOKY:
                    ghost->chara.color = ghost->initialValues.color;
                    ghost->chara.speed = ghost->initialValues.speed;
                    changeSprite(&ghost->chara.sprite, RED_GHOST_SPRITE+ghost->type);
                    break;
        case VULNERABLE:
                    ghost->chara.speed = ghost->initialValues.speed/2;
                    changeSprite(&ghost->chara.sprite, WAEK_GHOST_SPRITE);
                    ghost->chara.sprite.tint = GRAY;
                    break;
        case SPAWNING:
                    changeSprite(&ghost->chara.sprite, RED_GHOST_SPRITE+ghost->type);
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
void ghostAttackPacmaiden(PacMaiden* pacmaiden, Ghost* ghost, Map map, Sound deathEffect){
    if(pacmaiden->state != IMMORTAL)
        if(checkCharacterCollision(pacmaiden->chara, ghost->chara))
            hurtPacmaiden(pacmaiden, deathEffect);
}


void flee(Ghost* ghost, Map map, PacMaiden* pacmaiden){
    if(currenctScene==PVP){
        for(int i=0;i<2;i++){
            if(pacmaiden[i].state==KILLER)
                recklessEscape(&ghost->chara, pacmaiden[i].chara, map);
            else
                chooseDestinationAware(ghost, map);
            return;
        }
    }
    else{
        if(pacmaiden->state==KILLER)
            recklessEscape(&ghost->chara, pacmaiden->chara, map);
        else
            chooseDestinationAware(ghost, map);
        return;   
    }
}


void chooseDestinationByType(Ghost* ghost, Map map, PacMaiden* pacmaiden){
    if(ghost->type==STALKER)
        ghost->canChooseDestination=true;

    if(ghost->canChooseDestination && isCharacterInGridCenter(ghost->chara)){
        if(ghost->state == VULNERABLE){
            flee(ghost,map,pacmaiden);
            return;
        }
        switch (ghost->type){
            case UNAWARE:   choseDestinationUnaware(ghost); break;
            case AWARE:     chooseDestinationAware(ghost, map); break;
            case STALKER:   stalkPacmaiden(ghost, map, pacmaiden); break;
            case AMBUSHER:  ambushPacmaiden(ghost, map, pacmaiden, 4); break;
            case GREEDY:    getState(ghost, map); break;
        }
        ghost->canChooseDestination = false;
    }

    if (!isCharacterInGridCenter(ghost->chara))
        ghost->canChooseDestination=true;
        
}


/** @brief Todas as ações de comportamento de um fantasma genérico que devem ser rodadas por frame */
void ghostBehaviour(Ghost* ghost, Map map, PacMaiden* pacmaiden, Sound* effects){
    if(ghost->state == SPAWNING){
        spriteBlinkAnimation(&ghost->chara.sprite.spriteSheet, ghost->initialValues.sprite.spriteSheet , ghost->chara.sprite.mask, &ghost->chara.procAnimation, 5.0f, 1.0f/10.0f, 3.0f);
        if(!ghost->chara.procAnimation.running)
            changeGhostState(ghost, SPOOKY);
        return;
    }

    move(&ghost->chara, map);
    portalBorders(&ghost->chara);

    if(checkPowerPellet(pacmaiden, map))
        changeGhostState(ghost, VULNERABLE);

    if(ghost->state == VULNERABLE){
        spriteBlinkAnimation(&ghost->chara.sprite.spriteSheet, SPRITES[WAEK_GHOST_SPRITE] , ghost->chara.sprite.mask, &ghost->chara.procAnimation, 5.0f, 1.0f/300.0f, 5.0f);
        if(!ghost->chara.procAnimation.running)
            changeGhostState(ghost, SPOOKY);
        
        if(checkCharacterCollision(pacmaiden->chara, ghost->chara) && pacmaiden->state==KILLER){
            PlaySound(effects[EAT_GHOST]);
            hurtGhost(ghost);
            addPoints(pacmaiden, 100);
        }
    }
    else
        ghostAttackPacmaiden(pacmaiden, ghost, map, effects[DEATH]);
}

PacMaiden chooseClosestPacMaiden(Ghost *ghost, PacMaiden* players,Map map){
            
    PacMaiden chosenPacmaiden = players[0];

    NodeList path;
    path.start=NULL;
    path.size=1000000;

    for(int i=0;i<2;i++){

        NodeList temp;
        if(players[i].state!=NORMAL)
            continue;
        
        temp = findPath(vector2ToGridVector(ghost->chara.circle.center), vector2ToGridVector(players[i].chara.circle.center), map);
        
        if(path.size>temp.size){
            path=temp;
            chosenPacmaiden = players[i];
        }
    }

    return chosenPacmaiden;
} 
