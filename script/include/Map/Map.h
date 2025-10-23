#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "../System/WindowControl.h"

#pragma once


typedef char** Map;

void drawMap(Map map)
{
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
        temp = getc(arq);
    }
    fclose(arq);
    return;
}

Map setUpMap(){
    Map map = (char**)malloc(sizeof(char*)*20);

    for(int i=0;i<20;i++)
        *(map+i) = (char*)malloc(sizeof(char)*40);
    return map;
}

void freeMap(Map map){
    for(int i=0; i<20;i++)
        free(*(map+i));
    free(map);
}