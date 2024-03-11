#include "Console.hpp"
#include "Enemy.hpp"
#include "Event.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Utility.hpp"

bool Game::running = true;
std::string Game::title = "Fix Your PC";
int Game::win_w = 0;
int Game::win_h = 0;
float Game::fps = 60.0;
float Game::enemy_speed = 0;
SDL_Window *Game::window = nullptr;
SDL_Renderer *Game::renderer = nullptr;

Game::Game()
{
	prepareSDL2();
	prepareMedia();
}

Game::~Game()
{
	quitMedia();
	quitSDL2();
}

void Game::start()
{
	float frame_start, frame_time, frame_delay = 1000.0 / Game::fps;
	while (running)
	{
		frame_start = float(SDL_GetTicks());

		handleEvent();	// ux
		updateScreen(); // ui

		frame_time = float(SDL_GetTicks()) - frame_start;
		if (frame_delay > frame_time)
			SDL_Delay(frame_delay - frame_time);
	}
}

void Game::handleEvent()
{
	while (SDL_PollEvent(&Event::e))
	{
		if (Event::e.type == SDL_KEYDOWN && Event::e.key.keysym.sym == SDLK_ESCAPE)
		{
			running = false;
		}
		event.handleMouse();
		event.handleKeyboard();
	}
}

void Game::updateScreen()
{
	SDL_RenderClear(Game::renderer);

	Sprite bg = *sprites["background"];
	Sprite arrow = *sprites["arrow"];

	screen.drawSprite(bg, Vector(), Vector(win_w, win_h), 1, 1, false);
	screen.drawSprite(arrow, Vector(player.x, player.y), Vector(arrow.real_size.x, arrow.real_size.y), 1, 1, false);

	SDL_RenderPresent(Game::renderer);
}

void Game::prepareSDL2()
{
	console.log("[ SDL2 preparation ]");

	// Base init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		console.error("SDL could not be initialized!");
	else
		console.log("SDL initialized!");

	// Image init
	if (!IMG_Init(IMG_INIT_PNG))
		console.error("IMG could not be initialized!");
	else
		console.log("IMG initialized!");

	// Font init
	if (TTF_Init() != 0)
		console.error("TTF could not be initialized!");
	else
		console.log("TTF initialized!");

	// Sound init
	if (!Mix_Init(MIX_INIT_MP3) || Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096) != 0)
		console.error("MIX could not be initialized!");
	else
		console.log("MIX initialized!");

	// Create window and renderer
	window = SDL_CreateWindow(title.c_str(), 0, 0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		console.error("Window could not be created!");
	else
		console.log("Window created!");

	SDL_GetWindowSize(window, &win_w, &win_h);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		console.error("Renderer could not be created!");
	else
		console.log("Renderer created!");

	// some fixed set up
	SDL_ShowCursor(SDL_DISABLE);
}

void Game::prepareMedia()
{
	console.log("[ Media game preparation ]");

	// load ui
	screen.loadSprite("background", "res/backgrounds/bg1_blur.png", Vector(3840, 2400));
	screen.loadSprite("arrow", "res/cursors/arrow.png", Vector(64));

	// load ux
	// sound.loadMusic("endgame", "");
	sound.loadSoundEffect("right click", "res/sounds/rclick.wav");
	sound.loadSoundEffect("left click", "res/sounds/lclick.wav");
}

void Game::quitSDL2()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::quitMedia()
{
	screen.deleteSprite();
	sound.deleteSoundEffect();
	sound.deleteMusic();
}