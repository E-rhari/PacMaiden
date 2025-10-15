#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"


typedef char Mapa[20][40];
Mapa mapa;

void renderizaMapa()
{
    int cell = 40;
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 40; j++)
        {
            switch(mapa[i][j])
            {
                case '#':
                    DrawRectangle(i * cell, j * cell, cell, cell, DARKBLUE);
                    break;
                
                case '.':
                    DrawCircle(i * cell + cell/2, j * cell + cell/2, 1.5, WHITE);
                    break;
                
                case 'o':
                    DrawCircle(i * cell + cell/2, j * cell + cell/2, 3, GOLD);
                    break;
                
                case 'T':
                    DrawRectangle(i * cell, j * cell, cell, cell, PURPLE);
                    break;
            }
        }
    }
}

void lerMapa (int nivel)
{

    char path[50]="../../../sprites/mapas/mapa";
    char* nivelString;

    itoa(nivel,nivelString,10);
    strcmp(path,nivelString);
    strcmp(path,".txt");

    FILE* arq = fopen(path, "r");
    if(arq == NULL)
    {
        printf("Erro de abertura de arquivo\n");
        return;
    }
    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 40; j++)
           mapa[i][j]= getc(arq);

    fclose(arq);

    renderizaMapa();
}

