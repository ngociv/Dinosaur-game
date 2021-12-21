#ifndef COMMON_FUCTION_H_
#define COMMON_FUCTION_H_

#include "Object.h"

void initProgram();

void initSound();

void initFont();

void initSDL();

void quitProgram();

SDL_Texture* loadTexture(std::string file_name);

character* initCharacter(SDL_Rect characterDstRect);

background* initBackground(SDL_Rect backgroundSrcRect, SDL_Rect backgroundDstRect);

obstacle* initObstacle(std::string file_name, SDL_Rect obstacleSrcRect, SDL_Rect obstacleDstRect);

text* initText(std::string font_name, colorRGB color, int nSize, SDL_Rect textDstRect);


#endif // !COMMON_FUCTION_H_