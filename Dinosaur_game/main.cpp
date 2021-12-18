#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#undef main

using namespace std;

bool g_bMove = false;

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 420;
const int BACKGROUND_IMG_WIDTH = 840;
const int BACKGROUND_IMG_HEIGHT = 420;
const int FIRE_IMG_WIDTH = 80;
const int FIRE_IMG_HEIGHT = 80;
const int UFO_IMG_WIDTH = 80;
const int UFO_IMG_HEIGT = 69;
const string WINDOW_TITLE = "Dinosaur game by Ngociv";

unsigned long g_nTime = 0;
unsigned long g_nSpeedGame = 2;

SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;
SDL_Event* g_pEvent = new SDL_Event;

void initSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void quitSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void loadImage(string file_name, SDL_Renderer* pRenderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
void runGame();
SDL_Texture* loadTexture(string file_name, SDL_Renderer* pRenderer);

struct point_2d
{
	int x;
	int y;
};

struct rectangle
{
	point_2d A;
	point_2d B;
	point_2d C;
	point_2d D;
};

struct polygon
{
	point_2d leftHead;
	point_2d rightHead;
	point_2d leftFoot;
	point_2d rightFoot;
	point_2d tail;
};

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

	point_2d getDstRectXY(int plusX, int plusY) {
		point_2d point;
		point.x = pDstRect->x + plusX;
		point.y = pDstRect->y + plusY;
		return point;
	}

	string getName()
	{
		return file_name;
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
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
			pSrcRect->x += g_nSpeedGame;
			pSrcRect->w -= g_nSpeedGame;
			pDstRect->w -= g_nSpeedGame;
			if (pDstRect->w <= g_nSpeedGame)
			{
				pSrcRect->x = 0;
				pSrcRect->w = BACKGROUND_IMG_WIDTH;
				pDstRect->w = SCREEN_WIDTH;
			}
			g_nTime++;
		}
		else
		{
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
			pSrcRect->w += g_nSpeedGame;
			pDstRect->w += g_nSpeedGame;
			pDstRect->x -= g_nSpeedGame;
			if (pDstRect->x <= g_nSpeedGame)
			{
				pSrcRect->w = 0;
				pDstRect->w = 0;
				pDstRect->x = SCREEN_WIDTH;
			}
		}
	}
};

class obstacle : public objectMoving
{
private:
	bool bAppear;
	int srcW;
public:
	obstacle()
	{
		bAppear = false;
		srcW = 0;
	}
	void appear(int srcW) {
		bAppear = true;
		this->srcW = srcW;
	}
	virtual void render()
	{
		if (!bAppear) {
			return;
		}
		if (pSrcRect->w < srcW) {
			pSrcRect->w += g_nSpeedGame;
		}
		pDstRect->x -= g_nSpeedGame;
		if (pDstRect->w < srcW) {
			pDstRect->w += g_nSpeedGame;
		}
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		
		if (pDstRect->x < -srcW)
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

character* initCharacter()
{
	character* pCharacter = new character;

	pCharacter->setName("character.png");

	int dstX = 140;
	int dstY = 257;
	int dstW = 70;
	int dstH = 90;
	pCharacter->setDstRect(dstX, dstY, dstW, dstH);

	pCharacter->setTexture();

	return pCharacter;
}

background* initBackground(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH)
{
	background* pBackground = new background;
	pBackground->setName("background.png");
	pBackground->setSrcRect(srcX, srcY, srcW, srcH);
	pBackground->setDstRect(dstX, dstY, dstW, dstH);
	pBackground->setTexture();
	return pBackground;
}

obstacle* initObstacle(string file_name, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH)
{
	obstacle* pObstacle = new obstacle;
	pObstacle->setName(file_name);
	pObstacle->setSrcRect(srcX, srcY, srcW, srcH);
	pObstacle->setDstRect(dstX, dstY, dstW, dstH);
	pObstacle->setTexture();
	return pObstacle;
}

bool isInside(point_2d& M, rectangle* pRec)
{
	if ((pRec->A.x < M.x) && (M.x < pRec->B.x))
	{
		if ((pRec->A.y < M.y) && (M.y < pRec->C.y))
		{
			return true;
		}
	}
	return false;
}

rectangle* makeRectangle(obstacle* pObs)
{
	int plusX;
	if (pObs->getName() == "fire.png")
	{
		plusX = 20;
	}
	else
	{
		plusX = 0;
	}
	rectangle* pRectangle = new rectangle;
	pRectangle->A = pObs->getDstRectXY(plusX, 5);
	pRectangle->B = pObs->getDstRectXY(65, 5);
	pRectangle->C = pObs->getDstRectXY(66, 45);
	pRectangle->D = pObs->getDstRectXY(plusX, 45);
	return pRectangle;
}

polygon* makePolygon(character* pCharacter)
{
	polygon* pPolygon = new polygon;
	pPolygon->leftHead = pCharacter->getDstRectXY(0, 0);
	pPolygon->rightHead = pCharacter->getDstRectXY(45, 0);
	pPolygon->leftFoot = pCharacter->getDstRectXY(5, 85);
	pPolygon->rightFoot = pCharacter->getDstRectXY(45, 88);
	pPolygon->tail = pCharacter->getDstRectXY(70, 45);
	return pPolygon;
}

void handleEvent()
{
	if (SDL_PollEvent(g_pEvent) == 0)
	{
		return;
	}
	if (g_pEvent->type == SDL_QUIT)
	{
		exit(10);
	}
	if (g_pEvent->type == SDL_KEYDOWN)
	{
		if (g_pEvent->key.keysym.sym == SDLK_SPACE)
		{
			g_bMove = true;
		}
	}
}

bool isImpact(character* pCharacter, obstacle* pObs)
{
	rectangle* pRectangle = makeRectangle(pObs);
	polygon* pPolygon = makePolygon(pCharacter);
	
	if (isInside(pPolygon->leftHead, pRectangle))
	{
		return true;
	}
	if (isInside(pPolygon->rightHead, pRectangle))
	{
		return true;
	}
	if (isInside(pPolygon->leftFoot, pRectangle))
	{
		return true;
	}
	if (isInside(pPolygon->rightFoot, pRectangle))
	{
		return true;
	}
	if (isInside(pPolygon->tail, pRectangle))
	{
		return true;
	}
	return false;
}

void stopGame()
{
	while (true)
	{
		SDL_RenderPresent(g_pRenderer);
		handleEvent();
	}
}

void handleImpact(character* pCharacter, obstacle* pObs_1, obstacle* pObs_2, obstacle* pObs_3, obstacle* pObs_4)
{
	if (isImpact(pCharacter, pObs_1))
	{
		stopGame();
	}
	if (isImpact(pCharacter, pObs_2))
	{
		stopGame();
	}
	if (isImpact(pCharacter, pObs_3))
	{
		stopGame();
	}
	if (isImpact(pCharacter, pObs_4))
	{
		stopGame();
	}
}

void runGame()
{
	character* pCharacter = initCharacter();
	background* pBackground_1 = initBackground(0, 0, BACKGROUND_IMG_WIDTH, BACKGROUND_IMG_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	background* pBackground_2 = initBackground(0, 0, 0, BACKGROUND_IMG_HEIGHT, SCREEN_WIDTH, 0, 0, SCREEN_HEIGHT);
	obstacle* pFire_1 = initObstacle("fire.png", 0, 0, 0, FIRE_IMG_HEIGHT, SCREEN_WIDTH, 270, 0, FIRE_IMG_HEIGHT);
	obstacle* pFire_2 = initObstacle("fire.png", 0, 0, 0, FIRE_IMG_HEIGHT, SCREEN_WIDTH, 270, 0, FIRE_IMG_HEIGHT);
	obstacle* pUFO_1 = initObstacle("ufo.png", 0, 0, 0, UFO_IMG_HEIGT, SCREEN_WIDTH, 160, 0, UFO_IMG_HEIGT);
	obstacle* pUFO_2 = initObstacle("ufo.png", 0, 0, 0, UFO_IMG_HEIGT, SCREEN_WIDTH, 260, 0, UFO_IMG_HEIGT);

	while (true)
	{
		SDL_RenderClear(g_pRenderer);
		
		pBackground_1->render();
		pBackground_2->render();
		pCharacter->render();

		if (((g_nTime - 2) % 200) == 0) {
			srand(time(NULL));
			int nRandom = rand() % 4;
			switch (nRandom)
			{
			case 0: pFire_1->appear(FIRE_IMG_WIDTH); break;
			case 1: pFire_2->appear(FIRE_IMG_WIDTH); break;
			case 2: pUFO_1->appear(UFO_IMG_WIDTH); break;
			case 3: pUFO_2->appear(UFO_IMG_WIDTH); break;
			}
		}
		pFire_1->render();
		pFire_2->render();
		pUFO_1->render();
		pUFO_2->render();

		if ((g_nTime % 1500) == 0) {
			g_nSpeedGame++;
		}
		
		SDL_RenderPresent(g_pRenderer);
		SDL_Delay(11);
		handleEvent();
		handleImpact(pCharacter,pFire_1,pFire_2,pUFO_1,pUFO_2);
	}
}