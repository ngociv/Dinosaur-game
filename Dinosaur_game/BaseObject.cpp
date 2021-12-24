#include "CommonFuction.h"

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
	}
	if (pDstRect)
	{
		delete pDstRect;
	}
	if (pTexture)
	{
		SDL_DestroyTexture(pTexture);
	}
}

void baseObject::setName(const std::string& name)
{
	file_name = name;
}

void baseObject::setSrcRect(const SDL_Rect& srcRect)
{
	pSrcRect = new SDL_Rect;
	pSrcRect->x = srcRect.x;
	pSrcRect->y = srcRect.y;
	pSrcRect->w = srcRect.w;
	pSrcRect->h = srcRect.h;
}

void baseObject::setDstRect(const SDL_Rect& dstRect)
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

//get coordinate of a point on the object
//we set the (DstRect->x, DstRect->y) point of the object is root
//from root, we can get any point coordinate on the object by adding plusX and plusY
//for example: we will get the root if plusX = 0, plusY = 0;
point_2d baseObject::getCoordinate(int plusX, int plusY) {
	point_2d point;
	point.x = pDstRect->x + plusX;
	point.y = pDstRect->y + plusY;
	return point;
}

std::string baseObject::getName()
{
	return file_name;
}