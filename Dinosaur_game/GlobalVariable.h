#ifndef GLOBAL_VARIABLE_H_
#define GLOBAL_VARIABLE_H_

#include <SDL_mixer.h>

#include "Object.h"
#include "OwnStruct.h"

bool g_bJump = false;
bool g_bMoveUp = true;
bool g_bRunGame = false;
bool g_bGameOver = false;

int g_nScore = 0;
unsigned long g_nTime = 0;
unsigned long g_nDistance = 0;
unsigned long g_nGameSpeed = 2;

SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;
SDL_Event* g_pEvent = new SDL_Event;

colorRGB orange = { 255,165,0 };
colorRGB lightYellow4 = { 139,139,122 };

text* g_pScore;
text* g_pScorePoint;

obstacle* g_pUFO_1;
obstacle* g_pUFO_2;
obstacle* g_pFire_1;
obstacle* g_pFire_2;

character* g_pCharacter;

background* g_pBackground_1;
background* g_pBackground_2;

Mix_Chunk* g_pJumpSound;
Mix_Chunk* g_pBackgroundSound;

//rectangle that represent obstacle
//polygon with 5 vertices that represent character
//used for impact handling
polygon* g_pPolygon = new polygon;
rectangle* g_pRectangle = new rectangle;


#endif // !GLOBAL_VARIABLE_H_