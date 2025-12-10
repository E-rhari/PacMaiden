#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#include "../System/WindowControl.h"
#include "../System/PacMath.h"
#include "../System/Files.h"
#include "../Animations/SpriteAnimation.h"

#pragma once


/** @brief Apelido semântico para char** */
typedef char** Map;



void drawMap(Map map, Vector2** mapCellPosInSprite) {
    int cell = 40;
    Rectangle source = {0, 0, 20, 20};
    Rectangle dest;
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 40; j++){
            dest = (Rectangle){(float)(j)*GRID2PIX, (float)(i)*GRID2PIX, 40, 40};
            switch(map[i][j])
            {
                case '#':
                    source = (Rectangle){mapCellPosInSprite[i][j].x*20, mapCellPosInSprite[i][j].y*20, 20, 20};
                    dest   = (Rectangle){j*GRID2PIX, i*GRID2PIX, 40, 40};
                    DrawTexturePro(SPRITES[TILESET_SPRITE], source, dest, (Vector2){0,0}, 0.0f, WHITE);
                    break;

                case '.':
                    DrawRectangleLines(j * cell, i * cell, cell, cell, LIGHTGRAY);
                    DrawTexturePro(SPRITES[PELLET_SPRITE], source, dest, (Vector2){0,0}, 0.0f, WHITE);
                    break;
                
                case 'o':
                    DrawRectangleLines(j * cell, i * cell, cell, cell, LIGHTGRAY);
                    DrawTexturePro(SPRITES[POWER_PELLET_SPRITE], source, dest, (Vector2){0,0}, 0.0f, WHITE);
                    break;
                
                case 't':
                    DrawRectangle(j * cell, i * cell, cell, cell, PURPLE);
                    DrawTexturePro(SPRITES[PORTAL_SPRITE], source, dest, (Vector2){0,0}, 0.0f, WHITE);
                    break;
                    
                default:
                    DrawRectangleLines(j * cell, i * cell, cell, cell, LIGHTGRAY);
            } 
        }
    }
}


/** @brief Transforma o arquivo map#.txt em uma matriz do tipo char.
 * @param level Número do mapa a ser lido. Substitui o # em map#.txt.
 * @param map Referência da variável na qual será salvo o mapa. */
void readMap (int level, Map map)
{
    char temp;
    char path[50];

    sprintf(path, getFilePath("../../sprites/maps/map%d.txt"), level);

    FILE* arq = fopen(path, "r");

    if(arq == NULL)
    {
        printf("Erro de abertura de arquivo\n");
        return;
    }
    
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 40; j++){
            temp = getc(arq);
            if(temp !='\n')
                map[i][j] = temp;
        }
        getc(arq);
    }
    fclose(arq);
    return;
}



bool isInsideMap(GridVector gridPosition, Map map, GridVector displacement){
    return (int)gridPosition.y+(int)displacement.y>=0 && (int)gridPosition.y+(int)displacement.y<HEIGHT/40
        && (int)gridPosition.x+(int)displacement.x>=0 && (int)gridPosition.x+(int)displacement.x<WIDTH/40;
}


char readCoordinatesInMap(GridVector gridPosition, Map map, GridVector displacement){
    if(isInsideMap(gridPosition, map, displacement))
        return map[(int)gridPosition.y+(int)displacement.y][(int)gridPosition.x + (int)displacement.x];
    return ' ';
}


/** @brief Lê o valor da matriz na posição enviada.
 * @param position (px) Vetor da posição a ser lida no mapa. Ela deve estar em pixels e na
 *                 escala da tela do jogo. A conversão de pixel para célula da matriz é intera na função.
 * @param map Mapa do qual será lido o valor.
 * @param displacement (matrix cell) Deslocamento da posição que será lida na matriz. */
char readPositionInMap(Vector2 position, Map map, Vector2 displacement){
    GridVector gridPosition = vector2ToGridVector(position);
    GridVector gridDisplacement = {(int)displacement.x, (int)displacement.y};

    return readCoordinatesInMap(gridPosition, map, gridDisplacement);
}


/** @brief Percorre a matriz do mapa buscando correspondências do caractere descrito em object */
Vector2* searchInMap(Map map, char object){
    Vector2 *occurrences = (Vector2*)malloc(1);
    int length = 0;
    
    for(int i=0; i<20; i++)
        for(int j=0; j<40; j++)
            if(map[i][j] == object){
                occurrences = (Vector2*)realloc(occurrences, (length+1)*sizeof(Vector2));
                occurrences[length] = (Vector2){j*40, i*40};
                length++;
            }

    return occurrences;
}

int countPallets(Map map){
    int length=0;
    for(int i=0;i<20;i++)
        for(int j=0;j<40;j++)
            if(map[i][j]=='.')
                length++;
    return length;
}

/** @brief Confere se o personagem está no centro de uma célula do grid do jogo. */
bool isPositionInGridCenter(Vector2 position){
    return (int)(position.x+20)%40 < 3   &&  (int)(position.y+20)%40 < 3;
}


/** @brief Aloca espaço na memória para o mapa. */
Map setUpMap(){
    Map map = (char**)malloc(sizeof(char*)*20);

    for(int i=0;i<20;i++)
        *(map+i) = (char*)malloc(sizeof(char)*40);
    return map;
}

/** @brief Desaloca espaço na memória para o mapa. */
void freeMap(Map map){
    for(int i=0; i<20;i++)
        free(*(map+i));
    free(map);
}


Vector2** decideMapCellsSprite(Map map){    
    Vector2** spriteSheetPos = (Vector2**)malloc(sizeof(Vector2*)*20);
    for(int i=0;i<20;i++)
        *(spriteSheetPos+i) = (Vector2*)malloc(sizeof(Vector2)*40);
    
    printf("\nCHIRASHIZUSHI\n");
    for(int i=0; i<20; i++){
        for(int j=0; j<40; j++){
            if(readCoordinatesInMap((GridVector){j, i}, map, (GridVector){0,0}) == '#'){
                int spriteCollumn = rand()%4;
                if(!isPositionInsideScreen((Vector2){j*GRID2PIX, i*GRID2PIX}, (Vector2){0,1}) || readCoordinatesInMap((GridVector){j, i}, map, (GridVector){0,1}) == '#')
                    spriteSheetPos[i][j] = (Vector2){spriteCollumn, 0};
                else
                    spriteSheetPos[i][j] = (Vector2){spriteCollumn, 1};
            }
            else
                spriteSheetPos[i][j] = (Vector2){-1,-1};
        }
    }
    return spriteSheetPos;
}