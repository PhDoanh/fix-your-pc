#include "Game.hpp"
#include "Console.hpp"

string Game::title = "Fix Your PC";
int Game::screen_width = nullptr;
int Game::screen_height = nullptr;
float Game::fps = 60.0;
float Game::player_speed = nullptr;
float Game::enemy_speed = nullptr;
int Game::player_score = 0;
SDL_Event Game::event;
SDL_Window *Game::window = nullptr;
SDL_Renderer *Game::renderer = nullptr;

Game::Game()
{
	console.info("[ Initialization ]");

	// Base init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		console.error("SDL could not be initialized: " + SDL_GetError());
	else
		console.info("SDL initialized!");

	// Image init
	if (!IMG_Init(IMG_INIT_PNG))
		console.error("IMG could not be initialized: " + IMG_GetError());
	else
		console.info("IMG initialized!");

	// Font init
	if (TTF_Init() != 0)
		console.error("TTF could not be initialized: " + TTF_GetError());
	else
		console.info("TTF initialized!");

	// Sound init
	if (!Mix_Init(MIX_INIT_MP3) || Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096) != 0)
		console.error("MIX could not be initialized: " + Mix_GetError());
	else
		console.info("MIX initialized!");

	// Create window and renderer
	window = SDL_CreateWindow(title, 0, 0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		console.error("Window could not be created: " + SDL_GetError());
	else
		console.info("Window created!");

	SDL_GetWindowSize(window, &screen_width, &screen_height);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		console.error("Renderer could not be created: " + SDL_GetError());
	else
		console.info("Renderer created!");
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::start()
{
	Interface ui;
	ui.loadMenu();

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
		}

		SDL_RenderClear(Game::renderer);
	}
}