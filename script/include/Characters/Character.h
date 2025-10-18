/**
 * @brief Arquivo de implementação de tudo que diz respeito à struct abstrata Character
 */

#include "raylib.h"
#include<stdbool.h>

#include "../System/WindowControl.h"
#include "../Map/Map.h"

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
 * @param moveDirection Vetor que indica a direção do movimento. Para cada eixo: -1=trás; 0=parado; 1=frente. Outras valores são inválidos.
 */
typedef struct {
    Circle circle;
    float speed;
    Color color;
    Vector2 moveDirection;
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
    Circle characterCircle = {(Vector2){position.x+radius, position.y+radius}, radius};
    return (Character){characterCircle, speed, color, (Vector2){0,0}};
}

/**
 * @brief Move o personagem para um espaço não ocupado por um bloco de parede.
 * 
 * @param character* Referência para a struct de personagem que irá ser movimentada. 
 * @param moveDirection Vetor de módulo 1 que indica a direção do movimento.
 * 
 * @returns Se o personagem fio movimentado ou não.
 */
bool move(Character* character, Map map){
    // determina posição que a pacmaiden quer ir
    Vector2 destination = (Vector2){character->circle.center.x + character->moveDirection.x * character->speed*GetFrameTime(),
                                    character->circle.center.y + character->moveDirection.y * character->speed*GetFrameTime()};
    // define o ponto em que será detectada a colisão com a parede
    Vector2 movingBound = {destination.x + character->circle.radius * character->moveDirection.x,
                           destination.y + character->circle.radius * character->moveDirection.y};
    // converte essa posição do mapa do jogo para a posição na matriz do mapa
    Vector2 gridBound = Vector2Scale(movingBound, PIX2GRID);

    if(DEBUG_MODE){
        DrawCircleV(movingBound, 5, RED);
    }

    if((int)gridBound.y>=0 && (int)gridBound.y<20
    && (int)gridBound.x>=0 && (int)gridBound.x<40)
        if (map[(int)gridBound.y][(int)gridBound.x] == '#') {
            // arredonda vetor
            destination = Vector2Scale(character->circle.center, PIX2GRID);
            destination.x = (int)destination.x;
            destination.y = (int)destination.y;
            destination = Vector2Scale(destination, GRID2PIX);

            // executa correção de posição apenas no eixo do movimento
            if(character->moveDirection.x != 0)
                character->circle.center.x = destination.x + character->circle.radius;
            if(character->moveDirection.y!= 0)
                character->circle.center.y = destination.y + character->circle.radius;
            return false;
        }

    character->circle.center.x = destination.x;
    character->circle.center.y = destination.y;
    return true;
}


/**
 * @brief Teletransporta o personagem para o outro lado da tela caso ultrapasse as bordas dela.
 * 
 * @param chara Personagem que irá ser teletransportado
 */
void portalBorders(Character* chara){
    if(chara->circle.center.x < 0 - chara->circle.radius*2)
        chara->circle.center.x = LARGURA;
    else if(chara->circle.center.x > LARGURA)
        chara->circle.center.x = 0 - chara->circle.radius*2;
        
    if(chara->circle.center.y < 0 - chara->circle.radius*2)
        chara->circle.center.y = ALTURA;
    else if(chara->circle.center.y > ALTURA)
        chara->circle.center.y = 0 - chara->circle.radius*2;   
}