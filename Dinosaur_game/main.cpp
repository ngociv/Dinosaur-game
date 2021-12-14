#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#undef main

using namespace std;

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 416;
const string WINDOW_TITLE = "Dinosaur game by Ngociv";

SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;
SDL_Event* g_pEvent = new SDL_Event;

void initSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void quitSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void loadImage(string file_name, SDL_Renderer* pRenderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
void runGame();
SDL_Texture* loadTexture(string file_name, SDL_Renderer* pRenderer);

class objectMoving
{
private:
	string file_name;
	SDL_Texture* pTexture;

protected:
	SDL_Rect* pSrcRect;
	SDL_Rect* pDstRect;

public:
	objectMoving()
	{
		pSrcRect = nullptr;
		pDstRect = nullptr;
		pTexture = nullptr;
	}

	~objectMoving()
	{
		if (pSrcRect)
		{
			delete pSrcRect;
			pSrcRect = nullptr;
		}
		if (pDstRect)
		{
			delete pDstRect;
			pDstRect = nullptr;
		}
		if (pTexture)
		{
			SDL_DestroyTexture(pTexture);
			pTexture = nullptr;
		}
	}

	void setName(string name)
	{
		file_name = name;
	}

	SDL_Rect* setSrcRect(int x, int y, int w, int h)
	{
		pSrcRect = new SDL_Rect;
		pSrcRect->x = x;
		pSrcRect->y = y;
		pSrcRect->w = w;
		pSrcRect->h = h;
		return pSrcRect;
	}

	SDL_Rect* setDstRect(int x, int y, int w, int h)
	{
		pDstRect = new SDL_Rect;
		pDstRect->x = x;
		pDstRect->y = y;
		pDstRect->w = w;
		pDstRect->h = h;
		return pDstRect;
	}

	SDL_Texture* setTexture()
	{
		pTexture = loadTexture(file_name, g_pRenderer);
		return pTexture;
	}

	void show()
	{
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}

	virtual void move() = 0;
};

class character : public objectMoving
{
public:
	virtual void move()
	{
		pDstRect->y -= 1;
	}
};

int main()
{
	initSDL(g_pWindow, g_pRenderer);
	
	
	runGame();

	quitSDL(g_pWindow, g_pRenderer);
	return 0;
}

void initSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Cannot initialize SDL! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	pWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!pWindow)
	{
		cout << "Cannot create window! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	if (!pRenderer)
	{
		cout << "Cannot create renderer! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	SDL_RenderSetLogicalSize(pRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer)
{
	free(g_pEvent);
	g_pEvent = nullptr;
	SDL_DestroyRenderer(pRenderer);
	pRenderer = nullptr;
	SDL_DestroyWindow(pWindow);
	pWindow = nullptr;
	SDL_Quit();
	return;
}

SDL_Texture* loadTexture(string file_name, SDL_Renderer* pRenderer)
{
	SDL_Surface* pLoad = nullptr;
	SDL_Texture* pTexture = nullptr;

	pLoad = IMG_Load(file_name.c_str());
	if (!pLoad)
	{
		cout << "Cannot load image! Error : " << IMG_GetError() << endl;
		exit(2);
	}
	pTexture = SDL_CreateTextureFromSurface(pRenderer, pLoad);
	if (!pTexture)
	{
		cout << "Cannot create texture from surface! Error : " << SDL_GetError() << endl;
		exit(2);
	}
	SDL_FreeSurface(pLoad);
	pLoad = nullptr;

	return pTexture;
}

void loadImage(string file_name, SDL_Renderer* pRenderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
	SDL_Texture* pImage = loadTexture(file_name, pRenderer);
	SDL_RenderCopy(pRenderer, pImage, srcrect, dstrect);
	SDL_DestroyTexture(pImage);
	pImage = nullptr;
	return;
}

void runGame()
{
	objectMoving* pCharacter = new character;
	int chara_x = 140;
	int chara_y = 252;
	int chara_w = 70;
	int chara_h = 90;
	SDL_Rect* pCharacterDstRect = pCharacter->setDstRect(chara_x, chara_y, chara_w, chara_h);

	pCharacter->setName("character.png");

	SDL_Texture* pCharacterTexture = pCharacter->setTexture();

	bool bStillRun = true;
	while (bStillRun)
	{
		while (SDL_PollEvent(g_pEvent))
		{
			if (g_pEvent->type == SDL_QUIT)
			{
				bStillRun = false;
				break;
			} 
			SDL_RenderClear(g_pRenderer);
			//loadImage("background.png", g_pRenderer, NULL, NULL);
			pCharacter->move();
			pCharacter->show();
	
			SDL_RenderPresent(g_pRenderer);
			SDL_Delay(10);
		}
	}
	return;
}