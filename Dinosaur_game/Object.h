#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL_ttf.h>

#include "BaseObject.h"

class character : public baseObject
{
public:
	void beInGuide();
	void reset();
	void appearInGuide();
	void jumpUp();
	void jumpDown();
	virtual void render();
};

class background : public baseObject
{
public:
	void beInGuide();
	void reset();
	void appearInGuide();
	virtual void render();
	void renderBackground_1();
	void renderBackground_2();
};

class obstacle : public baseObject
{
private:
	bool bAppear;
	int srcW;
public:
	obstacle();
	void reset();
	void appear(int srcW);
	virtual void render();
};

class text : public baseObject
{
private:
	std::string content;
	TTF_Font* pFont;
	SDL_Color color;
public:
	text();
	~text();
	void setContent(std::string content);
	void setFont(const int& nSize);
	void setColor(const colorRGB& color);
	void setTexture();
	void appearInGuide();
	virtual void render();
};


#endif // !OBJECT_H_