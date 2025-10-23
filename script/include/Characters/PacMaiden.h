/**
 * @brief Arquivo de implementação da personagem do jogador, PacMaiden, incluindo vidas e pontuação
 */


#include "raylib.h"
#include<stdbool.h>

#include "./Character.h"

#pragma once


/**
 * @brief Personagem do jogador e protagonista do jogo :-)
 * 
 * @param chara Struct abstrata dos personagens. Lida com a posição, tamanho e velocidade da PacMaiden.
 * @param life Contador de vidas
 * @param points Contador de pontos
 * @param lastHurtTime (s) Segundos entre o início do jogo e a última vez que a pacmaiden levou dano.
 */
typedef struct {
    Character chara;
    int lifes;
    int points;
    int lastHurtTime;
} PacMaiden;


/**
 * @brief Cria a instância da Struct PacMaiden a partir dos parâmetros fornecidos.
 * 
 * @param position Vector de posição.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param color Cor do personagem a partir das definições da Raylib.
 * @param lifes Valor inicial do contador de vidas
 * @param points Valor inicial do contador de pontos
 */
PacMaiden initPacMaiden(Vector2 position, int radius, float speed, Color color, int lifes, int points){
    Character chara = initCharacter((Vector2){position.x, position.y}, speed, radius, color);
    return (PacMaiden){chara, lifes, points, 0};
}


char charCollided(PacMaiden pacMaiden, Map map){
    Vector2 convertedPos = Vector2Scale(pacMaiden.chara.circle.center, PIX2GRID);
    if((int)convertedPos.y>=0 && (int)convertedPos.y<20
        && (int)convertedPos.x>=0 && (int)convertedPos.x<40
        && (int)(pacMaiden.chara.circle.center.x+pacMaiden.chara.circle.radius)%40 < 30 
        && (int)(pacMaiden.chara.circle.center.y+pacMaiden.chara.circle.radius)%40 < 30)
            return map[(int)convertedPos.y][(int)convertedPos.x];
    return ' ';
}

void countPoints(PacMaiden* pacMaiden, Map map, char c){
    Vector2 convertedPos = Vector2Scale(pacMaiden->chara.circle.center, PIX2GRID);
    switch(c)
    {
        case '.':
            pacMaiden->points += 10;
            break;
        case 'o':
            pacMaiden->points += 50;
            break;
        case 'B':
            pacMaiden->points += 300;
            break;
        default:
            return;
    }
    map[(int)convertedPos.y][(int)convertedPos.x] = ' ';
}


/**
 * @brief Dá dano a pacmaiden caso o cooldown seja obedecido.
 * @return Se a pacmaiden levou dano ou não
 */
bool hurt(PacMaiden* pacmaiden){
    int curretTime = GetTime();
    int cooldown = 1;
    
    if(curretTime > pacmaiden->lastHurtTime + cooldown){
        pacmaiden->lifes--;
        pacmaiden->lastHurtTime = GetTime();
        return true;
    }
    return false;
}