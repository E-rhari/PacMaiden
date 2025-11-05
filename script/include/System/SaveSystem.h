#include <stdlib.h>
#include <stdio.h>

#include "../Map/Map.h"
#include "../Characters/Character.h"
#include "../Characters/Ghost.h"
#include "../Characters/PacMaiden.h"

#pragma once

// preciso salvar o mapa, as posições dos fantasmas/pacmaiden e o status dos mesmos
void save(Map map, PacMaiden pacMaiden, Ghost* ghost, int saveFile){
    
    char temp='\n';
    char path[50];
    
    #ifdef _WIN32
        strcpy(path,"PacMaiden/sprites/saves/saveMap");
        char saveString[3];

      itoa(saveFile,saveString,10);
        strcat(path,saveString);
        strcat(path,".bin");
    #elif __linux__
        sprintf(path, "../../sprites/maps/map%d.bin", saveFile);
        printf(path);
    #else
        printf("Sistema operacional não detectado. Proseguindo com configuração do linux");
        sprintf(path, "../../sprites/maps/map%d.bin", saveFile);
        printf(path);
    #endif

    FILE* arq = fopen(path, "wb");

    for(int i=0;i<20;i++){
        for(int j=0;j<40;j++)
            fwrite(&map[i][j],sizeof(char),1,arq);
        fwrite(&temp,sizeof(char),1,arq);
    }

    fwrite(&pacMaiden,sizeof(PacMaiden),1,arq);
    fwrite(ghost,sizeof(Ghost),4,arq);

    fclose(arq);
    arq=fopen(path,"rb");

    Map testeMap=setUpMap();
    PacMaiden testePacmain;
    Ghost testeFan[4];


    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 40; j++){
            fread(&temp,sizeof(char),1,arq);
            if(temp !='\n')
                testeMap[i][j] = temp;
        }
        fread(&temp,sizeof(char),1,arq);
    }
    fread(&testePacmain,sizeof(PacMaiden),1,arq);
    fread(testeFan,sizeof(Ghost),4,arq);

    for(int i=0;i<20;i++){
        for(int j=0;j<40;j++)
            printf("%c",testeMap[i][j]);
    }


}