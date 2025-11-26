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

#pragma once



/** @brief Corrige direção de movimentação de um fantasma quando ele atravessa um portal usando o A* */
Vector2 getStalkingDirection(Ghost* ghost, Node nextStep){
    Vector2 stalkingDirection = {nextStep.position.x - (int)(ghost->chara.circle.center.x*PIX2GRID), 
                                 nextStep.position.y - (int)(ghost->chara.circle.center.y*PIX2GRID)};
    // portals
    if(stalkingDirection.x > 1)
        stalkingDirection.x = -1;
    else if(stalkingDirection.x < -1)
        stalkingDirection.x = 1;
    if(stalkingDirection.y > 1)
        stalkingDirection.y = -1;
    else if(stalkingDirection.y < -1)
        stalkingDirection.y = 1;
    
    return stalkingDirection;
}


/** @brief Persegue a PacMaiden através do algorítmo A* */
void stalkPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden){
    if(!isCharacterInsideScreen(ghost->chara, (Vector2){0,0}))
            return;

    NodeList path;
    path = findPath(vector2ToGridVector(ghost->chara.circle.center), vector2ToGridVector(pacmaiden->chara.circle.center), map);

    if(path.start == NULL || pacmaiden->state == IMMORTAL){
        chooseDestinationAware(ghost, map);
        return;
    }

    Node nextStep = *getFromNodeList(&path, 1);

    ghost->chara.moveDirection = getStalkingDirection(ghost, nextStep);
}