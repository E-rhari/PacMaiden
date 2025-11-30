#include "raylib.h"
#include<stdbool.h>

#include "./Character.h"

#pragma once

/** @brief Tempo de invulnerabilidade da pacmaiden, em segundos. */
#define HURT_COOLDOWN 3

/** @brief Possibilidades de estados da pacmaiden. Devem ser alterados usando changePacmaidenState() */
typedef enum {
    NORMAL,
    DYING,
    IMMORTAL,
    KILLER
} PacState;


/** @brief Personagem do jogador e protagonista do jogo :-
 * @param chara Struct abstrata dos personagens. Lida com a posição, tamanho, velocidade e cor da PacMaiden.
 * @param initialValues Struct constante do tip Character para salver os valores iniciais de posição, tamanho, velocidade e cor.
 * @param life Contador de vidas
 * @param points Contador de pontos
 * @param timePivot (s) Segundos entre o início do jogo e a última vez que a pacmaiden levou dano.
 * @param state Estado atual da pacmaiden. */
typedef struct {
    Character chara;
    Character initialValues;
    int lifes;
    int points;
    int timePivot;
    Vector2 bufferedInput;
    bool canMove;
    PacState state;
    

} PacMaiden;


/** @brief Cria a instância da Struct PacMaiden a partir dos parâmetros fornecidos.
 * @param position Vector de posição.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param color Cor do personagem a partir das definições da Raylib.
 * @param lifes Valor inicial do contador de vidas
 * @param points Valor inicial do contador de pontos */
PacMaiden initPacMaiden(Vector2 position, int radius, float speed, Color color, int lifes, int points){
    Character chara = initCharacter((Vector2){position.x, position.y}, speed, radius, color);
    return (PacMaiden){chara, chara, lifes, points, 0, (Vector2){0,0},true};
}


/** @brief Além de determinar o valor da propriedade state da pacmaiden, também realiza as operações devidas
 *        na transição entre os estados. Sempre use no lugar de determinar o valor de state manualmente. */
void changePacmaidenState(PacMaiden* pacmaiden, PacState state){
    pacmaiden->state = state;

    switch (state){
        case NORMAL:
            break;

        case KILLER:
            break;

        case DYING:
            pacmaiden->chara.procAnimation.initTime = GetTime();
            break;

        case IMMORTAL:
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


/** @brief Verifica se a pacmaiden está tocando a Pellet */
bool checkPowerPellet(PacMaiden* pacmaiden, Map map){
    if(charCollided(*pacmaiden, map) == 'o')
        return true;
    return false;
}


/** @brief Adiciona a quantidade adequada de pontos à pontuação da pacmaiden.
 * @param pacMaiden Personagem do jogador que receberá a pontuação.
 * @param map Mapa de onde será contado o ponto.
 * @param object Caractere que indica o tipo de objeto. */
void countPoints(PacMaiden* pacMaiden, Map map, char object, int *pallets){
    Vector2 colliderBound = (Vector2){pacMaiden->chara.circle.center.x + pacMaiden->chara.circle.radius*pacMaiden->chara.moveDirection.x,
                                      pacMaiden->chara.circle.center.y + pacMaiden->chara.circle.radius*pacMaiden->chara.moveDirection.y};
    Vector2 convertedPos = Vector2Scale(colliderBound, PIX2GRID);
    switch(object)
    {
        case '.':
            addPoints(pacMaiden, 10);
            *pallets=*pallets-1;
            break;
        case 'o':
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


/** @brief Rotina a ser realizada quando a Pacmaiden chega a 0 pontos de vida. */
void die(PacMaiden* pacmaiden){
    printf("Damn morri omg");

}


/** @brief Dá dano a pacmaiden caso o cooldown seja obedecido.
 * @return Se a pacmaiden levou dano ou não */
bool hurtPacmaiden(PacMaiden* pacmaiden, Map map){
    pacmaiden->lifes--;
    pacmaiden->timePivot = GetTime();
    addPoints(pacmaiden, -200);
    
    changePacmaidenState(pacmaiden, DYING);
    
    if(pacmaiden->lifes <= 0)
        die(pacmaiden);
    
    return true;
}


/** @brief Todas as ações de comportamento da PacMaiden que devem ser rodadas por frame */
void pacmaidenBehaviour(PacMaiden* pacmaiden, Map map){
    
    if(pacmaiden->canMove)
        move(&pacmaiden->chara, map);

    portalBorders(&pacmaiden->chara);
    
    if(pacmaiden->state == IMMORTAL){
        blinkAnimation(&pacmaiden->chara.color, pacmaiden->initialValues.color , WHITE, &pacmaiden->chara.procAnimation, HURT_COOLDOWN, 2);
        if(!pacmaiden->chara.procAnimation.running)
            changePacmaidenState(pacmaiden, NORMAL);
    }
}

void canPlayersMove(PacMaiden* players){
    Vector2 playerNewCenter[2] = {players[0].chara.circle.center,players[1].chara.circle.center};
   

    for(int i=0;i<2;i++){
        playerNewCenter[i].x+= players[i].chara.moveDirection.x;
        playerNewCenter[i].y+= players[i].chara.moveDirection.y;
    }
    if(!CheckCollisionCircles(playerNewCenter[0],players[0].chara.circle.radius,playerNewCenter[1],players[1].chara.circle.radius)){
        players[0].canMove=true;
        players[1].canMove=true;
    }
    
}
