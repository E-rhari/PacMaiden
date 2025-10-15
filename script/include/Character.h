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
 * @brief Struct de um círculo no Raylib. A biblioteca não trás uma struct pra esse objeto, mesmo ele sendo usado com frequência
 *        e ter uma função de colisão dedicada a ele. Como será muito usado no código, criamos uma estrutura para isso.
 * @param center (px) Posição do centro do círculo
 * @param radius (px) Raio do círculo
 */
typedef struct {
    Vector2 center;
    float radius;
} Circle;


/**
 * @brief Struct abstrata de todos os personagens do jogo. Ela lida com a movimentação e definição visual de personagens
 *        do jogador e Inimigos. Essa "herança" ocorre colocando um campo do tipo Character na struct do personagem. Isso permite
 *        a criação de funções comuns a personagens serem definidos de forma genéricas para todos eles de uma vez só.
 *        Fantasmas e PacMaiden são Characters.
 * @param circle Objeto Circle que define a posição e área da colisão do personagem.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param color Cor do personagem a partir das definições da Raylib.
 */
typedef struct {
    Circle circle;
    float speed;
    Color color;
} Character;



/**
 * @brief Cria a instância da Struct personagem a partir dos parâmetros fornecidos.
 * 
 * @param position Vector de posição.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param color Cor do personagem a partir das definições da Raylib.
 */
Character initCharacter(Vector2 position, int speed, float radius, Color color){
    Circle characterCircle = {(Vector2){position.x, position.y}, radius};
    return (Character){characterCircle, speed, color};
}

/**
 * @brief Move o personagem para um espaço não ocupado por um bloco de parede.
 * 
 * @param character* Referência para a struct de personagem que irá ser movimentada. 
 * @param direction Vetor de módulo 1 que indica a direção do movimento.
 */
bool move(Character* character, Vector2 direction){
    Vector2 dest = (Vector2){character->circle.center.x + direction.x * character->speed*GetFrameTime(),
                             character->circle.center.y + direction.y * character->speed*GetFrameTime()};
    Vector2 movingBound = {dest.x + character->circle.radius * direction.x,
                           dest.y + character->circle.radius * direction.y};
    Vector2 gridBound = Vector2Scale(movingBound, PIX2GRID);

    if(DEBUG){
        char texto[100];
        sprintf(texto, "x: %.2f, y: %.2f.", gridBound.x, gridBound.y);
        DrawText(texto, 100, 100, 50, BLACK);
        
        DrawCircleV(movingBound, 5, BLACK);
    }

    if (mapa[(int)gridBound.y][(int)gridBound.x] == '@') {
        dest.x = (int)gridBound.x-1;
        return false;
    }

    character->circle.center.x = dest.x;
    character->circle.center.y = dest.y;
    return true;
}


/**
 * @brief Teletransporta o personagem para o outro lado da tela caso ultrapasse as bordas dela.
 * 
 * @param chara Personagem que irá ser teletransportado
 */
void portalBorders(Character* chara){
    if(chara->circle.center.x < 0 - chara->circle.radius)
        chara->circle.center.x = LARGURA + chara->circle.radius;
    else if(chara->circle.center.x > LARGURA)
        chara->circle.center.x = 0 - chara->circle.radius;
    if(chara->circle.center.y < 0 - chara->circle.radius)
        chara->circle.center.y = LARGURA + chara->circle.radius;
    else if(chara->circle.center.y > LARGURA)
        chara->circle.center.y = 0 - chara->circle.radius;   
}