/**
 * @brief Arquivo de implementação da personagem do jogador, PacMaiden, incluindo vidas e pontuação
 */


#include<raylib.h>
#include<stdbool.h>

#include "./Character.h"

#pragma once

/**
 * @brief Personagem do jogador e protagonista do jogo :-)
 * 
 * @param chara Struct abstrata dos personagens. Lida com a posição, tamanho e velocidade da PacMaiden.
 * @param life Contador de vidas
 * @param points Contador de pontos
 */
typedef struct {
    Character chara;
    int lifes;
    int ponits;
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
    Circle characterRec = {(Vector2){position.x+radius, position.y+radius}, radius};
    Character chara = (Character){characterRec, speed, color};
    return (PacMaiden){chara, lifes, points};
}