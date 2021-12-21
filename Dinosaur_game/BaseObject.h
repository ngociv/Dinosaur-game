#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include <string>

#include <SDL.h>

#include "OwnStruct.h"


class baseObject
{
protected:
	std::string file_name;
	SDL_Texture* pTexture;
	SDL_Rect* pSrcRect;
	SDL_Rect* pDstRect;

public:
	baseObject();

	~baseObject();

	void setName(std::string name);

	void setSrcRect(SDL_Rect srcRect);

	void setDstRect(SDL_Rect dstRect);

	void setTexture();

	point_2d getDstRectXY(int plusX, int plusY);

	std::string getName();

	virtual void render() = 0;
};

#endif // !BASE_OBJECT_H_