#include <iostream>

#include <SDL_image.h>
#include <SDL_mixer.h>

#include "CommonFuction.h"
#include "Constant.h"
#include "GlobalVariable.h"

using namespace std;

void initProgram()
{
	initSound();
	initFont();
	initSDL();
}

void initSound()
{
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, CHUNKSIZE) == -1)
	{
		cout << "Cannot open audio! Error : " << Mix_GetError() << endl;
		exit(1);
	}
}

void initFont()
{
	if (TTF_Init() == -1)
	{
		cout << "Cannot initialize TTF! Error : " << TTF_GetError() << endl;
		exit(1);
	}
}

void initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Cannot initialize SDL! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	g_pWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!g_pWindow)
	{
		cout << "Cannot create window! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
	if (!g_pRenderer)
	{
		cout << "Cannot create renderer! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	SDL_RenderSetLogicalSize(g_pRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitProgram()
{
	free(g_pEvent);
	g_pEvent = nullptr;
	SDL_DestroyRenderer(g_pRenderer);
	g_pRenderer = nullptr;
	SDL_DestroyWindow(g_pWindow);
	g_pWindow = nullptr;
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(string file_name)
{
	SDL_Surface* pLoad = nullptr;
	SDL_Texture* pTexture = nullptr;

	pLoad = IMG_Load(file_name.c_str());
	if (!pLoad)
	{
		cout << "Cannot load image! Error : " << IMG_GetError() << endl;
		exit(2);
	}
	pTexture = SDL_CreateTextureFromSurface(g_pRenderer, pLoad);
	if (!pTexture)
	{
		cout << "Cannot create texture from surface! Error : " << SDL_GetError() << endl;
		exit(2);
	}
	SDL_FreeSurface(pLoad);
	pLoad = nullptr;

	return pTexture;
}

character* initCharacter(SDL_Rect characterDstRect)
{
	character* pCharacter = new character;
	pCharacter->setName(CHARACTER_NAME);
	pCharacter->setDstRect(characterDstRect);
	pCharacter->setTexture();
	return pCharacter;
}

background* initBackground(SDL_Rect backgroundSrcRect, SDL_Rect backgroundDstRect)
{
	background* pBackground = new background;
	pBackground->setName(BACKGROUND_NAME);
	pBackground->setSrcRect(backgroundSrcRect);
	pBackground->setDstRect(backgroundDstRect);
	pBackground->setTexture();
	return pBackground;
}

obstacle* initObstacle(string file_name, SDL_Rect obstacleSrcRect, SDL_Rect obstacleDstRect)
{
	obstacle* pObstacle = new obstacle;
	pObstacle->setName(file_name);
	pObstacle->setSrcRect(obstacleSrcRect);
	pObstacle->setDstRect(obstacleDstRect);
	pObstacle->setTexture();
	return pObstacle;
}

text* initText(string font_name, colorRGB color, int nSize, SDL_Rect textDstRect)
{
	text* pText = new text;
	pText->setName(font_name);
	pText->setColor(color);
	pText->setFont(nSize);
	pText->setDstRect(textDstRect);
	return pText;
}
