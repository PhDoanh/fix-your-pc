#include "Game.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "Background.hpp"

Screen *screen = nullptr;
Sound *sound = nullptr;
Event *event = nullptr;
Background *bg = nullptr;
DevTool *dev = nullptr;
std::vector<Player *> players;
std::vector<Enemy *> enemies;
const Uint64 Enemy::spawn_time = 3000; // 3s
Uint64 Enemy::last_spawn_time = SDL_GetTicks64();
float Game::fps = 60.0;
int Game::win_w;
int Game::win_h;
bool Game::running = true;
std::stringstream Game::level;
std::queue<std::string> levels;
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
}

void Game::updateScreen()
{
	// update alls
	screen->updateBackground();
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
	dev->info("Initializing SDL2 ...");

	// Base init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		dev->error("SDL_Init - fail.");
	else
		dev->info("SDL_Init - done.");

	// Image init
	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
		dev->error("IMG_Init - fail.");
	else
		dev->info("IMG_Init - done.");

	// Font init
	if (TTF_Init() != 0)
		dev->error("TTF_Init - fail.");
	else
		dev->info("TTF_Init - done.");

	// Sound init
	if (!Mix_Init(MIX_INIT_MP3) || Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096) != 0)
		dev->error("Mix_Init - fail.");
	else
		dev->info("MIX_Init - done.");

	// Net init
	if (SDLNet_Init() != 0)
		dev->error("SDLNet_Init - fail.");
	else
		dev->info("SDLNet_Init - done.");

	// Create window
	window = SDL_CreateWindow(title.c_str(), 0, 0, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		dev->error("SDL_CreateWindow - fail.");
	else
		dev->info("SDL_CreateWindow - done.");

	// Get window's real size
	SDL_GetWindowSize(window, &win_w, &win_h);
	dev->info("window size: " + std::to_string(win_w) + "x" + std::to_string(win_h));

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		dev->error("SDL_CreateRenderer - fail.");
	else
		dev->info("SDL_CreateRenderer - done.");

	SDL_ShowCursor(SDL_DISABLE);
}

void Game::loadMedia()
{
	dev->info("Loading media ...");
	std::string text;
	screen = new Screen();
	sound = new Sound();
	event = new Event();
	bg = new Background();
	for (int i = 0; i < 1; i++)
		players.emplace_back(new Player("player" + std::to_string(i)));
	std::ifstream file("res/game_data/default_levels.txt");
	while (getline(file, text, '.'))
		levels.push(text);
	file.close();

	// Load UI
	screen->loadSprite("bg", "res/background/default.jpg", Vec2D(3840, 2400));
	screen->loadSprite("bg blur", "res/background/default_blur.jpg", Vec2D(3840, 2400));
	screen->loadSprite("window crash", "res/background/win_crash.jpg", Vec2D(5120, 2880));

	text = "enemy";
	for (int i = 1; i <= 50; i++)
		screen->loadSprite(text + std::to_string(i), "res/enemy/" + text + " (" + std::to_string(i) + ").png", Vec2D(256, 256));
	screen->loadSprite("boss", "res/enemy/boss.png", Vec2D(879, 501));

	screen->loadSprite("arrow", "res/player/arrow.png", Vec2D(64, 64));
	screen->loadSprite("beam", "res/player/beam.png", Vec2D(64, 44));
	screen->loadSprite("busy", "res/player/busy.png", Vec2D(1152, 64), 18);
	screen->loadSprite("helpsel", "res/player/helpsel.png", Vec2D(64, 64));
	screen->loadSprite("link", "res/player/link.png", Vec2D(64, 64));
	screen->loadSprite("move", "res/player/move.png", Vec2D(64, 64));
	screen->loadSprite("unvail", "res/player/unavail.png", Vec2D(64, 64));
	screen->loadSprite("working", "res/player/working.png", Vec2D(1152, 64), 18);
	std::vector<int> sizes = {18, 23, 26};
	for (int i = 0; i < sizes.size(); i++)
		screen->loadFont("res/SegUIVar.ttf", sizes[i]);

	// Load UX
	sound->loadSoundEffect("right click", "res/sound_effect/rclick.wav");
	sound->loadSoundEffect("left click", "res/sound_effect/lclick.wav");
	sound->loadSoundEffect("win error", "res/sound_effect/Windows Error.wav");
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
	delete bg;
	bg = nullptr;
	delete event;
	event = nullptr;
	delete sound;
	sound = nullptr;
	delete screen;
	screen = nullptr;
}