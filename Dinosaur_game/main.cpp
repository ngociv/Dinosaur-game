#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#undef main

using namespace std;

bool g_bRunGame = false;
bool g_bJump = false;
bool g_bGameOver = false;
bool g_bMoveUp = true;

const int SCREEN_WIDTH = 841;
const int SCREEN_HEIGHT = 421;
const int BACKGROUND_IMG_WIDTH = 841;
const int BACKGROUND_IMG_HEIGHT = 421;
const int FIRE_IMG_WIDTH = 80;
const int FIRE_IMG_HEIGHT = 80;
const int UFO_IMG_WIDTH = 80;
const int UFO_IMG_HEIGT = 69;
const string WINDOW_TITLE = "Dinosaur game by Ngociv";

unsigned long g_nTime = 0;
unsigned long g_nSpeedGame = 2;
unsigned long g_nDistance = 0;
int g_nScorePoint = 0;

SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;
SDL_Event* g_pEvent = new SDL_Event;

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

struct colorRGB
{
	int r;
	int g;
	int b;
};

colorRGB orange = { 255,165,0 };
colorRGB darkSeaGreen4 = { 105,139,105 };

void initSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void quitSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void openingGame();
void runGame();
SDL_Texture* loadTexture(string file_name, SDL_Renderer* pRenderer);

class object
{
protected:
	string file_name;
	SDL_Texture* pTexture;
	SDL_Rect* pSrcRect;
	SDL_Rect* pDstRect;

public:
	object()
	{
		pSrcRect = nullptr;
		pDstRect = nullptr;
		pTexture = nullptr;
	}

	~object()
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

class character : public object
{
public:
	void beInGuide()
	{
		pDstRect->y = -90;
	}
	void beInGame()
	{
		pDstRect->y = 257;
	}
	void appear()
	{
		if (pDstRect->y < 257)
		{
			pDstRect->y++;
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		}
		else
		{
			g_bRunGame = true;
		}
	}
	virtual void render()
	{
		if (!g_bJump){
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		}
		else{
			if (g_bMoveUp){
				if (pDstRect->y >= 100){
					SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
					if (pDstRect->y >= 179) {
						pDstRect->y -= 3;
					}
					else {
						pDstRect->y -= 2;
					}
					if (pDstRect->y < 100) {
						g_bMoveUp = false;
					}
				}
			}
			else{
				if (pDstRect->y < 257){
					if (pDstRect->y <= 179){
						pDstRect->y += 2;
					}
					else{
						pDstRect->y += 3;
					}
					SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
					if (pDstRect->y >= 257){
						g_bMoveUp = true;
						g_bJump = false;
					}
				}
			}
		}
	}
};

class background : public object
{
public:
	void beInGuide()
	{
		pSrcRect->x = 0;
		pSrcRect->w = BACKGROUND_IMG_WIDTH;
		pDstRect->w = SCREEN_WIDTH;
	}
	void beInGame()
	{
		pSrcRect->w = 0;
		pDstRect->w = 0;
		pDstRect->x = SCREEN_WIDTH;
	}
	void appear()
	{
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
	virtual void render()
	{
		if (pDstRect->x == 0)
		{
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
			pSrcRect->x += g_nSpeedGame;
			pSrcRect->w -= g_nSpeedGame;
			pDstRect->w -= g_nSpeedGame;
			if (pDstRect->w <= 0)
			{
				pSrcRect->x = -pDstRect->w;
				pSrcRect->w = BACKGROUND_IMG_WIDTH - pSrcRect->x;
				pDstRect->w = pSrcRect->w;
			}
			g_nTime++;
			g_nDistance += g_nSpeedGame;
			if ((g_nDistance % 80) < g_nSpeedGame) {
				g_nScorePoint++;
			}
		}
		else
		{
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
			pSrcRect->w += g_nSpeedGame;
			pDstRect->w += g_nSpeedGame;
			pDstRect->x -= g_nSpeedGame;
			if (pDstRect->x <= 0)
			{
				pSrcRect->w = -pDstRect->x;
				pDstRect->w = pSrcRect->w;
				pDstRect->x = SCREEN_WIDTH - pDstRect->w;
			}
		}
	}
};

class obstacle : public object
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
	void beInGame()
	{
		bAppear = false;
		pSrcRect->w = 0;
		pDstRect->x = SCREEN_WIDTH;
		pDstRect->w = 0;
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

class text : public object
{
private:
	string content;
	TTF_Font* pFont;
	SDL_Color color;
public:
	text()
	{
		pFont = nullptr;
	}
	~text()
	{
		if (pFont)
		{
			TTF_CloseFont(pFont);
			pFont = nullptr;
		}
	}
	void setContent(string content)
	{
		this->content = content;
	}
	void setFont(int nSize)
	{
		pFont = TTF_OpenFont(file_name.c_str(), nSize);
		if (!pFont)
		{
			cout << "Cannot open font! Error : " << TTF_GetError() << endl;
			exit(1);
		}
	}
	void setColor(colorRGB color)
	{
		this->color.r = color.r;
		this->color.g = color.g;
		this->color.b = color.b;
		this->color.a = 255;
	}
	void setTexture()
	{
		SDL_Surface* pSurface = nullptr;
		pSurface = TTF_RenderText_Solid(pFont, content.c_str(), color);
		if (!pSurface)
		{
			cout << "Cannot render text solid! Error : " << TTF_GetError() << endl;
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
	}
	void appear()
	{
		if (pDstRect->y < 150)
		{
			pDstRect->y++;
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		}
		else
		{
			SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		}
	}
	virtual void render()
	{
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
};

int main()
{
	initSDL(g_pWindow, g_pRenderer);

	Mix_Chunk* adjhd = nullptr;
	adjhd = Mix_LoadWAV("Bird-sound.wav");
	int ret = Mix_PlayChannel(-1, adjhd, -1);

	openingGame();
	runGame();

	quitSDL(g_pWindow, g_pRenderer);
	return 0;
}

void initSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer)
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Cannot initialize SDL! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	if (TTF_Init() < 0)
	{
		cout << "Cannot initialize TTF! Error : " << TTF_GetError() << endl;
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
	TTF_Quit();
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

character* initCharacter(int dstX, int dstY, int dstW, int dstH)
{
	character* pCharacter = new character;

	pCharacter->setName("character.png");

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

text* initText(string file_name, colorRGB color, int nSize, int dstX, int dstY, int dstW, int dstH)
{
	text* pText = new text;
	pText->setName(file_name);
	pText->setColor(color);
	pText->setFont(nSize);
	pText->setDstRect(dstX, dstY, dstW, dstH);
	return pText;
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
			g_bJump = true;
		}
		if (g_pEvent->key.keysym.sym == SDLK_RETURN)
		{
			g_bRunGame = true;
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

void gameOver()
{
	g_bGameOver = true;

	text* pGameOver = initText("dpcomic.ttf", orange, 100, 300, 100, 350, 60);
	pGameOver->setContent("Game Over");
	pGameOver->setTexture();
	pGameOver->render();

	text* pPlayAgain = initText("dpcomic.ttf", orange, 100, 200, 200, 450, 65);
	pPlayAgain->setContent("Enter to play again");
	pPlayAgain->setTexture();
	pPlayAgain->render();

	while (!g_bRunGame)
	{
		SDL_RenderPresent(g_pRenderer);
		handleEvent();
	}
}

void handleImpact(character* pCharacter, obstacle* pObs_1, obstacle* pObs_2, obstacle* pObs_3, obstacle* pObs_4)
{
	if (isImpact(pCharacter, pObs_1))
	{
		g_bRunGame = false;
		gameOver();
	}
	if (isImpact(pCharacter, pObs_2))
	{
		g_bRunGame = false;
		gameOver();
	}
	if (isImpact(pCharacter, pObs_3))
	{
		g_bRunGame = false;
		gameOver();
	}
	if (isImpact(pCharacter, pObs_4))
	{
		g_bRunGame = false;
		gameOver();
	}
}

void openingGame()
{
	background* pBackground = initBackground(0, 0, BACKGROUND_IMG_WIDTH, BACKGROUND_IMG_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	pBackground->appear();
	text* pIntro_1 = initText("dpcomic.ttf", darkSeaGreen4, 100, 83, 100, 700, 50);
	pIntro_1->setContent("Dinosaur game but the Dinosaur runs backwards");
	pIntro_1->setTexture();
	pIntro_1->render();

	text* pIntro_2 = initText("dpcomic.ttf", orange, 100, 200, 200, 450, 65);
	pIntro_2->setContent("Press Enter to try it");
	pIntro_2->setTexture();
	pIntro_2->render();

	while (!g_bRunGame)
	{
		SDL_RenderPresent(g_pRenderer);
		handleEvent();
	}
}

void runGame()
{
	text* pGuide = initText("dpcomic.ttf", orange, 100, 230, -60, 435, 60);
	pGuide->setContent("Press Space to jump");
	pGuide->setTexture();

	character* pCharacter = initCharacter(140, 257, 70, 90);
	background* pBackground_1 = initBackground(0, 0, BACKGROUND_IMG_WIDTH, BACKGROUND_IMG_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	background* pBackground_2 = initBackground(0, 0, 0, BACKGROUND_IMG_HEIGHT, SCREEN_WIDTH, 0, 0, SCREEN_HEIGHT);
	obstacle* pFire_1 = initObstacle("fire.png", 0, 0, 0, FIRE_IMG_HEIGHT, SCREEN_WIDTH, 270, 0, FIRE_IMG_HEIGHT);
	obstacle* pFire_2 = initObstacle("fire.png", 0, 0, 0, FIRE_IMG_HEIGHT, SCREEN_WIDTH, 270, 0, FIRE_IMG_HEIGHT);
	obstacle* pUFO_1 = initObstacle("ufo.png", 0, 0, 0, UFO_IMG_HEIGT, SCREEN_WIDTH, 160, 0, UFO_IMG_HEIGT);
	obstacle* pUFO_2 = initObstacle("ufo.png", 0, 0, 0, UFO_IMG_HEIGT, SCREEN_WIDTH, 260, 0, UFO_IMG_HEIGT);

	text* pScore = initText("zorque.ttf", orange, 100, 680, 20, 70, 25);
	pScore->setContent("Score : ");
	pScore->setTexture();

	text* pScorePoint = initText("zorque.ttf", orange, 100, 750, 20, 55, 25);

	while (true)
	{
		g_bRunGame = false;
		
		pBackground_1->beInGuide();
		pCharacter->beInGuide();

		while (!g_bRunGame)
		{
			SDL_RenderClear(g_pRenderer);
			pBackground_1->appear();
			pCharacter->appear();
			pGuide->appear();
			SDL_RenderPresent(g_pRenderer);
			SDL_Delay(11);
			handleEvent();
		}
		
		pCharacter->beInGame();
		pBackground_2->beInGame();
		pFire_1->beInGame();
		pFire_2->beInGame();
		pUFO_1->beInGame();
		pUFO_2->beInGame();
		g_nTime = 0;
		g_nDistance = 0;
		g_nSpeedGame = 2;
		g_nScorePoint = 0;

		g_bMoveUp = true;
		g_bJump = false;
		g_bGameOver = false;

		while (!g_bGameOver)
		{
			SDL_RenderClear(g_pRenderer);

			pBackground_1->render();
			pBackground_2->render();
			pCharacter->render();
			pScore->render();

			ostringstream scorePoint;
			scorePoint << setw(4) << setfill('0') << g_nScorePoint;
			pScorePoint->setContent(scorePoint.str());
			pScorePoint->setTexture();
			pScorePoint->render();

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
			handleImpact(pCharacter, pFire_1, pFire_2, pUFO_1, pUFO_2);
		}
	}
}