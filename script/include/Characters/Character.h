/** @brief Arquivo de implementação de tudo que diz respeito à struct abstrata Character */

#include "raylib.h"
#include<stdbool.h>

#include "../System/WindowControl.h"
#include "../Map/Map.h"
#include "../Animations/ProceduralAnimation.h"

#pragma once


#define RADIUS 20
#define SPEED 160



/** @brief Struct de um círculo no Raylib. A biblioteca não trás uma struct pra esse objeto, mesmo ele sendo usado com frequência
 *        e ter uma função de colisão dedicada a ele. Como será muito usado no código, criamos uma estrutura para isso.
 * @param center (px) Posição do centro do círculo
 * @param radius (px) Raio do círculo */
typedef struct {
    Vector2 center;
    float radius;
} Circle;


/** @brief Struct abstrata de todos os personagens do jogo. Ela lida com a movimentação e definição visual de personagens
 *        do jogador e Inimigos. Essa "herança" ocorre colocando um campo do tipo Character na struct do personagem. Isso permite
 *        a criação de funções comuns a personagens serem definidos de forma genéricas para todos eles de uma vez só.
 *        Fantasmas e PacMaiden são Characters.
 * @param circle Objeto Circle que define a posição e área da colisão do personagem.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param color Cor do personagem a partir das definições da Raylib.
 * @param moveDirection Vetor que indica a direção do movimento. Para cada eixo: -1=trás; 0=parado; 1=frente. Outras valores são inválidos. */
typedef struct {
    Circle circle;
    float speed;
    Color color;
    Vector2 moveDirection;
    ProceduralAnimation procAnimation; 
} Character;


/** @brief Detecta a colisão entre dois personagens. 
 * @returns Se os Circles dos personagens estão se tocando. */
bool checkCharacterCollision(Character chara1, Character chara2){
    return CheckCollisionCircles(chara1.circle.center, chara1.circle.radius, 
                                 chara2.circle.center,  chara2.circle.radius);
}


/** @brief Cria a instância da Struct personagem a partir dos parâmetros fornecidos.
 * 
 * @param position Vector da posição.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param color Cor do personagem a partir das definições da Raylib.
 * 
 * @return Objeto inicializado do personagem. */
Character initCharacter(Vector2 position, int speed, float radius, Color color){
    Circle characterCircle = {(Vector2){position.x+radius, position.y+radius}, radius};

    return (Character){characterCircle, speed, color, (Vector2){0,0}, (ProceduralAnimation){0,false}};
}


/** @brief Confere se o personagem está no centro de uma célula do grid do jogo. */
bool isCharacterInGridCenter(Character character){
    return isPositionInGridCenter(character.circle.center);
}

/** @brief Verifica se o personagem está dentro da tela do jogo */
bool isCharacterInsideScreen(Character character,Vector2 displacement){
    return isPositionInsideScreen(character.circle.center, (Vector2){0,0});
}


/** @brief determina a posição de um personagem se a posição for válida.
 * @return Se a determinação da posição foi bem sucedida. */
bool setPosition(Character* chara, Vector2 position){
    Vector2 centeredPosition = {position.x + chara->circle.radius, position.y + chara->circle.radius};
    if(isPositionInsideScreen(centeredPosition, (Vector2){0,0})){
        chara->circle.center = centeredPosition;
        return true;
    }
    return false;
}


/** @brief Move o personagem para um espaço não ocupado por um bloco de parede.
 * 
 * @param character Referência para a struct de personagem que irá ser movimentada. 
 * @param moveDirection Vetor de módulo 1 que indica a direção do movimento.
 * 
 * @returns Se o personagem foi movimentado ou não. */
bool move(Character* character, Map map){
    bool moved = true;

    // determina posição que a pacmaiden quer ir
    Vector2 destination = (Vector2){character->circle.center.x + character->moveDirection.x * character->speed*GetFrameTime(),
                                    character->circle.center.y + character->moveDirection.y * character->speed*GetFrameTime()};
    // define o ponto em que será detectada a colisão com a parede
    Vector2 movingBound = {destination.x + character->circle.radius * character->moveDirection.x,
                           destination.y + character->circle.radius * character->moveDirection.y};
    if(DEBUG_MODE){
        // Coloca a bolinha vermelha na frente do personagem
        DrawCircleV(movingBound, 5, RED);
    }

    // Encosta o personagem na parede quando ele tentaria atravessá-la. Caso não implementado, o personagem nunca chega na parede
    if (readPositionInMap(movingBound, map, (Vector2){0,0}) == '#') {
        // arredonda vetor
        destination = Vector2Scale(character->circle.center, PIX2GRID);
        destination.x = (int)destination.x;
        destination.y = (int)destination.y;
        destination = Vector2Scale(destination, GRID2PIX);

        destination.x = destination.x + character->circle.radius;
        destination.y = destination.y + character->circle.radius;

        moved = false;
    }

    // Alinhada ao centro de um grid no eixo que não é o do movimento
    if(character->moveDirection.x != 0)
        destination.y = (int)(destination.y/40)*40 + character->circle.radius;
    else if(character->moveDirection.y != 0)
        destination.x = (int)(destination.x/40)*40 + character->circle.radius;

    character->circle.center.x = destination.x;
    character->circle.center.y = destination.y;

    return moved;
}


/** @brief Teletransporta o personagem para o outro lado da tela caso ultrapasse as bordas dela.
 * 
 * @param chara Personagem que irá ser teletransportado */
void portalBorders(Character* chara){
    // Horizontal
    if(chara->circle.center.x < 0 - chara->circle.radius*2)
        chara->circle.center.x = LARGURA;
    else if(chara->circle.center.x > LARGURA)
        chara->circle.center.x = 0 - chara->circle.radius*2;
        
    // Vertical
    if(chara->circle.center.y < 0 - chara->circle.radius*2)
        chara->circle.center.y = ALTURA;
    else if(chara->circle.center.y > ALTURA)
        chara->circle.center.y = 0 - chara->circle.radius*2;   
}


Vector2 getDirectionToCharacter(Vector2 point, Character chara){
    Vector2 distance = {chara.circle.center.x-point.x, chara.circle.center.y-point.y};
    
    Vector2 direction = {0,0};
    if(distance.x == 0)
        direction = (Vector2){0, (distance.y)/fabs(distance.y)};
    else if(distance.y == 0)
        direction = (Vector2){(distance.x)/fabs(distance.x), 0};
    else
        direction = (Vector2){(distance.x)/fabs(distance.x), (distance.y)/fabs(distance.y)};
    
    return direction;
}


void recklessEscape(Character* escaper, Character threat, Map map){
    Vector2 directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    Vector2 escapeDirection = escaper->moveDirection;
    float biggestGCost = 0;

    Vector2 directionToThreat = getDirectionToCharacter(escaper->circle.center, threat);

    for(int i=0; i<4; i++)
        if(readPositionInMap(escaper->circle.center, map, directions[i]) != '#'){
            Vector2 nextPosition = {escaper->circle.center.x+directions[i].x, escaper->circle.center.y+directions[i].y};
            float currentGCost = fabs(nextPosition.x - threat.circle.center.x) + fabs(nextPosition.y - threat.circle.center.y);

            if(currentGCost > biggestGCost){
                escapeDirection = directions[i];
                biggestGCost = currentGCost;
            }
        }

    if((escapeDirection.x!=0 && escapeDirection.x==directionToThreat.x) || (escapeDirection.y!=0 && escapeDirection.y==directionToThreat.y))
        return;
    escaper->moveDirection = escapeDirection;
}
