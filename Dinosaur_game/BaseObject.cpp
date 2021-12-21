#include "CommonFuction.h"
#include "GlobalVariable.h"

baseObject::baseObject()
{
	pSrcRect = nullptr;
	pDstRect = nullptr;
	pTexture = nullptr;
}

baseObject::~baseObject()
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

void baseObject::setName(std::string name)
{
	file_name = name;
}

void baseObject::setSrcRect(SDL_Rect srcRect)
{
	pSrcRect = new SDL_Rect;
	pSrcRect->x = srcRect.x;
	pSrcRect->y = srcRect.y;
	pSrcRect->w = srcRect.w;
	pSrcRect->h = srcRect.h;
}

void baseObject::setDstRect(SDL_Rect dstRect)
{
	pDstRect = new SDL_Rect;
	pDstRect->x = dstRect.x;
	pDstRect->y = dstRect.y;
	pDstRect->w = dstRect.w;
	pDstRect->h = dstRect.h;
}

void baseObject::setTexture()
{
	pTexture = loadTexture(file_name);
}

point_2d baseObject::getDstRectXY(int plusX, int plusY) {
	point_2d point;
	point.x = pDstRect->x + plusX;
	point.y = pDstRect->y + plusY;
	return point;
}

std::string baseObject::getName()
{
	return file_name;
}