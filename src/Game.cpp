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
Player *player = nullptr;
Uint64 Enemy::spawn_time = 3000;
Uint64 Enemy::last_spawn_time = SDL_GetTicks64();
std::vector<Enemy *> enemies;
std::vector<Enemy *> static_enemies;
std::map<std::string, int> settings;
std::queue<std::queue<std::string>> lvs;
std::multimap<int, std::string, std::greater<int>> high_scores;
float Game::fps = 60.0;
int Game::state = ready;
int Game::prev_state = pause;
int Game::win_w;
int Game::win_h;
bool Game::running = true;
float Game::deltaTime = 0.01667;
std::fstream Game::data;
SDL_Window *Game::window = nullptr; // 1536x864
SDL_Renderer *Game::renderer = nullptr;

void Game::handleEvent()
{
	event->state = SDL_GetKeyboardState(nullptr);
	while (SDL_PollEvent(&event->e))
	{
		// if (event->e.type == SDL_KEYDOWN && event->e.key.keysym.sym == SDLK_F4)
		// 	running = false;
		event->handleMouse();
		event->handleKeyboard();
	}
}

void Game::updateScreen()
{
	screen->update();
	SDL_RenderClear(renderer);
	screen->draw();
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
	if (!IMG_Init(IMG_INIT_PNG))
		error("IMG_Init - fail.\n");
	else
		info("IMG_Init - done.\n");

	// Font init
	if (TTF_Init() != 0)
		error("TTF_Init - fail.\n");
	else
		info("TTF_Init - done.\n");

	// Sound init
	if (!Mix_Init(MIX_INIT_OGG | MIX_INIT_WAVPACK) || Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096) != 0)
		error("Mix_Init - fail.\n");
	else
		info("MIX_Init - done.\n");

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

	// load controllers
	screen = new Screen();
	sound = new Sound();
	event = new Event();
	level = new Level();

	// Load UI, UX
	// screen->loadSprite("crash", "res/background/crash.png", Vec2D(5120, 2880));
	screen->loadSprite("flower", "res/background/flower.png", Vec2D(3840, 2400));
	screen->loadSprite("space", "res/background/space.png", Vec2D(4096));
	screen->loadSprite("stars", "res/background/stars.png", Vec2D(4096));
	screen->loadSprite("full", "res/background/full.png", Vec2D(3840, 2400));
	screen->loadSprite("full blur", "res/background/full_blur.png", Vec2D(3840, 2400));
	for (int i = 1; i <= 50; i++)
		screen->loadSprite("enemy" + std::to_string(i), "res/enemy/enemy (" + std::to_string(i) + ").png", Vec2D(256, 256));
	screen->loadSprite("arrow", "res/player/arrow.png", Vec2D(65, 91), 5, 5);
	screen->loadSprite("beam", "res/player/beam.png", Vec2D(64, 44));
	screen->loadSprite("link", "res/player/link.png", Vec2D(64));
	screen->loadSprite("move", "res/player/move.png", Vec2D(64));
	screen->loadSprite("emp", "res/object/emp.png", Vec2D(256));
	screen->loadSprite("reticle", "res/object/reticle.png", Vec2D(256));
	screen->loadSprite("avatar", "res/object/avatar.png", Vec2D(250));
	screen->loadSprite("bullet", "res/object/bullet.png", Vec2D(21, 28));
	screen->loadSprite("game src", "res/object/game_src.png", Vec2D(148));
	screen->loadSprite("smile", "res/object/smile.png", Vec2D(452, 222));
	screen->loadFont("ui", "res/SegUIVar.ttf", {18, 24, 36, 48, 60}); // main font

	sound->loadSoundEffect("rclick", "res/sound/rclick.wav");
	sound->loadSoundEffect("lclick", "res/sound/lclick.wav");
	sound->loadSoundEffect("error", "res/sound/Windows Error.wav");
	sound->loadSoundEffect("startup", "res/sound/Windows Startup.wav");
	sound->loadSoundEffect("critical stop", "res/sound/Windows Critical Stop.wav");
	sound->loadSoundEffect("shutdown", "res/sound/Windows Shutdown.wav");
	sound->loadSoundEffect("notify", "res/sound/Windows Notify System Generic.wav");
	sound->loadSoundEffect("balloon", "res/sound/Windows Balloon.wav");
	sound->loadSoundEffect("cancel", "res/sound/cancel.wav");
	sound->loadSoundEffect("typing", "res/sound/click.wav");
	sound->loadSoundEffect("emp", "res/sound/emp.wav");
	sound->loadSoundEffect("explosion large", "res/sound/explosion-large.wav");
	sound->loadSoundEffect("explosion player", "res/sound/explosion-player.wav");
	sound->loadSoundEffect("explosion small", "res/sound/explosion-small.wav");
	sound->loadSoundEffect("explosion", "res/sound/explosion.wav");
	sound->loadSoundEffect("hit", "res/sound/hit.wav");
	sound->loadSoundEffect("plasma", "res/sound/plasma.wav");
	sound->loadSoundEffect("spawn", "res/sound/spawn.wav");
	sound->loadSoundEffect("target", "res/sound/target.wav");
	sound->loadSoundEffect("health loss", "res/sound/breaking.wav");
	sound->loadSoundEffect("locked", "res/sound/locked.wav");
	sound->loadSoundEffect("unlocked", "res/sound/unlocked.wav");
	sound->playSoundEffect("startup", general);

	ui = new UI();
	ui->loadElements();
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
	ui->deleteElements();
	sound->deleteMusics();
	sound->deleteSoundEffects();
	screen->deleteTexts();
	screen->deleteFonts();
	screen->deleteSprites();

	delete ui;
	ui = nullptr;
	delete level;
	level = nullptr;
	delete event;
	event = nullptr;
	delete sound;
	sound = nullptr;
	delete screen;
	screen = nullptr;
}