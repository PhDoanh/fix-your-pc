#include "Screen.hpp"
#include "Console.hpp"
#include "Game.hpp"

Screen screen;
std::map<std::string, Sprite *> sprites;

Screen::Screen()
{
}

Screen::~Screen()
{
}

void Screen::loadSprite(const std::string &name, const std::string &path, Vector real_size, int max_frame)
{
	console.log("Trying to load " + path + " ... ");
	sprites[name] = new Sprite();
	SDL_Surface *surface = IMG_Load(path.c_str());
	sprites[name]->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

	if (!sprites[name]->texture)
	{
		delete sprites[name];
		sprites[name] = nullptr;
		sprites.erase(name);
		console.error("Texture could not be created!");
	}

	console.log("Texture created!");
	sprites[name]->path = path;
	sprites[name]->real_size = real_size;
	sprites[name]->max_frame = max_frame;

	SDL_FreeSurface(surface);
}

void Screen::showSprite(const std::string &name)
{
}

void Screen::deleteSprite()
{
	std::string path;
	console.log("Deleting all sprites ...");
	for (auto &&sprite : sprites)
	{
		path = sprite.second->path;
		delete sprite.second;
		sprite.second = nullptr;
		if (sprite.second)
			console.error(path + " - fail");
		else
			console.log(path + " - done");
	}
}

// void Screen::loadFont(const std::string &path)
// {
// 	player_texture = IMG_LoadTexture(Game::renderer, path.c_str());
// 	if (!player_texture)
// 		console.error("Player image could not be loaded!");
// 	else
// 		console.log("Player image loaded!");
// 	if (SDL_QueryTexture(player_texture, NULL, NULL, &player_rect.w, &player_rect.h) != 0)
// 		console.error("Could not get player info");
// 	else
// 		console.log("Player info getted!");
// }

// void Screen::showFont()
// {

// 	if (SDL_RenderCopy(Game::renderer, player_texture, nullptr, &player_rect) != 0)
// 		console.error("Player image could not be displayed!");
// 	else
// 		console.log("Player image displayed!");
// 	SDL_RenderPresent(Game::renderer);
// }