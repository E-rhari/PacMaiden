#include "raylib.h"
#include "raymath.h"
#include "../../Map/Map.h"
#include<stdio.h>
#include<stdlib.h>


/** @brief 
 * @param map Mapa a ser analisado
 * @param mapSize Tamanho do mapa
 * @param chunkAmount Quantidade separada em x e y de unidades o mapa será dividido
 * @param boundA [retorno] Posição superior esquerda do retângulo da área com mais pellets
 * @param boundB [retorno] Posição inferior direita do retângulo da área com mais pellets
 * @return Os vetores de limite da área com mais pellets*/
void getGreedyLimits(Map map, Vector2 mapSize, Vector2 chunkAmount, Vector2* boundA, Vector2* boundB) {
    int totalChunks = (int)(chunkAmount.x * chunkAmount.y);
    
    Vector2 chunkSize = { mapSize.x / chunkAmount.x, mapSize.y / chunkAmount.y };
    
    int greatest = 0;

    for (int n = 0; n < totalChunks; n++) {
        int pelletCount = 0;

        int chunkX = n % (int)chunkAmount.x;
        int chunkY = n / (int)chunkAmount.x;

        int startX = chunkX * (int)chunkSize.x;
        int startY = chunkY * (int)chunkSize.y;
        int endX = startX + (int)chunkSize.x;
        int endY = startY + (int)chunkSize.y;
        
        //TraceLog(LOG_INFO, "Teste");
        for (int i = startX; i < endX; i++) {
            for (int j = startY; j < endY; j++) {
                //TraceLog(LOG_INFO, "i: %d, j: %d", i, j);
                if (map[j][i] == '.') {
                    pelletCount++;
                    //TraceLog(LOG_INFO, "a");
                }
            }
        }
    
        if (pelletCount > greatest) {
            greatest = pelletCount;
            *boundA = (Vector2){ (float)startX, (float)startY };
            *boundB = (Vector2){ (float)endX, (float)endY };
        }
    }
}


/** @brief Encaminha um fantasma para a área com mais pellets */
void moveGreedy(Ghost* ghost, Map map){
    static int moveCounter = 0;
    Vector2 limitA = (Vector2) {0, 0};
    Vector2 limitB = (Vector2) {0, 0};
    moveCounter++;

    int foundTarget = false;
    Vector2 targetPos;
    getGreedyLimits(map, (Vector2){40, 20}, (Vector2){4, 2}, &limitA, &limitB);
    Vector2 ghostPos = Vector2Scale(ghost->chara.circle.center, PIX2GRID);

    GridVector gridPos = vector2ToGridVector(ghost->chara.circle.center);
    if(!isInsideMap(gridPos,map,(GridVector){0,0})){
        chooseDestinationAware(ghost, map);
        return;
    }


    if (ghostPos.x >= limitA.x && ghostPos.x <= limitB.x && ghostPos.y >= limitA.y && ghostPos.y <= limitB.y){
        chooseDestinationAware(ghost, map);
    } else {
        for (int y = (int)limitA.y; y < (int)limitB.y; y++) {
            for (int x = (int)limitA.x; x < (int)limitB.x; x++) {
                  
                if (readCoordinatesInMap((GridVector){x,y}, map, (GridVector){0,0}) != '#' && readCoordinatesInMap((GridVector){x,y}, map, (GridVector){0,0}) != 'T') { 
                    targetPos = (Vector2){ (float)x, (float)y };
                    foundTarget = true;
                    break;
                }
            }
            if (foundTarget) break;
        }
    }

    if (foundTarget) {
        NodeList path = findPath(vector2ToGridVector(ghost->chara.circle.center), vector2ToGridVector(Vector2Scale(targetPos, GRID2PIX)), map);

        Node* nextStep = getFromNodeList(&path, 1);
        if (nextStep != NULL)
            ghost->chara.moveDirection = getStalkingDirection(ghost, *nextStep);
        else
            chooseDestinationAware(ghost, map);
    }
}