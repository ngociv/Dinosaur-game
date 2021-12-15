#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#undef main

using namespace std;

bool g_bMove = false;

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 420;
const int BACKGROUND_IMG_WIDTH = 841;
const int BACKGROUND_IMG_HEIGHT = 421;
const int FIRE_IMG_WIDTH = 80;
const int FIRE_IMG_HEIGHT = 80;
const string WINDOW_TITLE = "Dinosaur game by Ngociv";

unsigned long g_nDistance = 0;

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
protected:
	string file_name;
	SDL_Texture* pTexture;
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

	void setSrcRect(int x, int y, int w, int h)
	{
		pSrcRect = new SDL_Rect;
		pSrcRect->x = x;
		pSrcRect->y = y;
		pSrcRect->w = w;
		pSrcRect->h = h;
	}

	void setDstRect(int x, int y, int w, int h)
	{
		pDstRect = new SDL_Rect;
		pDstRect->x = x;
		pDstRect->y = y;
		pDstRect->w = w;
		pDstRect->h = h;
	}

	void setTexture()
	{
		pTexture = loadTexture(file_name, g_pRenderer);
	}

	virtual void render() = 0;
};

class character : public objectMoving
{
public:
	virtual void render()
	{
		if (!g_bMove){
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		}
		else{
			static bool bMoveUp = true;
			if (bMoveUp){
				while (pDstRect->y >= 100){
					SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
					if (pDstRect->y >= 179) {
						pDstRect->y -= 3;
					}
					else {
						pDstRect->y -= 2;
					}
					if (pDstRect->y < 100) {
						bMoveUp = false;
					}
					return;
				}
			}
			else{
				while (pDstRect->y < 257){
					if (pDstRect->y <= 179){
						pDstRect->y += 2;
					}
					else{
						pDstRect->y += 3;
					}
					SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
					if (pDstRect->y >= 257){
						bMoveUp = true;
						g_bMove = false;
					}
					return;
				}
			}
		}
	}
};

class background : public objectMoving
{
public:
	virtual void render()
	{
		if (pDstRect->x == 0)
		{
			pSrcRect->x += 2;
			pSrcRect->w -= 2;
			pDstRect->w -= 2;
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
			if (pDstRect->w == 2)
			{
				pSrcRect->x = 0;
				pSrcRect->w = BACKGROUND_IMG_WIDTH;
				pDstRect->w = SCREEN_WIDTH;
			}
			g_nDistance += 2;
			return;
		}
		else
		{
			pSrcRect->w += 2;
			pDstRect->w += 2;
			pDstRect->x -= 2;
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
			if (pDstRect->x == 2)
			{
				pSrcRect->w = 0;
				pDstRect->w = 0;
				pDstRect->x = SCREEN_WIDTH;
			}
			return;
		}
	}
};

class obstacle : public objectMoving
{
private:
	bool bAppear;
public:
	obstacle()
	{
		bAppear = false;
	}
	void appear() {
		bAppear = true;
	}
	virtual void render()
	{
		if (!bAppear) {
			return;
		}
		if (pSrcRect->w < FIRE_IMG_WIDTH) {
			pSrcRect->w += 2;
		}
		pDstRect->x -= 2;
		if (pDstRect->w < FIRE_IMG_WIDTH) {
			pDstRect->w += 2;
		}
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		
		if (pDstRect->x < -FIRE_IMG_WIDTH)
		{
			bAppear = false;
			pSrcRect->w = 0;
			pDstRect->x = SCREEN_WIDTH;
			pDstRect->w = 0;
		}
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

objectMoving* initCharacter()
{
	objectMoving* pCharacter = new character;

	pCharacter->setName("character.png");

	int dstX = 140;
	int dstY = 257;
	int dstW = 70;
	int dstH = 90;
	pCharacter->setDstRect(dstX, dstY, dstW, dstH);

	pCharacter->setTexture();

	return pCharacter;
}

objectMoving* initBackground(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH)
{
	objectMoving* pBackground = new background;
	pBackground->setName("background.png");
	pBackground->setSrcRect(srcX, srcY, srcW, srcH);
	pBackground->setDstRect(dstX, dstY, dstW, dstH);
	pBackground->setTexture();
	return pBackground;
}

obstacle* initObstacle(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH)
{
	obstacle* pObstacle = new obstacle;
	pObstacle->setName("fire.png");
	//pObstacle->setSrcRect(0, 0, 0, FIRE_IMG_HEIGHT);
	pObstacle->setSrcRect(srcX, srcY, srcW, srcH);
	//pObstacle->setDstRect(SCREEN_WIDTH, 270, 0, FIRE_IMG_HEIGHT);
	pObstacle->setDstRect(dstX, dstY, dstW, dstH);
	pObstacle->setTexture();
	return pObstacle;
}

void runGame()
{
	objectMoving* pCharacter = initCharacter();
	objectMoving* pBackground1 = initBackground(0, 0, BACKGROUND_IMG_WIDTH, BACKGROUND_IMG_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	objectMoving* pBackground2 = initBackground(0, 0, 0, BACKGROUND_IMG_HEIGHT, SCREEN_WIDTH, 0, 0, SCREEN_HEIGHT);
	obstacle* pObstacle1 = initObstacle(0, 0, 0, FIRE_IMG_HEIGHT, SCREEN_WIDTH, 270, 0, FIRE_IMG_HEIGHT);
	obstacle* pObstacle2 = initObstacle(0, 0, 0, FIRE_IMG_HEIGHT, SCREEN_WIDTH, 270, 0, FIRE_IMG_HEIGHT);

	while (true)
	{
		SDL_RenderClear(g_pRenderer);
		
		pBackground1->render();
		pBackground2->render();
		pCharacter->render();

		if (((g_nDistance - 2) % 960) == 0) {
			pObstacle1->appear();
		}
		if (((g_nDistance + 420) % 960) == 0) {
			pObstacle2->appear();
		}
		pObstacle1->render();
		pObstacle2->render();
		
		SDL_RenderPresent(g_pRenderer);
		SDL_Delay(11);
		
		if (SDL_PollEvent(g_pEvent) == 0)
		{
			continue;
		}
		if (g_pEvent->type == SDL_QUIT)
		{
			break;
		}
		if (g_pEvent->type == SDL_KEYDOWN)
		{
			if (g_pEvent->key.keysym.sym == SDLK_SPACE)
			{
				g_bMove = true;
			}
		}
	}
	return;
}