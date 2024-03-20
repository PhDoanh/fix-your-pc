#include "Game.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"
#include "Entity.hpp"

#define BG_NUM_PATHS 10
#define ENEMY_NUM_PATHS 50

Screen *screen = nullptr;
Sound *sound = nullptr;
Event *event = nullptr;
std::vector<Player *> players;
std::vector<Enemy *> enemies;
float Game::fps = 60.0;
int Game::win_w = 0;
int Game::win_h = 0;
bool Game::running = true;
SDL_Window *Game::window = nullptr;
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
	log(event->cur_txt_inp);
}

void Game::updateScreen()
{
	// update alls
	screen->updateEnemies();
	screen->updatePlayer();

	// redraw alls
	SDL_RenderClear(renderer);

	screen->drawBackground();
	screen->drawEnemies();
	screen->drawPlayer();

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

	// Net init
	if (SDLNet_Init() != 0)
		error("SDLNet_Init - fail.");
	else
		info("SDLNet_Init - done.");

	// Create window
	window = SDL_CreateWindow(title.c_str(), 0, 0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		error("SDL_CreateWindow - fail.");
	else
		info("SDL_CreateWindow - done.");

	// Get window's real size
	SDL_GetWindowSize(window, &win_w, &win_h);
	info("window size: " + std::to_string(win_w) + "x" + std::to_string(win_h));

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		error("SDL_CreateRenderer - fail.");
	else
		info("SDL_CreateRenderer - done.");

	SDL_ShowCursor(SDL_DISABLE);
}

void Game::loadMedia()
{
	info("Loading media ...");

	std::string name;
	screen = new Screen();
	sound = new Sound();
	event = new Event();
	for (int i = 0; i < 1; i++)
		players.emplace_back(new Player("player" + std::to_string(i)));
	std::ifstream file("res/enemy_names.txt");
	for (int i = 0; i < 10; i++)
	{
		file >> name;
		enemies.emplace_back(new Enemy(name));
		enemies[i]->spawn();
	}
	file.close();

	// Load UI
	name = "bg";
	for (int i = 1; i <= BG_NUM_PATHS; i++)
		screen->loadSprite(name + std::to_string(i), "res/backgrounds/" + name + " (" + std::to_string(i) + ").jpg", Vec2D(3840, 2400));
	screen->loadSprite("window crash", "res/backgrounds/win_crash.jpg", Vec2D(5120, 2880));

	name = "enemy";
	for (int i = 1; i <= ENEMY_NUM_PATHS; i++)
		screen->loadSprite(name + std::to_string(i), "res/enemy/" + name + " (" + std::to_string(i) + ").png", Vec2D(256, 256));
	screen->loadSprite("boss", "res/enemy/boss.png", Vec2D(879, 501));

	screen->loadSprite("arrow", "res/player/arrow.png", Vec2D(64, 64));
	screen->loadSprite("beam", "res/player/beam.png", Vec2D(64, 44));
	screen->loadSprite("busy", "res/player/busy.png", Vec2D(1152, 64), 18);
	screen->loadSprite("helpsel", "res/player/helpsel.png", Vec2D(64, 64));
	screen->loadSprite("link", "res/player/link.png", Vec2D(64, 64));
	screen->loadSprite("move", "res/player/move.png", Vec2D(64, 64));
	screen->loadSprite("unvail", "res/player/unavail.png", Vec2D(64, 64));
	screen->loadSprite("working", "res/player/working.png", Vec2D(1152, 64), 18);
	screen->loadFont("big", "res/SegUIVar.ttf", 32);
	screen->loadFont("medium", "res/SegUIVar.ttf", 28);
	screen->loadFont("small", "res/SegUIVar.ttf", 23);

	// Load UX
	sound->loadSoundEffect("right click", "res/sound_effects/rclick.wav");
	sound->loadSoundEffect("left click", "res/sound_effects/lclick.wav");
	sound->loadSoundEffect("win error", "res/sound_effects/Windows Error.wav");
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
	sound->deleteSoundEffects();
	sound->deleteMusics();
	screen->deleteFonts();
	screen->deleteSprites();

	for (int i = 0; i < players.size(); i++)
	{
		delete players[i];
		players[i] = nullptr;
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}
	delete screen;
	screen = nullptr;
	delete sound;
	sound = nullptr;
	delete event;
	event = nullptr;
}