#include "raylib.h"
#include<stdbool.h>

#include "./Character.h"
#include "../Animations/SpriteAnimation.h"
//#include "../System/Audio.h"

#pragma once


#define HURT_COOLDOWN 3 // Tempo de invulnerabilidade da pacmaiden, em segundos.



/** @brief Possibilidades de estados da pacmaiden. Devem ser alterados usando changePacmaidenState() */
typedef enum {
    NORMAL,     // Capaz de morrer.
    DYING,      // Em animação de morte.
    DEAD,       // Alcançou 0 vidas.
    IMMORTAL,   // Incapaz de morrer. Spawnando.
    KILLER      // Capaz de matar.
} PacState;


/** @brief Personagem do jogador e protagonista do jogo
 * @param chara Componente de movimentação, colisão e representação visaual do personagem
 * @param initialValues Struct constante do tip Character para salver os valores iniciais de posição, tamanho, velocidade e cor.
 * @param life Contador de vidas.
 * @param points Contador de pontos.
 * @param timePivot (s) Marca um momento no tempo desde o início do jogo. Utilizado para a duração de efeitos.
 * @param bufferedInput Input salvo porém ainda não aplicado.
 * @param canMove Permite ou trava a movimentação da pacmaiden.
 * @param playerColor Valor do enum SpriteName que referência o sprite desejado no array de sprites
 * @param state Estado atual da pacmaiden. */
typedef struct {
    Character chara;
    Character initialValues;
    int lifes;
    int points;
    int timePivot;
    Vector2 bufferedInput;
    bool canMove;
    SpriteName playerColor;
    PacState state;

} PacMaiden;


/** @brief Cria a instância da Struct PacMaiden a partir dos parâmetros fornecidos.
 * @param position Vector de posição.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param color [legacy] Cor do personagem a partir das definições da Raylib.
 * @param lifes Valor inicial do contador de vidas
 * @param points Valor inicial do contador de pontos
 * @param spriteSheet Valor do enum SpriteName que referência o sprite desejado no array de sprites */
PacMaiden initPacMaiden(Vector2 position, int radius, float speed, Color color, int lifes, int points, SpriteName spriteSheet){
    Character chara = initCharacter((Vector2){position.x, position.y}, speed, radius, color, spriteSheet);
    return (PacMaiden){chara, chara, lifes, points, 0, (Vector2){0,0}, true, spriteSheet, NORMAL};
}


/** @brief Descarrega todos os recursos da PacMaiden */
void freePacmaiden(PacMaiden* pacmaiden){
    UnloadTexture(pacmaiden->chara.sprite.mask);
}


SpriteName checkColor(PacMaiden* pacmaiden){
    if(pacmaiden->playerColor==YELLOW_PACMAIDEN_SPRITE)
        return YELLOW_POWERED_PACMAIDEN_SPRITE;
    return PURPLE_POWERED_PACMAIDEN_SPRITE;
}

/** @brief Além de determinar o valor da propriedade state da pacmaiden, também realiza as operações devidas
 *        na transição entre os estados. Sempre use no lugar de determinar o valor de state manualmente. */
void changePacmaidenState(PacMaiden* pacmaiden, PacState state){
    pacmaiden->state = state;
    pacmaiden->chara.procAnimation.initTime = GetTime();
    pacmaiden->chara.sprite.spriteSheet = pacmaiden->initialValues.sprite.spriteSheet;

    switch (state){
        case NORMAL:
            changeSprite(&pacmaiden->chara.sprite, pacmaiden->playerColor);
            break;
        case KILLER:
            changeSprite(&pacmaiden->chara.sprite, checkColor(pacmaiden));
            break;
        case DYING:
            pacmaiden->canMove=false;
            pacmaiden->chara.procAnimation.initTime = GetTime();
            break;
        case DEAD: 
            break;

        case IMMORTAL:
            pacmaiden->chara.moveDirection = (Vector2){0,1};
            pacmaiden->chara.sprite.tint = pacmaiden->initialValues.sprite.tint;
            pacmaiden->canMove=true;
            pacmaiden->chara.moveDirection = (Vector2){0,0};
            pacmaiden->chara.procAnimation.initTime = GetTime();
            pacmaiden->chara.circle.center = pacmaiden->initialValues.circle.center;
            break;

        default:
            break;
    }
}


/** @brief Adiciona pontos para a pontuação da pacmaiden, executando as correções necessárias. */
void addPoints(PacMaiden* pacmaiden, int pointsToAdd){
    pacmaiden->points += pointsToAdd;
    if(pacmaiden->points < 0)
        pacmaiden->points = 0;
}


/** @brief Retorna o caractere no do mapa na posição de colisão do personagem
 * @return O caractere lido ou ' ' se a entrada for inválida. */
char charCollided(PacMaiden pacMaiden, Map map){
    Vector2 colliderBound = (Vector2){pacMaiden.chara.circle.center.x + pacMaiden.chara.circle.radius*pacMaiden.chara.moveDirection.x,
                                      pacMaiden.chara.circle.center.y + pacMaiden.chara.circle.radius*pacMaiden.chara.moveDirection.y};

    if(isPositionInGridCenter(colliderBound))
        return readPositionInMap(colliderBound, map, (Vector2){0,0});

    return ' ';
}
void killerTime(PacMaiden* pacMaiden,int duration){
    double timeElapsed = GetTime() - pacMaiden->chara.procAnimation.initTime;
    if(timeElapsed>=duration)
       changePacmaidenState(pacMaiden, NORMAL);    
}

/** @brief Verifica se a pacmaiden está tocando a Pellet */
bool checkPowerPellet(PacMaiden* pacmaiden, Map map){
    if(charCollided(*pacmaiden, map) == 'o'){
        changePacmaidenState(pacmaiden,KILLER);

        return true;
    }
    return false;
}


/** @brief Adiciona a quantidade adequada de pontos à pontuação da pacmaiden.
 * @param pacMaiden Personagem do jogador que receberá a pontuação.
 * @param map Mapa de onde será contado o ponto.
 * @param object Caractere que indica o tipo de objeto.
 * @param pallets Quantidade de pellets
 * @param effects Array de efeitos sonoros do jogo */
void countPoints(PacMaiden* pacMaiden, Map map, char object, int *pallets, Sound* effects){
    Vector2 colliderBound = (Vector2){pacMaiden->chara.circle.center.x + pacMaiden->chara.circle.radius*pacMaiden->chara.moveDirection.x,
                                      pacMaiden->chara.circle.center.y + pacMaiden->chara.circle.radius*pacMaiden->chara.moveDirection.y};
    Vector2 convertedPos = Vector2Scale(colliderBound, PIX2GRID);
    switch(object)
    {
        case '.':
            //SetSoundPitch(effects[0], .75 + ((float)(rand() % 5))/10);
            //PlaySound(effects[0]);
            addPoints(pacMaiden, 10);
            *pallets=*pallets-1;
            break;
        case 'o':
            PlaySound(effects[1]);
            addPoints(pacMaiden, 50);
            break;
        case 'B':
            addPoints(pacMaiden, 300);
            break;
        default:
            return;
    }
    map[(int)convertedPos.y][(int)convertedPos.x] = ' ';
}


/** @brief Dá dano a pacmaiden caso o cooldown seja obedecido.
 * @return Se a pacmaiden levou dano ou não */
bool hurtPacmaiden(PacMaiden* pacmaiden, Sound deathEffect){
    pacmaiden->lifes--;
    pacmaiden->timePivot = GetTime();
    addPoints(pacmaiden, -200);
    
    changePacmaidenState(pacmaiden, DYING);
    
    if(pacmaiden->lifes <= 0)
        changePacmaidenState(pacmaiden, DEAD);
    else
        PlaySound(deathEffect);
        
    return true;
}


void canPlayersMove(PacMaiden* players){
    Vector2 playerNewCenter[2] = {players[0].chara.circle.center,players[1].chara.circle.center};
    
    
    for(int i=0;i<2;i++){
        playerNewCenter[i].x+= players[i].chara.moveDirection.x*30;
        playerNewCenter[i].y+= players[i].chara.moveDirection.y*30;
    }
    if(!CheckCollisionCircles(playerNewCenter[0],players[0].chara.circle.radius,playerNewCenter[1],players[1].chara.circle.radius)){
        players[0].canMove=true;
        players[1].canMove=true;
    }
    
}


/** @brief Todas as ações de comportamento da PacMaiden que devem ser rodadas por frame */
void pacmaidenBehaviour(PacMaiden* pacmaiden, Map map){
    if(pacmaiden->canMove)
        move(&pacmaiden->chara, map);

    portalBorders(&pacmaiden->chara);
    
    if(pacmaiden->state == IMMORTAL){
        spriteBlinkAnimation(&pacmaiden->chara.sprite.spriteSheet, pacmaiden->initialValues.sprite.spriteSheet , pacmaiden->chara.sprite.mask, &pacmaiden->chara.procAnimation, HURT_COOLDOWN, 2, 1);
        if(!pacmaiden->chara.procAnimation.running)
            changePacmaidenState(pacmaiden, NORMAL);
    }
    else if(pacmaiden->state==KILLER)
            killerTime(pacmaiden,5);
}