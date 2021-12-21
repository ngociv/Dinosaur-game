#include <sstream>
#include <iomanip>

#include "Constant.h"
#include "GlobalVariable.h"
#include "CommonFuction.h"
#include "Object.h"


#undef main

using namespace std;

void introduceGame();
void runGame();
void initGame();
void reset();
void render_1();
void render_2();
void guidePlay();

int main()
{
	initProgram();
	/*
	Mix_Chunk* adjhd = nullptr;
	adjhd = Mix_LoadWAV("Bird-sound.wav");
	int ret = Mix_PlayChannel(-1, adjhd, -1);
	*/
	introduceGame();
	runGame();

	quitProgram();
	return 0;
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

	text* pGameOver = initText("dpcomic.ttf", orange, GAME_OVER_SIZE, GAME_OVER_DST_RECT);
	pGameOver->setContent("Game Over");
	pGameOver->setTexture();
	pGameOver->render();

	text* pPlayAgain = initText("dpcomic.ttf", orange, ENTER_2_SIZE, ENTER_2_DST_RECT);
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

void introduceGame()
{
	background* pBackground = initBackground(BACKGROUND_1_SRC_RECT, BACKGROUND_1_DST_RECT);
	pBackground->appearInGuide();

	text* pIntro_1 = initText("dpcomic.ttf", darkSeaGreen4, INTRO_SIZE, INTRO_DST_RECT);
	pIntro_1->setContent("Dinosaur game but the Dinosaur runs backwards");
	pIntro_1->setTexture();
	pIntro_1->render();

	text* pIntro_2 = initText("dpcomic.ttf", orange, ENTER_1_SIZE, ENTER_1_DST_RECT);
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
	initGame();

	while (true)
	{
		guidePlay();
		reset();

		while (!g_bGameOver)
		{
			SDL_RenderClear(g_pRenderer);

			render_1();
			render_2();

			

			if ((g_nTime % 1500) == 0) {
				g_nSpeedGame++;
			}

			SDL_RenderPresent(g_pRenderer);
			SDL_Delay(11);
			handleEvent();
			handleImpact(g_pCharacter, g_pFire_1, g_pFire_2, g_pUFO_1, g_pUFO_2);
		}
	}
}

void initGame()
{
	g_pGuide = initText("dpcomic.ttf", orange, GUIDE_SIZE, GUIDE_APPEAR_DST_RECT);
	g_pGuide->setContent("Press Space to jump");
	g_pGuide->setTexture();

	g_pCharacter = initCharacter(CHARACTER_DST_RECT);
	g_pBackground_1 = initBackground(BACKGROUND_1_SRC_RECT, BACKGROUND_1_DST_RECT);
	g_pBackground_2 = initBackground(BACKGROUND_2_SRC_RECT, BACKGROUND_2_DST_RECT);
	g_pFire_1 = initObstacle("fire.png", FIRE_SRC_RECT, FIRE_DST_RECT);
	g_pFire_2 = initObstacle("fire.png", FIRE_SRC_RECT, FIRE_DST_RECT);
	g_pUFO_1 = initObstacle("ufo.png", UFO_SRC_RECT, HIGH_UFO_DST_RECT);
	g_pUFO_2 = initObstacle("ufo.png", UFO_SRC_RECT, LOW_UFO_DST_RECT);

	g_pScore = initText("zorque.ttf", orange, SCORE_TEXT_SIZE, SCORE_TEXT_DST_RECT);
	g_pScore->setContent("Score : ");
	g_pScore->setTexture();

	g_pScorePoint = initText("zorque.ttf", orange, SCORE_POINT_SIZE, SCORE_POINT_DST_RECT);
}

void reset()
{
	g_pCharacter->reset();
	g_pBackground_2->reset();
	g_pFire_1->reset();
	g_pFire_2->reset();
	g_pUFO_1->reset();
	g_pUFO_2->reset();

	g_nTime = 0;
	g_nDistance = 0;
	g_nSpeedGame = 2;
	g_nScore = 0;

	g_bMoveUp = true;
	g_bJump = false;
	g_bGameOver = false;
}

void guidePlay()
{
	g_bRunGame = false;

	g_pBackground_1->beInGuide();
	g_pCharacter->beInGuide();

	while (!g_bRunGame)
	{
		SDL_RenderClear(g_pRenderer);
		g_pBackground_1->appearInGuide();
		g_pCharacter->appearInGuide();
		g_pGuide->appear();
		SDL_RenderPresent(g_pRenderer);
		SDL_Delay(11);
		handleEvent();
	}
}

void render_1()
{
	g_pBackground_1->render();
	g_pBackground_2->render();
	g_pCharacter->render();
	g_pScore->render();

	ostringstream scorePoint;
	scorePoint << setw(4) << setfill('0') << g_nScore;
	g_pScorePoint->setContent(scorePoint.str());
	g_pScorePoint->setTexture();
	g_pScorePoint->render();
}

void render_2()
{
	if (((g_nTime - 2) % 200) == 0) {
		srand(time(NULL));
		int nRandom = rand() % 4;
		switch (nRandom)
		{
		case 0: g_pFire_1->appear(FIRE_WIDTH); break;
		case 1: g_pFire_2->appear(FIRE_WIDTH); break;
		case 2: g_pUFO_1->appear(UFO_WIDTH); break;
		case 3: g_pUFO_2->appear(UFO_WIDTH); break;
		}
	}
	g_pFire_1->render();
	g_pFire_2->render();
	g_pUFO_1->render();
	g_pUFO_2->render();
}