#include "GameInit.hpp"
#include "Console.hpp"

SDL_Window *GameInit::window = nullptr;
SDL_Renderer *GameInit::renderer = nullptr;
int GameInit::win_width = 0;
int GameInit::win_height = 0;

GameInit::GameInit(Uint32 flags)
{
	// Base init
	if (SDL_Init(flags) != 0)
		console.error();
	window = SDL_CreateWindow("Fix Your PC", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == nullptr)
		console.error();
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
		console.error();

	SDL_GetWindowSize(window, &win_width, &win_height);

	// Image init

	// Font init
	if (TTF_Init() != 0)
		console.error();
}

GameInit::~GameInit()
{
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
