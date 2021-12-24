#include <sstream>
#include <iomanip>

#include "Constant.h"
#include "CommonFuction.h"
#include "GlobalVariable.h"


#undef main

using namespace std;

void playBackgroundSound();
void introduceGame();
void runGame();

void initGame();
void guidePlay();
void reset();
void render_1();
void render_2();
void handleEvent();
void handleImpact();
bool isImpact(obstacle* pObs);
void setRectangle(obstacle* pObs);
void setPolygon();
bool isInside(const point_2d& M);
void gameOver();

int main()
{
	initProgram();

	playBackgroundSound();
	introduceGame();
	runGame();

	quitProgram();
	return 0;
}

void playBackgroundSound()
{
	g_pBackgroundSound = Mix_LoadWAV("Bird-sound.wav");
	Mix_PlayChannel(CHANNEL, g_pBackgroundSound, BACKGROUND_SOUND_LOOPS);
}

//some introduction about game, wait for player to press Enter
void introduceGame()
{
	background* pBackground = initBackground(BACKGROUND_1_SRC_RECT, BACKGROUND_1_DST_RECT);
	pBackground->appearInGuide();

	text* pIntro_1 = initText("dpcomic.ttf", lightYellow4, INTRO_SIZE, INTRO_DST_RECT);
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
	delete(pBackground);
	delete(pIntro_1);
	delete(pIntro_2);
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

			if ((g_nTime % 1500) == 0)
			{
				g_nGameSpeed++;
			}

			SDL_RenderPresent(g_pRenderer);
			SDL_Delay(11);
			handleEvent();
			handleImpact();
		}
	}
}

//initialize background, character, obstacle, text, and jump sound
void initGame()
{
	g_pCharacter = initCharacter(CHARACTER_DST_RECT);
	g_pBackground_1 = initBackground(BACKGROUND_1_SRC_RECT, BACKGROUND_1_DST_RECT);
	g_pBackground_2 = initBackground(BACKGROUND_2_SRC_RECT, BACKGROUND_2_DST_RECT);
	g_pFire_1 = initObstacle("fire.png", FIRE_SRC_RECT, FIRE_DST_RECT);
	g_pFire_2 = initObstacle("fire.png", FIRE_SRC_RECT, FIRE_DST_RECT);
	g_pUFO_1 = initObstacle("ufo.png", UFO_SRC_RECT, HIGH_UFO_DST_RECT);
	g_pUFO_2 = initObstacle("ufo.png", UFO_SRC_RECT, LOW_UFO_DST_RECT);

	g_pScore = initText("dpcomic.ttf", orange, SCORE_TEXT_SIZE, SCORE_TEXT_DST_RECT);
	g_pScore->setContent("Score : ");
	g_pScore->setTexture();

	g_pScorePoint = initText("zorque.ttf", orange, SCORE_POINT_SIZE, SCORE_POINT_DST_RECT);

	g_pJumpSound = Mix_LoadWAV("jump-sound.wav");
}

//some guide befor run game 
void guidePlay()
{
	g_bRunGame = false;

	text* pGuide = initText("dpcomic.ttf", orange, GUIDE_SIZE, GUIDE_APPEAR_DST_RECT);
	pGuide->setContent("Press Space to jump");
	pGuide->setTexture();

	g_pBackground_1->beInGuide();
	g_pCharacter->beInGuide();

	while (!g_bRunGame)
	{
		SDL_RenderClear(g_pRenderer);
		g_pBackground_1->appearInGuide();
		g_pCharacter->appearInGuide();
		pGuide->appearInGuide();

		SDL_RenderPresent(g_pRenderer);
		SDL_Delay(11);
		handleEvent();
	}
	delete(pGuide);
}

//reset to get ready to start running game
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
	g_nGameSpeed = 2;
	g_nScore = 0;

	g_bMoveUp = true;
	g_bJump = false;
	g_bGameOver = false;
}

//render background, character and score
void render_1()
{
	g_pBackground_1->render();
	g_pBackground_2->render();
	g_pCharacter->render();
	g_pScore->render();

	//convert score form number to text
	ostringstream scorePoint;
	scorePoint << setw(4) << setfill('0') << g_nScore;
	g_pScorePoint->setContent(scorePoint.str());
	g_pScorePoint->setTexture();
	g_pScorePoint->render();
}

//render obstacle
void render_2()
{
	//first, we need to decide which obstacle will appear
	if (((g_nTime - 2) % 200) == 0)
	{
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
		//player press Space, character jump and jump sound is turned on
		if (g_pEvent->key.keysym.sym == SDLK_SPACE)
		{
			g_bJump = true;
			Mix_PlayChannel(CHANNEL, g_pJumpSound, JUMP_SOUND_LOOPS);
		}
		//player press Enter
		if (g_pEvent->key.keysym.sym == SDLK_RETURN)
		{
			g_bRunGame = true;
		}
	}
}

void handleImpact()
{
	if (isImpact(g_pUFO_1))
	{
		g_bRunGame = false;
		gameOver();
	}
	if (isImpact(g_pUFO_2))
	{
		g_bRunGame = false;
		gameOver();
	}
	if (isImpact(g_pFire_1))
	{
		g_bRunGame = false;
		gameOver();
	}
	if (isImpact(g_pFire_2))
	{
		g_bRunGame = false;
		gameOver();
	}
}

//create a rectangle that represent obstacle
//create a polygon with 5 vertices that represent character with left head, right head, left foot, right foot and tail
//if the rectangle and the polygon intersect, return true
bool isImpact(obstacle* pObs)
{
	setRectangle(pObs);
	setPolygon();

	if (isInside(g_pPolygon->leftHead))
	{
		return true;
	}
	if (isInside(g_pPolygon->rightHead))
	{
		return true;
	}
	if (isInside(g_pPolygon->leftFoot))
	{
		return true;
	}
	if (isInside(g_pPolygon->rightFoot))
	{
		return true;
	}
	if (isInside(g_pPolygon->tail))
	{
		return true;
	}
	return false;
}

void setRectangle(obstacle* pObs)
{
	static int plusX;
	//if obstacle is fire
	if (pObs->getName() == "fire.png")
	{
		plusX = 20;
	}
	//if obstacle is UFO
	else
	{
		plusX = 0;
	}
	
	g_pRectangle->A = pObs->getCoordinate(plusX, 5);
	g_pRectangle->B = pObs->getCoordinate(65, 5);
	g_pRectangle->C = pObs->getCoordinate(66, 45);
	g_pRectangle->D = pObs->getCoordinate(plusX, 45);
}

void setPolygon()
{
	g_pPolygon->leftHead = g_pCharacter->getCoordinate(0, 0);
	g_pPolygon->rightHead = g_pCharacter->getCoordinate(45, 0);
	g_pPolygon->leftFoot = g_pCharacter->getCoordinate(5, 85);
	g_pPolygon->rightFoot = g_pCharacter->getCoordinate(45, 88);
	g_pPolygon->tail = g_pCharacter->getCoordinate(70, 45);
}

//check if a point is inside the rectangle (of obstacle) or not
bool isInside(const point_2d& M)
{
	if ((g_pRectangle->A.x < M.x) && (M.x < g_pRectangle->B.x))
	{
		if ((g_pRectangle->A.y < M.y) && (M.y < g_pRectangle->C.y))
		{
			return true;
		}
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
	delete(pGameOver);
	delete(pPlayAgain);
}