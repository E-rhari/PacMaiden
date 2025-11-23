#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#include "./GridVector.h"
#include "../System/WindowControl.h"

#pragma once


/** @brief Apelido semântico para char** */
typedef char** Map;


void drawMap(Map map) {
    int cell = 40;
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 40; j++)
            switch(map[i][j])
            {
                case '#':
                    DrawRectangle(j * cell, i * cell, cell, cell, DARKBLUE);
                    break;
                
                case '.':
                    DrawRectangleLines(j * cell, i * cell, cell, cell, LIGHTGRAY);
                    DrawCircle(j * cell + cell/2, i * cell + cell/2, 1.5, WHITE);
                    break;
                
                case 'o':
                    DrawCircle(j * cell + cell/2, i * cell + cell/2, 5, GOLD);
                    DrawRectangleLines(j * cell, i * cell, cell, cell, LIGHTGRAY);
                    break;
                
                case 't':
                    DrawRectangle(j * cell, i * cell, cell, cell, PURPLE);
                    break;
                    
                default:
                    DrawRectangleLines(j * cell, i * cell, cell, cell, LIGHTGRAY);
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
    
    #ifdef _WIN32
        strcpy(path,"PacMaiden/sprites/maps/map");
        char nivelString[3];

        itoa(level,nivelString,10);
        strcat(path,nivelString);
        strcat(path,".txt");
    #elif __linux__
        sprintf(path, "../../sprites/maps/map%d.txt", level);
        printf(path);
    #else
        printf("Sistema operacional não detectado. Proseguindo com configuração do linux");
        sprintf(path, "../../sprites/maps/map%d.txt", level);
        printf(path);
    #endif

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
    return (int)gridPosition.y+(int)displacement.y>=0 && (int)gridPosition.y+(int)displacement.y<ALTURA/40
        && (int)gridPosition.x+(int)displacement.x>=0 && (int)gridPosition.x+(int)displacement.x<LARGURA/40;
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