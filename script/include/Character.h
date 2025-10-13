/**
 * @brief Arquivo de implementação de tudo que diz respeito à struct abstrata Character
 */

#include<raylib.h>
#include<stdbool.h>

#include "./WindowControl.h"
#include "./Map.h"
// #include "./Degub.h"

#pragma once


/**
 * @brief Struct abstrata de todos os personagens do jogo. Ela lida com a movimentação e definição visual de personagens
 *        do jogador e Inimigos. Essa "herança" ocorre colocando um campo do tipo Character na struct do personagem. Isso permite
 *        a criação de funções comuns a personagens serem definidos de forma genéricas para todos eles de uma vez só.
 *        Fantasmas e PacMaiden são Characters.
 * @param rec Objeto Rectangle que define a posição e tamanho do personagem.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param color Cor do personagem a partir das definições da Raylib.
 */
typedef struct {
    Rectangle rec;
    float speed;
    Color color;
} Character;


/**
 * @brief Cria a instância da Struct personagem a partir dos parâmetros fornecidos.
 * 
 * @param position Vector de posição.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param width (px) Largura do Rectangle do personagem.
 * @param height (px) Altura do Rectangle do personagem.
 * @param color Cor do personagem a partir das definições da Raylib.
 */
Character initCharacter(Vector2 position, int speed, float width, float height, Color color){
    Rectangle characterRec = {position.x, position.y, width, height};
    return (Character){characterRec, speed, color};
}

/**
 * @brief Move o personagem para um espaço não ocupado por um bloco de parede.
 * 
 * @param character* Referência para a struct de personagem que irá ser movimentada. 
 * @param direction Vetor de módulo 1 que indica a direção do movimento.
 */
bool move(Character* character, Vector2 direction){
    Vector2 dest = (Vector2){character->rec.x + direction.x * character->speed*GetFrameTime(),
                             character->rec.y + direction.y * character->speed*GetFrameTime()};
    Vector2 movingBound = {dest.x + character->rec.width * (1.0f/2.0f + direction.x/2),
                           dest.y + character->rec.height * (1.0f/2.0f + direction.y/2)};
    Vector2 gridBound = Vector2Scale(movingBound, PIX2GRID);

    char texto[100];
    sprintf(texto, "x: %.2f, y: %.2f.", gridBound.x, gridBound.y);
    DrawText(texto, 100, 100, 50, BLACK);
    
    DrawCircleV(movingBound, 5, BLACK);
    
    if (mapa[(int)gridBound.y][(int)gridBound.x] == '@') {
        dest.x = (int)gridBound.x-1;
        return false;
    }

    character->rec.x = dest.x;
    character->rec.y = dest.y;
    return true;
}


/**
 * @brief Teletransporta o personagem para o outro lado da tela caso ultrapasse as bordas dela.
 * 
 * @param chara Personagem que irá ser teletransportado
 */
void portalBorders(Character* chara){
    if(chara->rec.x < 0 - chara->rec.width)
        chara->rec.x = LARGURA + chara->rec.width;
    else if(chara->rec.x > LARGURA)
        chara->rec.x = 0 - chara->rec.width;
    if(chara->rec.y < 0 - chara->rec.height)
        chara->rec.y = LARGURA + chara->rec.height;
    else if(chara->rec.y > LARGURA)
        chara->rec.y = 0 - chara->rec.height;   
}