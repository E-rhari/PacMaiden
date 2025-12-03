#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma once



char* getFilePath(char* path){
    #ifdef _WIN32
        int i=0;
        for(i=0; path[i]=='.' || path[i]=='/'; i++)
            continue;

        char newPath[200] = "PacMaiden/";
        strcat(newPath, &path[i]);
        path = (char*)malloc(sizeof(char)*200);
        strcpy(path, newPath);
        return path;
    #endif

    return path;
}