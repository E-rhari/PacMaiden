#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "../WindowControl.h"

#pragma once


typedef char** Mapa;

void renderizaMapa(Mapa mapa)
{
    int cell = 40;

    for(int i = 0; i < LARGURA/40; i++)
        for(int j = 0; j < ALTURA/40; j++)
            switch(mapa[i][j])
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
                    break;
                
                case 't':
                    DrawRectangle(j * cell, i * cell, cell, cell, PURPLE);
                    break;
                case 'P':
                    DrawCircle(j * cell + cell/2, i * cell + cell/2, 10, GREEN);
                    break;
            } 
}

void lerMapa (int nivel,Mapa mapa)
{
    char temp;

    char path[50];
    sprintf(path, "../../sprites/maps/map%d.txt", nivel);

    if(DEBUG_MODE)
        printf(path);
        
    FILE* arq = fopen(path, "r");

    if(arq == NULL)
    {
        printf("Erro de abertura de arquivo\n");
        return;
    }
    
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 41; j++){
            temp= getc(arq);
            if(temp =='\n')
                continue;
            mapa[i][j]= temp;
        }
    }
    fclose(arq);
    return;
}

Mapa setUpMapa(){
    Mapa mapa=malloc(sizeof(char*)*(LARGURA/40));

    for(int i=0;i<20;i++)
        *(mapa+i)=malloc(sizeof(char)*(ALTURA/40));

    return mapa;
}

void freeMapa(Mapa mapa){
    for(int i=0; i<20;i++)
        free(*(mapa+i));
    free(mapa);
}
