#include <Windows.h>
#include <iostream>
#include <SDL.h>
#include <string>

#undef main

int main() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Something wrong! Detail : " << SDL_GetError();
		return -1;
	}

	SDL_Window* pWindow = nullptr;
	pWindow = SDL_CreateWindow("Dinosaur game by Ngociv", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 297, SDL_WINDOW_SHOWN);
	SDL_Delay(3000);

	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}