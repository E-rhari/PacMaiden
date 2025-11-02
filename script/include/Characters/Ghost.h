#include "raylib.h"
#include<stdbool.h>

#include "./Character.h"
#include "../Map/Map.h"

#pragma once



/** @brief Inimigos do jogador */
typedef struct {
    Character chara;
    bool canChooseDestination;
} Ghost;


/** @brief Cria a instância do fantasma
 * 
 * @param position Vector de posição.
 * @param speed (px/s) Velocidade, em pixels por segundo, que o personagem se move.
 * @param radius (px) Raio do círculo de colisão do personagem.
 * @param color Cor do personagem a partir das definições da Raylib. */
Ghost initGhost(Vector2 position, int radius, float speed, Color color){
    Circle characterRec = {(Vector2){position.x+radius, position.y+radius}, radius};
    Character chara = (Character){characterRec, speed, color};
    return (Ghost){chara,true};
}

/** @brief Altera a propriedade moveDirection do fantasma aleatóriamente, mas considerando os seus arredores. O movimento 
 *        só é retrocedido se estritamente necessário, enquanto escolhe aleatoriamente uma posição entre as possíveis.
 * @return Se a direção foi alterada. */
bool choseDestinationAware(Ghost* ghost, Map map){

    if (!isCharacterInGridCenter(ghost->chara)){
        ghost->canChooseDestination=true;
        return false;
    }

    if(!isCharacterInsideScreen(ghost->chara, (Vector2){0,0}))
        return false;

    if(!ghost->canChooseDestination)
        return false;
    
    
    Vector2 directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    
    Vector2 *possibleDirections= (Vector2*)malloc(1);
    int length=0;

    for(int i=0; i<4; i++)
        // Detectas as direções possíveis, desconsiderando a retrograda
        if(readPositionInMap(ghost->chara.circle.center, map, directions[i]) != '#'
        && !Vector2Equals(directions[i], Vector2Scale(ghost->chara.moveDirection,-1))){
            possibleDirections = (Vector2*)realloc(possibleDirections, (length+1)*sizeof(Vector2));
            possibleDirections[length] = directions[i];
            length++;
        }
    if(length>0){
        // Escolhe aleatoriamente uma direção possível
        int random = (rand()%length);
        ghost->chara.moveDirection = possibleDirections[random];
    }
    else
    // Determina a direção como retrograda
        ghost->chara.moveDirection = Vector2Scale(ghost->chara.moveDirection,-1);

    ghost->canChooseDestination=false;
    return true;

}


/** @brief Movimenta o fantasma aleatóriamente, mas considerando os seus arredores. Ele só vai pra trás se estritamente necessário,
 *        enquanto escolhe aleatoriamente uma posição entre as possíveis.
 * @return Se o personagem foi movimentado ou não. */
bool moveAware(Ghost* ghost, Map map){
    choseDestinationAware(ghost, map);
    return move(&ghost->chara, map);
}


/**  @brief Escolhe uma direção aleatória para a movimentação do fantasma. Esse função não vê nenhum problema em bater de cara na 
 *        parede, em retroceder o movimento derrepente ou em andar em círculos.
 * @return Se a direção foi alterada. */
bool choseDestinationUnaware(Ghost* ghost){
    if (isCharacterInGridCenter(ghost->chara) && isCharacterInsideScreen(ghost->chara, (Vector2){0,0})){
        int random = (rand()%4) + 1;
        if(random == 1)
            ghost->chara.moveDirection = (Vector2){0,-1};
        else if(random == 2)
            ghost->chara.moveDirection = (Vector2){0,1};
        else if(random == 3)
            ghost->chara.moveDirection = (Vector2){-1, 0};
        else if(random == 4)
            ghost->chara.moveDirection = (Vector2){1, 0};
        return true;
    }
    return false;
}


/**  @brief Move o fantasma aleatóriamente. Esse função não vê nenhum problema em bater de cara na parede, em 
 *        retroceder o movimento derrepente ou em andar em círculos.
 * @return Se o personagem foi movimentado ou não. */
bool moveUnaware(Ghost* ghost, Map map){
    choseDestinationUnaware(ghost);
    return move(&ghost->chara, map);
}