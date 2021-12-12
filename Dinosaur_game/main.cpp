#include <Windows.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

using namespace std;

#undef main

SDL_Surface* LoadImage(string file_name) {
	SDL_Surface* pLoad = nullptr;
	SDL_Surface* pOptimize = nullptr;

	pLoad = IMG_Load(file_name.c_str());
	if (pLoad) {
		pOptimize = SDL_ConvertSurfaceFormat(pLoad, SDL_PixelFormatEnum(), 0);
		SDL_FreeSurface(pLoad);
	}

	return pOptimize;
}

int main() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Something wrong! Detail : " << SDL_GetError();
		return -1;
	}

	SDL_Window* pWindow = nullptr;
	SDL_Surface* pScreen = nullptr;
	SDL_Surface* pBackgroud = nullptr;
	SDL_Renderer* pRenderer = nullptr;

	pWindow = SDL_CreateWindow("Dinosaur game by Ngociv", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 297, SDL_WINDOW_SHOWN);
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	pScreen = SDL_GetWindowSurface(pWindow);
	
	pBackgroud = LoadImage("background.png");

	SDL_BlitSurface(pBackgroud, NULL, pScreen, NULL);

	SDL_RenderPresent(pRenderer);

	SDL_Delay(3000);
	SDL_FreeSurface(pBackgroud);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}