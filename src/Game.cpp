#include "Game.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "Level.hpp"
#include "UI.hpp"

Screen *screen = nullptr;
Sound *sound = nullptr;
Event *event = nullptr;
Level *level = nullptr;
UI *ui = nullptr;
std::vector<Player *> players;
std::vector<Enemy *> enemies;
std::map<std::string, int> settings;
const Uint64 Enemy::spawn_time = 3000; // 3s
Uint64 Enemy::last_spawn_time = SDL_GetTicks64();
float Game::fps = 60.0;
int Game::win_w;
int Game::win_h;
bool Game::running = true;
float Game::deltaTime = 0.01667;
std::fstream Game::data;
std::queue<std::string> lvs;
SDL_Window *Game::window = nullptr; // 1536x864
SDL_Renderer *Game::renderer = nullptr;

void Game::handleEvent()
{
	event->state = SDL_GetKeyboardState(nullptr);
	while (SDL_PollEvent(&event->e))
	{
		if (event->e.type == SDL_KEYDOWN && event->e.key.keysym.sym == SDLK_F4)
			running = false;
		event->handleMouse();
		event->handleKeyboard();
	}
}

void Game::updateScreen()
{
	// update alls
	screen->updateUI();
	screen->updateEnemies();
	screen->updatePlayer();

	// redraw alls
	SDL_RenderClear(renderer);

	screen->drawUI();
	screen->drawEnemies();
	screen->drawPlayer();

	SDL_RenderPresent(renderer);
}

void Game::initSDL2()
{
	info("Initializing SDL2 ...\n");

	// Base init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		error("SDL_Init - fail.\n");
	else
		info("SDL_Init - done.\n");

	// Image init
	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
		error("IMG_Init - fail.\n");
	else
		info("IMG_Init - done.\n");

	// Font init
	if (TTF_Init() != 0)
		error("TTF_Init - fail.\n");
	else
		info("TTF_Init - done.\n");

	// Sound init
	if (!Mix_Init(MIX_INIT_MP3) || Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096) != 0)
		error("Mix_Init - fail.\n");
	else
		info("MIX_Init - done.\n");

	// Net init
	if (SDLNet_Init() != 0)
		error("SDLNet_Init - fail.\n");
	else
		info("SDLNet_Init - done.\n");

	// Create window
	window = SDL_CreateWindow(title.c_str(), 0, 0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		error("SDL_CreateWindow - fail.\n");
	else
		info("SDL_CreateWindow - done.\n");

	// Get window's real size
	SDL_GetWindowSize(window, &win_w, &win_h);
	info("window size: " + std::to_string(win_w) + "x" + std::to_string(win_h));

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		error("SDL_CreateRenderer - fail.\n");
	else
		info("SDL_CreateRenderer - done.\n");

	SDL_ShowCursor(SDL_DISABLE);
}

void Game::loadMedia()
{
	info("Loading media ...\n");

	// load controller
	screen = new Screen();
	sound = new Sound();
	event = new Event();
	level = new Level();
	std::string key;
	int value;

	// load levels
	data.open("res/game_data/default_levels.txt");
	while (getline(data, key, '.'))
		lvs.push(key);
	data.close();

	// load settings
	data.open("res/game_data/settings.txt");
	while (!data.eof())
	{
		data >> key >> value;
		settings[key] = value;
	}
	data.close();

	// Load UI, UX
	screen->loadSprite("crash", "res/background/crash.png", Vec2D(5120, 2880));
	screen->loadSprite("flower blur", "res/background/flower_blur.png", Vec2D(3840, 2400));
	screen->loadSprite("flower", "res/background/flower.png", Vec2D(3840, 2400));
	screen->loadSprite("gradient", "res/background/gradient.png", Vec2D(3840, 2400));
	screen->loadSprite("space", "res/background/space.png", Vec2D(4096));
	screen->loadSprite("stars", "res/background/stars.png", Vec2D(4096));
	key = "enemy";
	for (int i = 1; i <= 50; i++)
		screen->loadSprite(key + std::to_string(i), "res/enemy/" + key + " (" + std::to_string(i) + ").png", Vec2D(256, 256));
	screen->loadSprite("arrow", "res/player/arrow.png", Vec2D(64, 64));
	screen->loadSprite("beam", "res/player/beam.png", Vec2D(64, 44));
	screen->loadSprite("busy", "res/player/busy.png", Vec2D(1152, 64), 18);
	screen->loadSprite("helpsel", "res/player/helpsel.png", Vec2D(64, 64));
	screen->loadSprite("link", "res/player/link.png", Vec2D(64, 64));
	screen->loadSprite("move", "res/player/move.png", Vec2D(64, 64));
	screen->loadSprite("unvail", "res/player/unavail.png", Vec2D(64, 64));
	screen->loadSprite("working", "res/player/working.png", Vec2D(1152, 64), 18);
	screen->loadFont("ui", "res/SegUIVar.ttf", {18, 23, 26}); // main font
	ui = new UI();

	sound->loadSoundEffect("rclick", "res/sound_effect/rclick.wav");
	sound->loadSoundEffect("lclick", "res/sound_effect/lclick.wav");
	sound->loadSoundEffect("win error", "res/sound_effect/Windows Error.wav");

	// load objects
	for (int i = 0; i < 1; i++)
		players.emplace_back(new Player("player" + std::to_string(i), Vec2D(win_w / 2.0, win_h / 2.0)));
}

void Game::quitSDL2()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDLNet_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::quitMedia()
{
	for (int i = 0; i < players.size(); i++)
	{
		delete players[i];
		players[i] = nullptr;
	}
	delete ui;
	ui = nullptr;

	sound->deleteSoundEffects();
	sound->deleteMusics();
	screen->deleteTexts();
	screen->deleteFonts();
	screen->deleteSprites();

	delete level;
	level = nullptr;
	delete event;
	event = nullptr;
	delete sound;
	sound = nullptr;
	delete screen;
	screen = nullptr;
}