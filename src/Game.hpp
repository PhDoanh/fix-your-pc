#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "../inc/SDL_mixer.h"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
public:
	static bool running;
	static std::string title;
	static int win_w;
	static int win_h;
	static float fps;
	static float player_speed;
	static float enemy_speed;
	static int player_score;
	static SDL_Window *window;
	static SDL_Renderer *renderer;

	Game();
	~Game();

	// prepare game
	void prepareSDL2();
	void prepareMedia();

	// play game
	void start(); // core function
	void handleEvent();
	void updateScreen();

	// quit game
	void quitMedia();
	void quitSDl2();
};

#endif // GAME_HPP