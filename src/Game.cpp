#include "Game.hpp"
#include "Enemies.hpp"
#include "Player.hpp"
#include "util.hpp"

float Game::fps = 60.0;
int Game::win_w = 0;
int Game::win_h = 0;
bool Game::running = false;
std::string Game::title = "Fix Your PC";
SDL_Window *Game::window = nullptr;
SDL_Renderer *Game::renderer = nullptr;

Game::Game()
{
	info("Game constructor called!");

	initSDL2();
	loadMedia();
}

Game::~Game()
{
	info("Game destructor called!");

	quitMedia();
	quitSDL2();
}

void Game::start()
{
	std::srand(std::time(NULL));
	float frame_start, frame_time, frame_delay = 1000.0 / fps;

	running = true;
	while (running)
	{
		frame_start = float(SDL_GetTicks());

		handleEvent();
		updateScreen();

		frame_time = float(SDL_GetTicks()) - frame_start;
		if (frame_delay > frame_time)
			SDL_Delay(frame_delay - frame_time);
	}
}

void Game::handleEvent()
{
	event.state = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event.e))
	{
		event.handleMouse();
		event.handleKeyboard();
	}
}

void Game::updateScreen()
{
	// update alls
	screen.updateEnemies();
	screen.updatePlayer();

	// redraw alls
	SDL_RenderClear(renderer);

	screen.drawBackground();
	screen.drawEnemies();
	screen.drawPlayer();

	SDL_RenderPresent(renderer);
}

void Game::initSDL2()
{
	info("Initializing SDL2 ...");

	// Base init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		error("SDL_Init - fail.");
	else
		info("SDL_Init - done.");

	// Image init
	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
		error("IMG_Init - fail.");
	else
		info("IMG_Init - done.");

	// Font init
	if (TTF_Init() != 0)
		error("TTF_Init - fail.");
	else
		info("TTF_Init - done.");

	// Sound init
	if (!Mix_Init(MIX_INIT_MP3) || Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096) != 0)
		error("Mix_Init - fail.");
	else
		info("MIX_Init - done.");

	// Create window and renderer
	window = SDL_CreateWindow(title.c_str(), 0, 0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		error("SDL_CreateWindow - fail.");
	else
		info("SDL_CreateWindow - done.");

	SDL_GetWindowSize(window, &win_w, &win_h);
	log("window size: " + std::to_string(win_w) + "x" + std::to_string(win_h));

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		error("SDL_CreateRenderer - fail.");
	else
		info("SDL_CreateRenderer - done.");

	// some fixed set up
	SDL_ShowCursor(SDL_DISABLE);
}

void Game::loadMedia()
{
	info("Loading media ...");

	// Load UI
	std::string name;
	name = "bg";
	for (int i = 1; i <= NUM_BG; i++)
		screen.loadSprite(name + std::to_string(i), "res/backgrounds/" + name + " (" + std::to_string(i) + ").jpg", Vec2D(3840, 2400));
	screen.loadSprite("window crash", "res/backgrounds/win_crash.jpg", Vec2D(5120, 2880));

	name = "enemy";
	for (int i = 1; i <= MAX_NUM_ENEMY; i++)
		screen.loadSprite(name + std::to_string(i), "res/enemy/" + name + " (" + std::to_string(i) + ").png", Vec2D(256, 256));
	screen.loadSprite("boss", "res/enemy/boss.png", Vec2D(879, 501));

	screen.loadSprite("arrow", "res/player/arrow.png", Vec2D(64, 64));
	screen.loadSprite("beam", "res/player/beam.png", Vec2D(64, 44));
	screen.loadSprite("busy", "res/player/busy.png", Vec2D(1152, 64), 18);
	screen.loadSprite("helpsel", "res/player/helpsel.png", Vec2D(64, 64));
	screen.loadSprite("link", "res/player/link.png", Vec2D(64, 64));
	screen.loadSprite("move", "res/player/move.png", Vec2D(64, 64));
	screen.loadSprite("unvail", "res/player/unavail.png", Vec2D(64, 64));
	screen.loadSprite("working", "res/player/working.png", Vec2D(1152, 64), 18);
	// screen.loadFont("thin", "res/fonts/Sarabun-Thin.ttf");
	// screen.loadFont("light", "res/fonts/Sarabun-Light.ttf");
	// screen.loadFont("medium", "res/fonts/Sarabun-Medium.ttf");
	// screen.loadFont("regular", "res/fonts/Sarabun-Regular.ttf");
	// screen.loadFont("bold", "res/fonts/Sarabun-Bold.ttf");
	// screen.loadFont("italic", "res/fonts/Sarabun-Italic.ttf");

	// Load UX
	sound.loadSoundEffect("right click", "res/sound_effects/rclick.wav");
	sound.loadSoundEffect("left click", "res/sound_effects/lclick.wav");
	sound.loadSoundEffect("win error", "res/sound_effects/Windows Error.wav");
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
	screen.deleteSprites();
	sound.deleteSoundEffects();
	sound.deleteMusics();
}