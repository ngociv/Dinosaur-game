#include "Object.h"
#include "Constant.h"


extern int g_nScore;
extern unsigned long g_nTime;
extern unsigned long g_nDistance;
extern unsigned long g_nGameSpeed;

extern SDL_Renderer* g_pRenderer;

//setup to get ready to appear in guide part
void background::beInGuide()
{
	pSrcRect->x = 0;
	pSrcRect->w = BACKGROUND_WIDTH;
	pDstRect->w = SCREEN_WIDTH;
}

//reset to get ready to start running game
void background::reset()
{
	pSrcRect->w = 0;
	pDstRect->w = 0;
	pDstRect->x = SCREEN_WIDTH;
}

void background::appearInGuide()
{
	SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
}

void background::render()
{
	//this project uses 2 background, called background_1 & background_2
	//Dst_Rect_x of background_1 is always 0

	//handle background_1
	if (pDstRect->x == 0)
	{
		renderBackground_1();

		g_nTime++;
		g_nDistance += g_nGameSpeed;
		if ((g_nDistance % 80) < g_nGameSpeed) {
			g_nScore++;
		}
	}
	//handle background_2
	else
	{
		renderBackground_2();
	}
}

void background::renderBackground_1()
{
	SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	pSrcRect->x += g_nGameSpeed;
	pSrcRect->w -= g_nGameSpeed;
	pDstRect->w -= g_nGameSpeed;

	//reset to re-use
	if (pDstRect->w <= 0)
	{
		pSrcRect->x = -pDstRect->w;
		pSrcRect->w = BACKGROUND_WIDTH - pSrcRect->x;
		pDstRect->w = pSrcRect->w;
	}
}

void background::renderBackground_2()
{
	SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	pSrcRect->w += g_nGameSpeed;
	pDstRect->w += g_nGameSpeed;
	pDstRect->x -= g_nGameSpeed;

	//reset to re-use
	if (pDstRect->x <= 0)
	{
		pSrcRect->w = -pDstRect->x;
		pDstRect->w = pSrcRect->w;
		pDstRect->x = SCREEN_WIDTH - pDstRect->w;
	}
}