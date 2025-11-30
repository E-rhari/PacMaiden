#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include "math.h"

#include "../Character.h"
#include "./Ghost.h"
#include "../PacMaiden.h"
#include "../../Map/Map.h"
#include "../../Map/AStar.h"

#include "./StalkerGhost.h"

#pragma once


/** @brief Tenta prever a posição que a PacMaiden quer estar calculando a posição válida a sua frente */
Vector2 predictPacmaidenPosition(PacMaiden* pacmaiden, Map map, int blocksAhead){
    Vector2 currentPosition = pacmaiden->chara.circle.center;
    Vector2 directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    for(int i=0; i<blocksAhead; i++){
        Vector2 testingPosition = {modulate(currentPosition.x+(pacmaiden->chara.moveDirection.x*40), LARGURA),
                                   modulate(currentPosition.y+(pacmaiden->chara.moveDirection.y*40), ALTURA)};
        if(readPositionInMap(testingPosition, map, (Vector2){0,0}) == '#')
            return currentPosition;
        currentPosition = testingPosition;
    }
    return currentPosition;
}


/** @brief Persegue a PacMaiden através do algorítmo A* */
void ambushPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden, int blocksAhead){
    if(!isCharacterInsideScreen(ghost->chara, (Vector2){0,0}))
            return;

    Vector2 pacmaidenDesiredPosition = predictPacmaidenPosition(pacmaiden, map, blocksAhead);

    NodeList path;
    path = stalkPacmaiden(ghost, map, pacmaiden);
    if(path.size <= blocksAhead)
        return;

    path = findPath(vector2ToGridVector(ghost->chara.circle.center), vector2ToGridVector(pacmaidenDesiredPosition), map);
    if(path.start == NULL || pacmaiden->state == IMMORTAL){
        chooseDestinationAware(ghost, map);
        return;
    }

    Node* nextStep = getFromNodeList(&path, 1);
    if(nextStep != NULL)
        ghost->chara.moveDirection = getStalkingDirection(ghost, *nextStep);
}