#ifndef GLOBAL_VARIABLE_H_
#define GLOBAL_VARIABLE_H_

#include "Object.h"
#include "OwnStruct.h"

bool g_bRunGame = false;
bool g_bJump = false;
bool g_bGameOver = false;
bool g_bMoveUp = true;

int g_nScore = 0;
unsigned long g_nTime = 0;
unsigned long g_nSpeedGame = 2;
unsigned long g_nDistance = 0;

SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;
SDL_Event* g_pEvent = new SDL_Event;

colorRGB orange = { 255,165,0 };
colorRGB darkSeaGreen4 = { 105,139,105 };

character* g_pCharacter;
background* g_pBackground_1;
background* g_pBackground_2;
obstacle* g_pFire_1;
obstacle* g_pFire_2;
obstacle* g_pUFO_1;
obstacle* g_pUFO_2;
text* g_pGuide;
text* g_pScore;
text* g_pScorePoint;


#endif // !GLOBAL_VARIABLE_H_