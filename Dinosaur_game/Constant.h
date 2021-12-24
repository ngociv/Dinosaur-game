#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <string>

#include <SDL.h>


//*********************
//		SCREEN
//*********************
const int SCREEN_WIDTH = 841;
const int SCREEN_HEIGHT = 421;


//*********************
//		BACKGROUND
//*********************
const std::string BACKGROUND_NAME = "background.png";

const int BACKGROUND_WIDTH = 841;
const int BACKGROUND_HEIGHT = 421;

const SDL_Rect BACKGROUND_1_SRC_RECT = { 0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT };
const SDL_Rect BACKGROUND_1_DST_RECT = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

const SDL_Rect BACKGROUND_2_SRC_RECT = { 0,0,0,BACKGROUND_HEIGHT };
const SDL_Rect BACKGROUND_2_DST_RECT = { SCREEN_WIDTH,0,0,SCREEN_HEIGHT };


//*********************
//		CHARACTER
//*********************
const std::string CHARACTER_NAME = "character.png";

const SDL_Rect CHARACTER_DST_RECT = { 140,257,70,90 };

const int MINIMUM_DST_RECT_Y_WHEN_JUMPING = 100;

const int SLOW_JUMP_SPEED = 2;
const int FAST_JUMP_SPEED = 3;

const int DST_RECT_Y_WHERE_SPEED_CHANGED = 179;


//*********************
//		OBSTACLE
//*********************
// 
//   FIRE_NAME = "fire.png";
//   UFO_NAME = "ufo.png";
const int FIRE_WIDTH = 80;
const int FIRE_HEIGHT = 80;

const int UFO_WIDTH = 80;
const int UFO_HEIGHT = 69;

const SDL_Rect FIRE_SRC_RECT = { 0,0,0,FIRE_HEIGHT };
const SDL_Rect FIRE_DST_RECT = { SCREEN_WIDTH,270,0,FIRE_HEIGHT };

const SDL_Rect UFO_SRC_RECT = { 0,0,0,UFO_HEIGHT };
const SDL_Rect LOW_UFO_DST_RECT = { SCREEN_WIDTH,260,0,UFO_HEIGHT };
const SDL_Rect HIGH_UFO_DST_RECT = { SCREEN_WIDTH,160,0,UFO_HEIGHT };


//*********************
//	    SDL MIXER
//*********************
const int CHANNEL = -1;
const int CHANNELS = 1;
const int CHUNKSIZE = 4096;
const int JUMP_SOUND_LOOPS = 0;
const int BACKGROUND_SOUND_LOOPS = -1;


//*********************
//		  TEXT
//*********************
const std::string WINDOW_TITLE = "Dinosaur game by Ngociv";

// "Dinosaur game but the Dinosaur runs backwards";
const SDL_Rect INTRO_DST_RECT = { 83,100,700,50 };
const int INTRO_SIZE = 100;

// "Press Space to jump";
const SDL_Rect GUIDE_DST_RECT = { 230,150,435,60 };
const SDL_Rect GUIDE_APPEAR_DST_RECT = { 230,-60,435,60 };
const int GUIDE_SIZE = 100;

// "Press Enter to try it";
const SDL_Rect ENTER_1_DST_RECT = { 200,200,450,65 };
const int ENTER_1_SIZE = 100;

// "Enter to play again";
const SDL_Rect ENTER_2_DST_RECT = { 200,200,450,65 };
const int ENTER_2_SIZE = 100;

// "Game over";
const SDL_Rect GAME_OVER_DST_RECT = { 230,90,400,80 };
const int GAME_OVER_SIZE = 100;

// "Score";
const SDL_Rect SCORE_TEXT_DST_RECT = { 680,20,80,25 };
const int SCORE_TEXT_SIZE = 100;

const SDL_Rect SCORE_POINT_DST_RECT = { 758,20,55,25 };
const int SCORE_POINT_SIZE = 100;


#endif // !CONSTANT_H_

