#include <stdlib.h>
#include <stdio.h>

#include "../Map/Map.h"
#include "../Characters/Character.h"
#include "../Characters/Ghosts/Ghost.h"
#include "../Characters/PacMaiden.h"

#pragma once
/**
 * @brief gera o caminho do save 
 * 
 * @param saveFile o numero do arquivo.
 * @return o caminho do arquivo a ser acessado
 */
char* selecSaveFile(int saveFile){
    char *path= (char*)malloc(sizeof(char)*50);

    sprintf(path, getFilePath("../../sprites/saves/saveMap%d.txt"), saveFile);

    return path;
}

/**
 * @brief salva todas as informações das structs: PacMaiden, Ghost e Map em um arquivo .bin
 * 
 * @param map o estado mapa atual
 * @param pacMaiden as informações da pacmaiden
 * @param ghost as informações dos fantasmas
 * @param saveFile o numero do arquivo a ser salvo
 */
void save(Map map, PacMaiden pacMaiden, Ghost* ghost, int saveFile){

    char*path=selecSaveFile(saveFile);

    char temp='\n';
    FILE* arq = fopen(path, "wb");

    for(int i=0;i<20;i++){
        for(int j=0;j<40;j++)
            fwrite(&map[i][j],sizeof(char),1,arq);
        fwrite(&temp,sizeof(char),1,arq);
    }

    fwrite(&pacMaiden,sizeof(PacMaiden),1,arq);
    fwrite(ghost,sizeof(Ghost),4,arq);

    fclose(arq);
    free(path);

}
/**
 * @brief carrega de um arquivo binário todas as informações salvas em um .bin
 * 
 * @param map o estado mapa do arquivo
 * @param pacMaiden as informações da pacmaiden do arquivo
 * @param ghost as informações dos fantasmas do arquivo
 * @param saveFile o numero do arquivo a ser lido
 */
bool load(Map map, PacMaiden* pacMaiden, Ghost* ghost, int saveFile){

    char* path=selecSaveFile(saveFile);
    FILE* arq=fopen(path,"rb");
    if(arq==NULL)
        return false;
    char temp;
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 40; j++){
            fread(&temp,sizeof(char),1,arq);
            if(temp !='\n')
                map[i][j] = temp;
        }
        fread(&temp,sizeof(char),1,arq);
    }
    fread(pacMaiden,sizeof(PacMaiden),1,arq);
    fread(ghost,sizeof(Ghost),4,arq);
    free(path);
    return true;
}