#include "Constant.h"
#include "GlobalVariable.h"

obstacle::obstacle()
{
	bAppear = false;
	srcW = 0;
}

void obstacle::reset()
{
	bAppear = false;
	pSrcRect->w = 0;
	pDstRect->x = SCREEN_WIDTH;
	pDstRect->w = 0;
}

void obstacle::appear(int srcW) {
	bAppear = true;
	this->srcW = srcW;
}

void obstacle::render()
{
	//if obstacle is not allowed to appearInGuide
	if (!bAppear) {
		return;
	}

	//if it is
	if (pSrcRect->w < srcW)
	{
		pSrcRect->w += g_nSpeedGame;
	}
	pDstRect->x -= g_nSpeedGame;
	if (pDstRect->w < srcW)
	{
		pDstRect->w += g_nSpeedGame;
	}
	SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);

	//reset when obstacle is out of screen
	if (pDstRect->x < -srcW)
	{
		bAppear = false;
		pSrcRect->w = 0;
		pDstRect->x = SCREEN_WIDTH;
		pDstRect->w = 0;
	}
}