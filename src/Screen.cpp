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
	console.info("Trying to load " + path + " ... ");
	sprites[name] = new Sprite();
	SDL_Surface *surface = IMG_Load(path.c_str());
	sprites[name]->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

	if (!sprites[name]->texture)
	{
		delete sprites[name];
		sprites[name] = nullptr;
		sprites.erase(name);
		console.error(path + " - fail.");
	}

	console.info(path + " - done.");
	sprites[name]->path = path;
	sprites[name]->real_size = real_size;
	sprites[name]->max_frame = max_frame;

	SDL_FreeSurface(surface);
}

void Screen::drawSprite(Sprite &sprite, const Vector &pos, const Vector &size, float scale, int cur_frame, bool flip)
{
	int x = (cur_frame % sprite.max_frame) * sprite.real_size.x;
	int y = 0;
	int w = sprite.real_size.x;
	int h = sprite.real_size.y;
	SDL_Rect src_rect = {x, y, w, h};
	SDL_Rect dst_rect = Rect::reScale(pos, size, scale);
	SDL_RenderCopyEx(Game::renderer, sprite.texture, &src_rect, &dst_rect, 0, NULL, (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

void Screen::deleteSprite()
{
	std::string path;
	console.info("Deleting all sprites ...");
	for (auto &&sprite : sprites)
	{
		path = sprite.second->path;
		delete sprite.second;
		sprite.second = nullptr;
		if (sprite.second)
			console.error(path + " - fail.");
		else
			console.info(path + " - done.");
	}
}

// void Screen::loadFont(const std::string &path)
// {
// 	player_texture = IMG_LoadTexture(Game::renderer, path.c_str());
// 	if (!player_texture)
// 		console.error("Player image could not be loaded!");
// 	else
// 		console.info("Player image loaded!");
// 	if (SDL_QueryTexture(player_texture, NULL, NULL, &player_rect.w, &player_rect.h) != 0)
// 		console.error("Could not get player info");
// 	else
// 		console.info("Player info getted!");
// }

// void Screen::showFont()
// {

// 	if (SDL_RenderCopy(Game::renderer, player_texture, nullptr, &player_rect) != 0)
// 		console.error("Player image could not be displayed!");
// 	else
// 		console.info("Player image displayed!");
// 	SDL_RenderPresent(Game::renderer);
// }