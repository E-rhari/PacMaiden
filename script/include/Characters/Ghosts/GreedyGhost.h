#include "raylib.h"
#include "raymath.h"
#include "../../Map/Map.h"
#include<stdio.h>
#include<stdlib.h>

Vector2 directions[] = {(Vector2) {1, 0}, (Vector2) {-1, 0}, (Vector2) {0, 1}, (Vector2) {0, -1}};
int moveCounter = 0;

void getLimits(char** map, Vector2 mapSize, Vector2 chunkAmount, Vector2* boundA, Vector2* boundB) {
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
        //TraceLog(LOG_INFO, "abaporu");
    }
}


void getState(Ghost* ghost, Map map){
    Vector2 limitA = (Vector2) {0, 0};
    Vector2 limitB = (Vector2) {0, 0};
    moveCounter++;

    int foundTarget = false;
    Vector2 targetPos;
    getLimits(map, (Vector2){40, 20}, (Vector2){4, 2}, &limitA, &limitB);
    //TraceLog(LOG_INFO, "Limite A: (%f, %f); Limite B: (%f, %f)", limitA.x, limitA.y, limitB.x, limitB.y);
    Vector2 ghostPos = Vector2Scale(ghost->chara.circle.center, PIX2GRID);

    if (ghostPos.x >= limitA.x && ghostPos.x <= limitB.x && ghostPos.y >= limitA.y && ghostPos.y <= limitB.y){
        TraceLog(LOG_INFO, "Dentro do quadrante mais denso.");
        chooseDestinationAware(ghost, map);
    } else {
        TraceLog(LOG_INFO, "Fora do quadrante mais denso.");
        for (int y = (int)limitA.y; y < (int)limitB.y; y++) {
            for (int x = (int)limitA.x; x < (int)limitB.x; x++) {
                
                char tile = map[y][x];
                
                if (tile != '#' && tile != 't') { 
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
        if (nextStep != NULL){
            ghost->chara.moveDirection = getStalkingDirection(ghost, *nextStep);
        }
    }
}