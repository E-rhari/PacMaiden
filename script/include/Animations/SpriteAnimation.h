/** @brief Animações de sprite renderizam de arquivos externos imagens, chamadas de "frames", alternando entre elas em um intervalo de tempo regular. */

#include "raylib.h"
#include <stdlib.h>

#include "../System/WindowControl.h"


#pragma once

/** @brief Enum dos sprites que os elementos do jogo podem assumir*/
typedef enum {
    YELLOW_PACMAIDEN_SPRITE,
    YELLOW_POWERED_PACMAIDEN_SPRITE,
    PURPLE_PACMAIDEN_SPRITE,
    PURPLE_POWERED_PACMAIDEN_SPRITE,

    RED_GHOST_SPRITE,
    BLUE_GHOST_SPRITE,
    PINK_GHOST_SPRITE,
    ORANGE_GHOST_SPRITE,
    WAEK_GHOST_SPRITE,

    TILESET_SPRITE,
    PELLET_SPRITE,
    POWER_PELLET_SPRITE,
    PORTAL_SPRITE,

    TITLE_SCREEN_SPRITE,
    AMOUNT_OF_SPRITES
} SpriteName;


Texture* SPRITES;

// Todo o lugar que tiver /40, troca pra * PIX2GRID, e todo lugar que tiver *40, troca por GRID2PIX. 💋💋💋💋💖💖💖

/** @brief Cuida das propriedades de uma animação baseada numa imagem sprite sheet.
* @param spriteSheet    Textura do spritesheet completo.
* @param mask           Mscara do sprite
* @param frameSize      Tamanho de um único frame.
* @param tint           Coloração procedural do sprite
* @param first          Index do primeiro frame da animação.
* @param last           Index do último frame da animação.
* @param current        Index do frame atual.
* @param lineSelect     Index da linha de animação a ser percorrida.
* @param step           Quantidade de frames a ser pulados a cada iteração da animação.
* @param speed          Velocidade da animação. Quantas vezes por segundo a animação muda de frame.
* @param timeInFrame    Quantidade de tempo que a animação está no frame atual
* @param loop           Se a animação reinicia quando termina.
* @param running        Se a animação deve correr.
*/
typedef struct {
    Texture2D spriteSheet;   
    Texture2D mask;
    Vector2 frameSize;      
    Color tint;            

    int first;              
    int last;               
    int current;            
    int lineSelect;         
    int step;              

    float speed;            
    float timeInFrame;      
    bool loop;              
    bool running;           
} SpriteAnimation; 


/** @brief Instancia uma Sprite Animation
 * @param spriteSheet Caminho para a imagem a ser usada como spritesheet.
 * @param frameSize Tamanho de um único frame.
 * @param speed Velocidade da animação. Quantas vezes por segundo a animação muda de frame.
 * @param loop Se a animação reinicia quando termina. */
SpriteAnimation innitSpriteAnimation(SpriteName sprite, Vector2 frameSize, float speed, bool loop){
    Image spriteSheetImage = LoadImageFromTexture(SPRITES[sprite]);
    Image mask = ImageFromChannel(spriteSheetImage, 3);
    ImageAlphaMask(&mask, mask);
    Texture whiteSpriteSheetTexture = LoadTextureFromImage(mask); 

    UnloadImage(spriteSheetImage);
    UnloadImage(mask);
    
    return (SpriteAnimation) {
        .spriteSheet=SPRITES[sprite],
        .mask=whiteSpriteSheetTexture,
        .tint = WHITE,

        .first=0,
        .last=(SPRITES[sprite].width/frameSize.x)-1,
        .current=0,
        .lineSelect=0,
        .step=1,
        
        .speed=speed,
        .timeInFrame=0,

        .frameSize=frameSize,
        .loop=loop,
        .running=true,
    };
}


void stopSpriteAnimation(SpriteAnimation* animation){
    animation->running = false;
}


void playSpriteAnimation(SpriteAnimation* animation, int animationRow){
    animation->lineSelect = animationRow;
    animation->running = true;
}


/** @brief Seleciona a região da spritesheet que se encontra o frame atual */
Rectangle getSpriteFrame(SpriteAnimation* animation){
    float x = animation->current*animation->frameSize.x;
    float y = animation->lineSelect*animation->frameSize.y;
    return (Rectangle){x, y, animation->frameSize.x, animation->frameSize.y};
}


void updateSpriteAnimation(SpriteAnimation* animation){
    if(!animation->running)
        return;

    float timePerFrame = 1/animation->speed;
    animation->timeInFrame += GetFrameTime();

    if(animation->timeInFrame >= timePerFrame){
        animation->current += animation->step;

        if(animation->current > animation->last){
            if(animation->loop)
                animation->current = (animation->current%animation->last)+animation->first-1; 
            else{
                animation->current = animation->last;
                animation->running = false;
            }
        }
        animation->timeInFrame = 0;
    }
}


void changeSprite(SpriteAnimation* animation, SpriteName spriteSheet){
    animation->spriteSheet = SPRITES[spriteSheet];
}


void changeMask(SpriteAnimation* animation, SpriteName maskSheet){
    UnloadTexture(animation->mask);
    Image maskImage = LoadImageFromTexture(SPRITES[maskSheet]);
    Image mask = ImageFromChannel(maskImage, 3);
    ImageAlphaMask(&mask, mask);
    animation->mask = LoadTextureFromImage(mask);
}


void changeSpriteAndMask(SpriteAnimation* animation, SpriteName spriteSheet){
    changeSprite(animation, spriteSheet);
    changeMask(animation, spriteSheet);
}


void drawSpriteAnimation(SpriteAnimation* sprite, Vector2 position, Vector2 scale){
    Vector2 spriteVirtualSize = {sprite->frameSize.x*scale.x, sprite->frameSize.y*scale.y};
    DrawTexturePro(sprite->spriteSheet, getSpriteFrame(sprite), (Rectangle){position.x, position.y, spriteVirtualSize.x, spriteVirtualSize.y}, (Vector2){0,0}, 0, sprite->tint);
}


void loadAllSprites(){
    if(SPRITES != NULL)
        free(SPRITES);
    SPRITES = (Texture*)malloc(sizeof(Texture)*AMOUNT_OF_SPRITES);

    SPRITES[YELLOW_PACMAIDEN_SPRITE]         = LoadTexture(getFilePath("../../sprites/pacmaiden/PacMaidenYellow.png"));
    SPRITES[YELLOW_POWERED_PACMAIDEN_SPRITE] = LoadTexture(getFilePath("../../sprites/pacmaiden/PacMaidenYellowPower.png"));
    SPRITES[PURPLE_PACMAIDEN_SPRITE]         = LoadTexture(getFilePath("../../sprites/pacmaiden/PacMaidenPurple.png"));
    SPRITES[PURPLE_POWERED_PACMAIDEN_SPRITE] = LoadTexture(getFilePath("../../sprites/pacmaiden/PacMaidenPurplePower.png"));

    SPRITES[RED_GHOST_SPRITE]    = LoadTexture(getFilePath("../../sprites/ghosts/Ghost0.png"));
    SPRITES[BLUE_GHOST_SPRITE]   = LoadTexture(getFilePath("../../sprites/ghosts/Ghost1.png"));
    SPRITES[PINK_GHOST_SPRITE]   = LoadTexture(getFilePath("../../sprites/ghosts/Ghost2.png"));
    SPRITES[ORANGE_GHOST_SPRITE] = LoadTexture(getFilePath("../../sprites/ghosts/Ghost3.png"));
    SPRITES[WAEK_GHOST_SPRITE]   = LoadTexture(getFilePath("../../sprites/ghosts/WeakGhost.png"));

    SPRITES[TILESET_SPRITE]      = LoadTexture(getFilePath("../../sprites/tiles/Tilesheet.png"));
    SPRITES[PELLET_SPRITE]       = LoadTexture(getFilePath("../../sprites/pellets/Pellet.png"));
    SPRITES[POWER_PELLET_SPRITE] = LoadTexture(getFilePath("../../sprites/pellets/PowerPellet.png"));
    SPRITES[PORTAL_SPRITE]       = LoadTexture(getFilePath("../../sprites/tiles/Portal.png"));

    SPRITES[TITLE_SCREEN_SPRITE] = LoadTexture(getFilePath("../../sprites/title/TitleScreen.png"));
}


void unloadAllSprites(){
    for(int i=0; i<AMOUNT_OF_SPRITES; i++)
        UnloadTexture(SPRITES[i]);
    free(SPRITES);
}