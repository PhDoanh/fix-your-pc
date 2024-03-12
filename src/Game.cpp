#include "Console.hpp"
#include "Enemy.hpp"
#include "Event.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Utility.hpp"

#define NUM_BG 10
#define MAX_NUM_ENEMY 50

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
	std::srand(std::time(NULL));
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
			running = false;
		event.handleMouse();
		event.handleKeyboard();
	}
}

void Game::updateScreen()
{
	SDL_RenderClear(Game::renderer);

	// Sprite bg = *sprites["bg1"];
	// Sprite arrow = *sprites["arrow"];
	player.move();

	screen.drawSprite(*sprites["bg1"], Vector(), Vector(win_w, win_h), 1, 1, false);
	screen.drawTileMap();
	screen.drawSprite(*sprites["arrow"], Vector(player.rect.x, player.rect.y), Vector(player.rect.w, player.rect.h), 1, 1, false);

	SDL_RenderPresent(Game::renderer);
}

void Game::prepareSDL2()
{
	console.info("Initializing SDL2 ...");

	// Base init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		console.error("SDL_Init - fail.");
	else
		console.info("SDL_Init - done.");

	// Image init
	if (!IMG_Init(IMG_INIT_PNG))
		console.error("IMG_Init - fail.");
	else
		console.info("IMG_Init - done.");

	// Font init
	if (TTF_Init() != 0)
		console.error("TTF_Init - fail.");
	else
		console.info("TTF_Init - done.");

	// Sound init
	if (!Mix_Init(MIX_INIT_MP3) || Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096) != 0)
		console.error("Mix_Init - fail.");
	else
		console.info("MIX_Init - done.");

	// Create window and renderer
	window = SDL_CreateWindow(title.c_str(), 0, 0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		console.error("SDL_CreateWindow - fail.");
	else
		console.info("SDL_CreateWindow - done.");

	SDL_GetWindowSize(window, &win_w, &win_h);
	console.log("window size: " + std::to_string(win_w) + "x" + std::to_string(win_h));

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		console.error("SDL_CreateRenderer - fail.");
	else
		console.info("SDL_CreateRenderer - done.");

	// some fixed set up
	SDL_ShowCursor(SDL_DISABLE);
}

void Game::prepareMedia()
{
	console.info("Loading media ...");

	// Load UI
	std::string name;
	name = "bg";
	for (int i = 1; i <= NUM_BG; i++)
		screen.loadSprite(name + std::to_string(i), "res/backgrounds/" + name + " (" + std::to_string(i) + ").png", Vector(3840, 2400));

	name = "enemy";
	for (int i = 1; i <= MAX_NUM_ENEMY; i++)
		screen.loadSprite(name + std::to_string(i), "res/enemy/" + name + " (" + std::to_string(i) + ").png", Vector(256, 256));

	screen.loadSprite("arrow", "res/player/arrow.png", Vector(64, 64));
	screen.loadSprite("beam", "res/player/beam.png", Vector(64, 44));
	screen.loadSprite("busy", "res/player/busy.png", Vector(1152, 64), 18);
	screen.loadSprite("helpsel", "res/player/helpsel.png", Vector(64, 64));
	screen.loadSprite("link", "res/player/link.png", Vector(64, 64));
	screen.loadSprite("move", "res/player/move.png", Vector(64, 64));
	screen.loadSprite("unvail", "res/player/unavail.png", Vector(64, 64));
	screen.loadSprite("working", "res/player/working.png", Vector(1152, 64), 18);

	// Load UX
	sound.loadSoundEffect("rclick", "res/sound_effects/rclick.wav");
	sound.loadSoundEffect("lclick", "res/sound_effects/lclick.wav");
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