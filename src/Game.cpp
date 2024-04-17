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
std::vector<Enemy *> enemies;
std::map<std::string, int> settings;
std::queue<std::queue<std::string>> lvs;
const Uint64 Enemy::spawn_time = 3000; // 3s
Uint64 Enemy::last_spawn_time = SDL_GetTicks64();
float Game::fps = 60.0;
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
	ui = new UI();
	std::string key, sentence, word;
	int value;

	// load levels
	data.open("res/game_data/default_levels.txt");
	while (getline(data, sentence, '.'))
	{
		std::queue<std::string> lv;
		std::stringstream ss(sentence);
		while (ss >> word)
			lv.push(word);
		lvs.push(lv);
	}
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
	screen->loadSprite("arrow", "res/player/arrow.png", Vec2D(65, 91), 5, 5);
	screen->loadSprite("beam", "res/player/beam.png", Vec2D(64, 44));
	screen->loadSprite("link", "res/player/link.png", Vec2D(64));
	screen->loadSprite("move", "res/player/move.png", Vec2D(64));
	screen->loadSprite("unvail", "res/player/unavail.png", Vec2D(64));
	screen->loadSprite("emp", "res/object/emp.png", Vec2D(256));
	screen->loadSprite("reticle", "res/object/reticle.png", Vec2D(256));
	screen->loadSprite("avatar", "res/object/avatar.png", Vec2D(250));
	screen->loadFont("ui", "res/SegUIVar.ttf", {18, 23, 26}); // main font

	sound->loadSoundEffect("rclick", "res/sound/rclick.wav");
	sound->loadSoundEffect("lclick", "res/sound/lclick.wav");
	sound->loadSoundEffect("error", "res/sound/Windows Error.wav");
	sound->loadSoundEffect("unlock", "res/sound/Windows Unlock.wav");
	sound->loadSoundEffect("critical stop", "res/sound/Windows Critical Stop.wav");
	sound->loadSoundEffect("shutdown", "res/sound/Windows Shutdown.wav");
	sound->loadSoundEffect("notify", "res/sound/Windows Notify System Generic.wav");
	sound->loadSoundEffect("new level", "res/sound/Windows Balloon.wav");
	sound->loadSoundEffect("cancel", "res/sound/cancel.ogg");
	sound->loadSoundEffect("typing", "res/sound/click.ogg");
	sound->loadSoundEffect("emp", "res/sound/emp.ogg");
	sound->loadSoundEffect("explosion large", "res/sound/explosion-large.ogg");
	sound->loadSoundEffect("explosion player", "res/sound/explosion-player.ogg");
	sound->loadSoundEffect("explosion small", "res/sound/explosion-small.ogg");
	sound->loadSoundEffect("explosion", "res/sound/explosion.ogg");
	sound->loadSoundEffect("hit", "res/sound/hit.ogg");
	sound->loadSoundEffect("plasma", "res/sound/plasma.ogg");
	sound->loadSoundEffect("spawn", "res/sound/spawn.ogg");
	sound->loadSoundEffect("target", "res/sound/target.ogg");
	sound->loadMusic("endure", "res/music/endure.ogg");
	sound->loadMusic("orientation", "res/music/orientation.ogg");

	player = new Player("player", Vec2D(win_w / 2.0, win_h / 2.0), Vec2D(43.3, 60.6));
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
	sound->deleteSoundEffects();
	sound->deleteMusics();
	screen->deleteTexts();
	screen->deleteFonts();
	screen->deleteSprites();

	delete player;
	player = nullptr;
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