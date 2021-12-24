#ifndef COMMON_FUCTION_H_
#define COMMON_FUCTION_H_

#include "Object.h"

void initProgram();

void initSound();

void initFont();

void initSDL();

void quitProgram();

SDL_Texture* loadTexture(const std::string& file_name);

character* initCharacter(const SDL_Rect& characterDstRect);

background* initBackground(const SDL_Rect& backgroundSrcRect, const SDL_Rect& backgroundDstRect);

obstacle* initObstacle(std::string file_name, const SDL_Rect& obstacleSrcRect, const SDL_Rect& obstacleDstRect);

text* initText(std::string font_name, const colorRGB& color, const int& nSize, const SDL_Rect& textDstRect);


#endif // !COMMON_FUCTION_H_