#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "../inc/SDL_mixer.h"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"

#ifndef GAME_HPP
#define GAME_HPP

#define NUM_BG 10
#define MAX_NUM_ENEMY 50

class Game
{
private:
	void initSDL2();
	void loadMedia();

	void handleEvent();
	void updateScreen();

	void quitMedia();
	void quitSDL2();

	Screen screen;
	Sound sound;
	Event event;

public:
	static float fps;
	static int win_w;
	static int win_h;
	static bool running;
	static std::string title;
	static SDL_Window *window;
	static SDL_Renderer *renderer;

	Game();
	~Game();

	void start();
};

#endif // GAME_HPP