#include <iostream>

#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constant.h"
#include "CommonFuction.h"


using namespace std;

extern SDL_Event* g_pEvent;
extern SDL_Window* g_pWindow;
extern SDL_Renderer* g_pRenderer;

extern text* g_pScore;
extern text* g_pScorePoint;

extern obstacle* g_pUFO_1;
extern obstacle* g_pUFO_2;
extern obstacle* g_pFire_1;
extern obstacle* g_pFire_2;

extern character* g_pCharacter;

extern background* g_pBackground_1;
extern background* g_pBackground_2;

extern Mix_Chunk* g_pJumpSound;
extern Mix_Chunk* g_pBackgroundSound;

extern polygon* g_pPolygon;
extern rectangle* g_pRectangle;

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
	Mix_FreeChunk(g_pJumpSound);
	Mix_FreeChunk(g_pBackgroundSound);
	Mix_CloseAudio();
	delete(g_pEvent);
	delete(g_pRectangle);
	delete(g_pPolygon);
	delete(g_pScore);
	delete(g_pScorePoint);
	delete(g_pCharacter);
	delete(g_pUFO_1);
	delete(g_pUFO_2);
	delete(g_pFire_1);
	delete(g_pFire_2);
	delete(g_pBackground_1);
	delete(g_pBackground_2);
	TTF_Quit();
	SDL_DestroyRenderer(g_pRenderer);
	SDL_DestroyWindow(g_pWindow);
	SDL_Quit();
}

//create texture of object to get ready to render
SDL_Texture* loadTexture(const string& file_name)
{
	SDL_Surface* pSurface = nullptr;
	SDL_Texture* pTexture = nullptr;

	pSurface = IMG_Load(file_name.c_str());
	if (!pSurface)
	{
		cout << "Cannot load image! Error : " << IMG_GetError() << endl;
		exit(2);
	}
	pTexture = SDL_CreateTextureFromSurface(g_pRenderer, pSurface);
	if (!pTexture)
	{
		cout << "Cannot create texture from surface! Error : " << SDL_GetError() << endl;
		exit(2);
	}
	SDL_FreeSurface(pSurface);
	pSurface = nullptr;

	return pTexture;
}

character* initCharacter(const SDL_Rect& characterDstRect)
{
	character* pCharacter = new character;
	pCharacter->setName(CHARACTER_NAME);
	pCharacter->setDstRect(characterDstRect);
	pCharacter->setTexture();
	return pCharacter;
}

background* initBackground(const SDL_Rect& backgroundSrcRect, const SDL_Rect& backgroundDstRect)
{
	background* pBackground = new background;
	pBackground->setName(BACKGROUND_NAME);
	pBackground->setSrcRect(backgroundSrcRect);
	pBackground->setDstRect(backgroundDstRect);
	pBackground->setTexture();
	return pBackground;
}

obstacle* initObstacle(string file_name, const SDL_Rect& obstacleSrcRect, const SDL_Rect& obstacleDstRect)
{
	obstacle* pObstacle = new obstacle;
	pObstacle->setName(file_name);
	pObstacle->setSrcRect(obstacleSrcRect);
	pObstacle->setDstRect(obstacleDstRect);
	pObstacle->setTexture();
	return pObstacle;
}

text* initText(string font_name, const colorRGB& color, const int& nSize, const SDL_Rect& textDstRect)
{
	text* pText = new text;
	pText->setName(font_name);
	pText->setColor(color);
	pText->setFont(nSize);
	pText->setDstRect(textDstRect);
	return pText;
}
