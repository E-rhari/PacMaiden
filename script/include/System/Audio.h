#include "raylib.h"

#pragma once
 

/** @brief Enumeração das possíveis músicas a serem tocadas em  levels */
typedef enum {
    MAIN_THEME,
    PAUSED_MAIN_THEME,
    SONG_AMOUT // Quantidade de músicas tocando simultanêamente durante o levels
}Track;

/** @brief Enumeração dos possíveis SFX a serem tocadas em  levels */
typedef enum {
    PELLET,
    SUPER_PELLET,
    EAT_GHOST,
    DEATH,
    SOUND_AMOUNT // Quantidade de efeitos sonoros a serem carregados em levels
} Sfx;


/** @brief Carrega todas as músicas a serem tocas simultanêamente no levels */
Music* initiateMusic(Music* tracks){
    tracks[MAIN_THEME]        = LoadMusicStream(getFilePath("../../audio/Music/MainTheme/MainTheme.wav"));
    tracks[PAUSED_MAIN_THEME] = LoadMusicStream(getFilePath("../../audio/Music/MainTheme/PausedMainTheme.wav"));

    PlayMusicStream(tracks[MAIN_THEME]);
    PlayMusicStream(tracks[PAUSED_MAIN_THEME]);

    return tracks;
}


/** @brief Para todas as músicas a serem tocas simultanêamente no levels */
void stopMusic(Music* tracks){
    for(int i=0; i<SONG_AMOUT; i++)
        StopMusicStream(tracks[i]);
}


/** @brief Descarrega todas as músicas a serem tocas simultanêamente no levels */
void freeMusic(Music* tracks){
    stopMusic(tracks);
    for(int i=0; i<SONG_AMOUT; i++)
        UnloadMusicStream(tracks[i]);
}


/** @brief Coloca o volume de uma música no máximo e muta todas as outras */
void focusTrack(Music* tracks, Track trackName){
    for(int i=0; i<SONG_AMOUT; i++)
        SetMusicVolume(tracks[i], 0.0f);
    SetMusicVolume(tracks[trackName], 1.0f);
}


/** @brief Controla o pause e atualização da música */
void handleMusic(Music* tracks, bool pause){
    if(pause)
        focusTrack(tracks, PAUSED_MAIN_THEME);
    else
        focusTrack(tracks, MAIN_THEME);

    for(int i=0; i<SONG_AMOUT; i++)
        UpdateMusicStream(tracks[i]);
}


Sound* initiateSFX(Sound* effects){
    effects[PELLET]       = LoadSound(getFilePath("../../audio/SFX/pellet.wav"));
    effects[SUPER_PELLET] = LoadSound(getFilePath("../../audio/SFX/superPellet.wav"));
    effects[EAT_GHOST]    = LoadSound(getFilePath("../../audio/SFX/eatGhost.wav"));
    effects[DEATH]        = LoadSound(getFilePath("../../audio/SFX/death_5.wav"));
    
    //Diminui o volume de todos efeitos sonoros, exceto o úlitmo (Som de morte)
    for (int i = 0; i < SOUND_AMOUNT - 1; i++)
        SetSoundVolume(effects[i], .4f);
    return effects;
}


void freeSFX(Sound* effects){
    for(int i=0; i<SOUND_AMOUNT; i++)
        UnloadSound(effects[i]);
}