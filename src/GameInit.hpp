#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"

#ifndef GAME_INIT_HPP
#define GAME_INIT_HPP

class GameInit
{
public:
	static SDL_Window *window;
	static SDL_Renderer *renderer;
	static int win_width;
	static int win_height;

	GameInit(Uint32 flags = 0);
	virtual ~GameInit();
};

#endif // GAME_INIT_HPP