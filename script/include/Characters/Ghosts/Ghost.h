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
    SPOOKY,     // Capaz de dar dano
    VULNERABLE, // Capaz de levar dano
    SPAWNING    // Parado, imortal e inofensivo
} GhostState;


typedef enum {
    STALKER,    // Persegue a PacMaiden através do A*
    AMBUSHER,   // Encurrala a PacMaiden através do A*
    GREEDY,     // Fica na região com mais pellets do jogo
    AWARE,      // Se move aleatoriamente, mas sem voltar pra trás sem necessidade ou bater de cara na parede
    UNAWARE,    // Se move aleatoriamente sem se importar com nada
} GhostType;


/** @brief Inimigos do jogador.
 * @param chara Componente de movimentação, colisão e representação visaual do personagem
 * @param initialValues Os valoreis iniciais da propriedade chara.
 * @param canChooseDestination Restringe o momento que o fantasma pode escolher uma direção, 
 *                             garantido que ele escolha apenas uma direção por chegada no centro do grid.
 * @param state Condição atual do fantasma.
 * @param type Determina o comportamento e visual do fantasma. */
typedef struct TGhost{
    Character chara;
    Character initialValues;
    bool canChooseDestination;
    GhostState state;
    GhostType type;
} Ghost;


/** @brief Cria a instância do fantasma
 * @param position Vector de posição.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move. 
 * @param color [legacy] Cor do círculo do personagem.
 * @param type Determina o comportamento e visual do fantasma. */
Ghost initGhost(Vector2 position, int radius, float speed, Color color, GhostType type){
    Character chara = initCharacter((Vector2){position.x, position.y}, speed, radius, color, RED_GHOST_SPRITE+type);
    return (Ghost){chara, chara, true, SPOOKY, type};
}


/** @brief Desaloca o fantasma e descarrega seus recursos. */
void freeGhosts(Ghost* ghosts, int amount){
    for(int i=0; i<amount; i++)
        UnloadTexture(ghosts[i].chara.sprite.mask);
    free(ghosts);
}

// Comportamento dos fantasmas. Essas funções estão definidas em seus respectivos arquivos. Foram declaradas aqui para evitar importação circular. 

bool choseDestinationUnaware(Ghost* ghost);
bool chooseDestinationAware(Ghost* ghost, Map map);
NodeList stalkPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden);
void ambushPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden, int blocksAhead);
void moveGreedy(Ghost* ghost, Map map);


/** @brief Muda o estado do fantasma, aplicando as modificações necessárias. Use essa função no lugar de alterar a propriedade state manualmente */
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

/** @brief Faz o fantasma se movimentar da direção oposta que o pacman se encontra, se possível */
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


/** @brief Executa a função de comportamente de acordo com o tipo do fantasma */
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
            case GREEDY:    moveGreedy(ghost, map); break;
        }
        ghost->canChooseDestination = false;
    }

    if (!isCharacterInGridCenter(ghost->chara))
        ghost->canChooseDestination=true;
        
}


/** @brief Função para o PVP. Busca qual pacman está mais proximo */
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