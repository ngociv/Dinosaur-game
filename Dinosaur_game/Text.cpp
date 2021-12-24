#include <iostream>

#include "Object.h"
#include "Constant.h"

using namespace std;

extern SDL_Renderer* g_pRenderer;

text::text()
{
	pFont = nullptr;
	color = { 0,0,0 };
}

text::~text()
{
	if (pFont)
	{
		TTF_CloseFont(pFont);
	}
}

void text::setContent(std::string content)
{
	this->content = content;
}

void text::setFont(const int& nSize)
{
	pFont = TTF_OpenFont(file_name.c_str(), nSize);
	if (!pFont)
	{
		cout << "Cannot open font! Error : " << TTF_GetError() << endl;
		exit(1);
	}
}

void text::setColor(const colorRGB& color)
{
	this->color.r = color.r;
	this->color.g = color.g;
	this->color.b = color.b;
	this->color.a = 255;
}

//create texture of text to get ready to render
void text::setTexture()
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
}

void text::appearInGuide()
{
	//the guide text will run down from top
	if (pDstRect->y < GUIDE_DST_RECT.y)
	{
		pDstRect->y++;
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
	//and it stops when reach the middle of window 
	else
	{
		SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
	}
}

void text::render()
{
	SDL_RenderCopy(g_pRenderer, pTexture, pSrcRect, pDstRect);
}