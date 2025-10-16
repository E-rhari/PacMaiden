#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"


typedef char** Mapa;

void renderizaMapa(Mapa mapa)
{
    
    int cell = 40;

    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 40; j++)
        {

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
    }

}

void lerMapa (int nivel,Mapa mapa)
{

    char temp;

    char path[50]="PacMaiden/sprites/mapas/mapa";
    char nivelString[3];

    itoa(nivel,nivelString,10);
    strcat(path,nivelString);
    strcat(path,".txt");


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
}

Mapa setUpMapa(){
    Mapa mapa=malloc(sizeof(char*)*20);

    for(int i=0;i<20;i++)
        *(mapa+i)=malloc(sizeof(char)*40);

    return mapa;
}

void freeMapa(Mapa mapa){
    for(int i=0; i<20;i++)
        free(*(mapa+i));
    free(mapa);
}
