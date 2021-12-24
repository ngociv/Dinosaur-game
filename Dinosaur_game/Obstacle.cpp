#include "Object.h"
#include "Constant.h"


extern unsigned long g_nGameSpeed;

extern SDL_Renderer* g_pRenderer;

obstacle::obstacle()
{
	bAppear = false;
	srcW = 0;
}

//reset to get ready to start running game
void obstacle::reset()
{
	bAppear = false;
	pSrcRect->w = 0;
	pDstRect->x = SCREEN_WIDTH;
	pDstRect->w = 0;
}

//allow to appear
void obstacle::appear(int srcW) {
	bAppear = true;
	this->srcW = srcW;
}

void obstacle::render()
{
	//if obstacle is not allowed to appear
	if (!bAppear) {
		return;
	}

	//if it is
	if (pSrcRect->w < srcW)
	{
		pSrcRect->w += g_nGameSpeed;
	}
	pDstRect->x -= g_nGameSpeed;
	if (pDstRect->w < srcW)
	{
		pDstRect->w += g_nGameSpeed;
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