#include "Object.h"
#include "Constant.h"


extern bool g_bJump;
extern bool g_bMoveUp;
extern bool g_bRunGame;

extern SDL_Renderer* g_pRenderer;


//setup to get ready to appear in guide part
void character::beInGuide()
{
	pDstRect->y = -CHARACTER_DST_RECT.h;
}

//reset to get ready to start running game
void character::reset()
{
	pDstRect->y = CHARACTER_DST_RECT.y;
}

void character::appearInGuide()
{
	//the character will drop down until he hit ground
	if (pDstRect->y < CHARACTER_DST_RECT.y)
	{
		pDstRect->y++;
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
	//when he hit ground, start running game
	else
	{
		g_bRunGame = true;
	}
}

void character::render()
{
	//if player doesn't press Space
	if (!g_bJump) {
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
	//if he do
	else {
		if (g_bMoveUp) {
			jumpUp();
		}
		else {
			jumpDown();
		}
	}
}

void character::jumpUp()
{
	//don't forget that Dst_Rect_y == 0 is top of window
	if (pDstRect->y >= MINIMUM_DST_RECT_Y_WHEN_JUMPING) {
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		if (pDstRect->y >= DST_RECT_Y_WHERE_SPEED_CHANGED)
		{
			pDstRect->y -= FAST_JUMP_SPEED;
		}
		else
		{
			pDstRect->y -= SLOW_JUMP_SPEED;
		}
		//when he hit the max height, he have to jump down
		if (pDstRect->y < MINIMUM_DST_RECT_Y_WHEN_JUMPING)
		{
			g_bMoveUp = false;
		}
	}
}

void character::jumpDown()
{
	if (pDstRect->y < CHARACTER_DST_RECT.y)
	{
		if (pDstRect->y <= DST_RECT_Y_WHERE_SPEED_CHANGED)
		{
			pDstRect->y += SLOW_JUMP_SPEED;
		}
		else
		{
			pDstRect->y += FAST_JUMP_SPEED;
		}
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
		//when he finished his jump, we reset
		if (pDstRect->y >= CHARACTER_DST_RECT.y)
		{
			g_bMoveUp = true;
			g_bJump = false;
		}
	}
}