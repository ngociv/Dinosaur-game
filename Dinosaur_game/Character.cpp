#include "Constant.h"
#include "GlobalVariable.h"

void character::beInGuide()
{
	pDstRect->y = -CHARACTER_DST_RECT.h;
}

void character::reset()
{
	pDstRect->y = CHARACTER_DST_RECT.y;
}

void character::appearInGuide()
{
	if (pDstRect->y < CHARACTER_DST_RECT.y)
	{
		pDstRect->y++;
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
	else
	{
		g_bRunGame = true;
	}
}

void character::render()
{
	if (!g_bJump) {
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
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
		if (pDstRect->y >= CHARACTER_DST_RECT.y)
		{
			g_bMoveUp = true;
			g_bJump = false;
		}
	}
}