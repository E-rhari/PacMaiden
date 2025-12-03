#include "raylib.h"

#pragma once

#define SONG_AMOUT 2
#define SOUND_AMOUNT 3


typedef enum {
    MAIN_THEME,
    PAUSED_MAIN_THEME
}Track;

typedef enum {
    PELLET,
    SUPER_PELLET,
    EAT_GHOST,
    DIE
} Sfx;


Music* initiateMusic(Music* tracks){
    tracks[MAIN_THEME]        = LoadMusicStream(getFilePath("../../audio/Music/MainTheme/MainTheme.wav"));
    tracks[PAUSED_MAIN_THEME] = LoadMusicStream(getFilePath("../../audio/Music/MainTheme/PausedMainTheme.wav"));

    PlayMusicStream(tracks[MAIN_THEME]);
    PlayMusicStream(tracks[PAUSED_MAIN_THEME]);

    return tracks;
}


void stopMusic(Music* tracks){
    for(int i=0; i<SONG_AMOUT; i++)
        StopMusicStream(tracks[i]);
}


void freeMusic(Music* tracks){
    stopMusic(tracks);
    for(int i=0; i<SONG_AMOUT; i++)
        UnloadMusicStream(tracks[i]);
}


void focusTrack(Music* tracks, Track trackName){
    for(int i=0; i<SONG_AMOUT; i++)
        SetMusicVolume(tracks[i], 0.0f);
    SetMusicVolume(tracks[trackName], 1.0f);
}


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
    
    for (int i = 0; i < SOUND_AMOUNT; i++)
        SetSoundVolume(effects[i], .4f);
    return effects;
}