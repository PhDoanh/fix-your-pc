#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "../inc/SDL_mixer.h"
#include "../inc/SDL_net.h"
#include "util.hpp"

#ifndef GAME_HPP
#define GAME_HPP

enum game_states
{
	start,
	play,
	pause,
	over
};

class Game
{
public:
	std::string title;
	static float fps;
	static int state;
	static int win_w;
	static int win_h;
	static bool running;
	static float deltaTime;
	static std::fstream data;
	static SDL_Window *window;
	static SDL_Renderer *renderer;

	Game(const std::string &title) : title(title) { info("Game constructor called.\n"); }
	~Game() { info("Game destructor called.\n"); }

	void initSDL2();
	void loadMedia();

	void handleEvent();
	void updateScreen();

	void quitMedia();
	void quitSDL2();
};

#endif // GAME_HPP