#include <stdio.h>
#include <string.h>
#include "raylib.h"
void lerMapa (char** matriz, char* arq)
{
    FILE* arq = fopen("mapa.txt", "r");
    if(arq == NULL)
    {
        printf("Erro de abertura de arquivo\n");
        return;
    }
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 40; j++)
        {
            *(*(matriz + i) + j) = getc(arq);
        }
    }
    fclose(arq);
}

void renderizaMapa(char** matriz)
{
    int cell = 40;
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 40; j++)
        {
            switch(*(*(matriz + i) + j))
            {
                case "#":
                    DrawRectangle(i * cell, j * cell, cell, cell, DARKBLUE);
                    break;
                
                case ".":
                    DrawCircle(i * cell + cell/2, j * cell + cell/2, 1.5, WHITE);
                    break;
                
                case "o":
                    DrawCircle(i * cell + cell/2, j * cell + cell/2, 3, GOLD);
                    break.
                
                case "T":
                    DrawRectangle(i * cell, j * cell, cell, cell, PURPLE);
                    break;
            }
        }
    }
}