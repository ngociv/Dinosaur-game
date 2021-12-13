#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

#undef main

using namespace std;

const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 297;
const string WINDOW_TITLE = "Dinosaur game by Ngociv";

SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;

void initSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void quitSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer);
void loadImage(string file_name, SDL_Renderer* pRenderer);
SDL_Texture* loadTexture(string file_name, SDL_Renderer* pRenderer);

int main()
{
	initSDL(g_pWindow, g_pRenderer);

	loadImage("background.png", g_pRenderer);
	SDL_Delay(7000);

	quitSDL(g_pWindow, g_pRenderer);
	return 0;
}

void initSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Cannot initialize SDL! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	pWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
	if (!pWindow)
	{
		cout << "Cannot create window! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	if (!pRenderer)
	{
		cout << "Cannot create renderer! Error : " << SDL_GetError() << endl;
		exit(1);
	}
	SDL_RenderSetLogicalSize(pRenderer, SCR_WIDTH, SCR_HEIGHT);
}

void quitSDL(SDL_Window*& pWindow, SDL_Renderer*& pRenderer)
{
	SDL_DestroyRenderer(pRenderer);
	pRenderer = nullptr;
	SDL_DestroyWindow(pWindow);
	pWindow = nullptr;
	SDL_Quit();
	return;
}

SDL_Texture* loadTexture(string file_name, SDL_Renderer* pRenderer)
{
	SDL_Surface* pLoad = nullptr;
	SDL_Texture* pTexture = nullptr;

	pLoad = IMG_Load(file_name.c_str());
	if (!pLoad)
	{
		cout << "Cannot load image! Error : " << IMG_GetError() << endl;
		exit(2);
	}
	pTexture = SDL_CreateTextureFromSurface(pRenderer, pLoad);
	if (!pTexture)
	{
		cout << "Cannot create texture from surface! Error : " << SDL_GetError() << endl;
		exit(2);
	}
	SDL_FreeSurface(pLoad);
	pLoad = nullptr;

	return pTexture;
}

void loadImage(string file_name, SDL_Renderer* pRenderer)
{
	SDL_Texture* pImage = loadTexture(file_name, pRenderer);
	SDL_RenderCopy(pRenderer, pImage, NULL, NULL);
	SDL_DestroyTexture(pImage);
	pImage = nullptr;
	SDL_RenderPresent(pRenderer);
	return;
}