#include "Screen.hpp"
#include "Console.hpp"
#include "Game.hpp"
#include "Utility.hpp"

Screen screen;
std::map<std::string, Sprite *> sprites;
std::map<std::string, TTF_Font *> fonts;

Screen::Screen()
{
	std::vector<int> index(50);
	std::iota(index.begin(), index.end(), 1);
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 8; y++)
		{
			if (tilemap[x][y])
			{
				tilemap[x][y] = index[rand() % index.size()];
				index.erase(std::find(index.begin(), index.end(), tilemap[x][y]));
			}
		}

	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 8; y++)
		{
			tile[x][y].x = x * 96;
			tile[x][y].y = y * 96;
			tile[x][y].w = 96;
			tile[x][y].h = 96;
		}
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

void Screen::drawTileMap()
{
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 8; y++)
			if (tilemap[x][y])
				SDL_RenderCopy(
					Game::renderer,
					sprites["enemy" + std::to_string(tilemap[x][y])]->texture,
					NULL,
					&tile[x][y]);
}

void Screen::drawDialog(bool flag)
{
	if (flag)
	{
		Sprite *dialog = sprites["boss"];
		int w = dialog->real_size.x;
		int h = dialog->real_size.y;
		int x = (Game::win_w - w) / 2.0;
		int y = (Game::win_h - h) / 2.0;
		screen.drawSprite(*dialog, Vector(x, y), Vector(w, h), 1, 1, false);
	}
}

void Screen::loadFont(const std::string &name, const std::string &path, int font_size)
{
	console.info("Trying to load " + path + " ... ");
	fonts[name] = TTF_OpenFont(path.c_str(), font_size);
	if (!fonts[name])
	{
		TTF_CloseFont(fonts[name]);
		fonts[name] = nullptr;
		fonts.erase(name);
		console.error(path + " - fail.");
	}
	console.info(path + " - done.");
}

void Screen::renderFont(const std::string &content, const Vector &pos, const Vector &size, SDL_Color color, float scale, const std::string &name)
{
	SDL_Surface *surface = TTF_RenderText_Blended(fonts[name], content.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect dst_rect = Rect::reScale(pos, size, scale);
	SDL_RenderCopy(Game::renderer, texture, NULL, &dst_rect);
}