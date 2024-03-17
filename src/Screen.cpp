#include "Game.hpp"
#include "Screen.hpp"
#include "Event.hpp"

std::map<std::string, Sprite *> sprites;
std::map<std::string, TTF_Font *> fonts;

Screen::Screen(/* args */)
{
	info("Screen constructor called!");

	std::vector<int> indexes(50);
	std::iota(indexes.begin(), indexes.end(), 1);
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 8; y++)
		{
			if (tilemap[x][y])
			{
				tilemap[x][y] = indexes[rand() % indexes.size()];
				indexes.erase(std::find(indexes.begin(), indexes.end(), tilemap[x][y]));
			}
		}

	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 8; y++)
		{
			int index = tilemap[x][y];
			if (index)
			{
				enemy[index]->x = x * 96;
				enemy[index]->y = y * 96;
				enemy[index]->w = 96;
				enemy[index]->h = 96;
			}
		}
}

Screen::~Screen()
{
	info("Screen destructor called!");
}

void Screen::loadSprite(const std::string &name, const std::string &path, Vector real_size, int max_frame)
{
	info("Trying to load " + path + " ... ");
	sprites[name] = new Sprite();
	SDL_Surface *surface = IMG_Load(path.c_str());
	sprites[name]->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

	if (!sprites[name]->texture)
	{
		delete sprites[name];
		sprites[name] = nullptr;
		sprites.erase(name);
		error(path + " - fail.");
	}

	info(path + " - done.");
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

void Screen::deleteSprites()
{
	std::string path;
	info("Deleting all sprites ...");
	for (auto &&sprite : sprites)
	{
		path = sprite.second->path;
		delete sprite.second;
		sprite.second = nullptr;
		if (sprite.second)
			error(path + " - fail.");
		else
			info(path + " - done.");
	}
}

void Screen::updateEnemies()
{
	for (int i = 1; i <= 50; i++)
		enemies.bounceMotion(i);
}

void Screen::updatePlayer()
{
	player.update();
}

void Screen::drawBackground()
{
	drawSprite(*sprites["bg1"], Vector(), Vector(Game::win_w, Game::win_h), 1, 1, false);
}

void Screen::drawEnemies()
{
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 8; y++)
		{
			int index = tilemap[x][y];
			if (index)
				drawSprite(
					*sprites["enemy" + std::to_string(index)],
					Vector(enemy[index]->x, enemy[index]->y),
					Vector(enemy[index]->w, enemy[index]->h),
					1, 1, false);
		}
}

void Screen::drawPlayer()
{
	drawSprite(*sprites["arrow"], Vector(player.x, player.y), Vector(player.w, player.h), 1, 1, false);
}

// void Screen::drawDialog()
// {
// 	if (flag)
// 	{
// 		Sprite *dialog = sprites["boss"];
// 		int w = dialog->real_size.x;
// 		int h = dialog->real_size.y;
// 		int x = (Game::win_w - w) / 2.0;
// 		int y = (Game::win_h - h) / 2.0;
// 	 rawSprite(*dialog, Vector(x, y), Vector(w, h), 1, 1, false);
// 	}
// }

// void Screen::loadFont(const std::string &name, const std::string &path, int font_size)
// {
// 	info("Trying to load " + path + " ... ");
// 	fonts[name] = TTF_OpenFont(path.c_str(), font_size);
// 	if (!fonts[name])
// 	{
// 		TTF_CloseFont(fonts[name]);
// 		fonts[name] = nullptr;
// 		fonts.erase(name);
// 		error(path + " - fail.");
// 	}
// 	info(path + " - done.");
// }

// void Screen::renderFont(const std::string &content, const Vector &pos, const Vector &size, SDL_Color color, float scale, const std::string &name)
// {
// 	SDL_Surface *surface = TTF_RenderText_Blended(fonts[name], content.c_str(), color);
// 	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
// 	SDL_FreeSurface(surface);
// 	SDL_Rect dst_rect = Rect::reScale(pos, size, scale);
// 	SDL_RenderCopy(Game::renderer, texture, NULL, &dst_rect);
// }