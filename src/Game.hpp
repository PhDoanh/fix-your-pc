#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "../inc/SDL_mixer.h"

#ifndef GAME_INIT_HPP
#define GAME_INIT_HPP

class Game
{
public:
	static string title;
	static int screen_width;
	static int screen_height;
	static float fps;
	static float player_speed;
	static float enemy_speed;
	static int player_score;
	static SDL_Event event;
	static SDL_Window *window;
	static SDL_Renderer *renderer;

	Game();
	~Game();

	void start();
};

#endif // GAME_INIT_HPP