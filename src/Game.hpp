#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <queue>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "../inc/SDL_mixer.h"
#include "../inc/SDL_net.h"
#include "util.hpp"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
public:
	std::string title;
	static float fps;
	static int win_w;
	static int win_h;
	static bool running;
	static const Uint32 spawn_time;
	static Uint32 last_spawn_time;
	static std::stringstream level;
	static SDL_Window *window;
	static SDL_Renderer *renderer;

	Game(std::string title) : title(title) { info("Game constructor called."); }
	~Game() { info("Game destructor called."); }

	void initSDL2();
	void loadMedia();

	void handleEvent();
	void updateScreen();

	void quitMedia();
	void quitSDL2();
};

extern std::queue<std::string> levels;

#endif // GAME_HPP