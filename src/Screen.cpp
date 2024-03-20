#include "Game.hpp"
#include "Screen.hpp"
#include "Event.hpp"
#include "Entity.hpp"

std::map<std::string, Sprite *> sprites;
std::map<std::string, Font *> fonts;

Screen::Screen()
{
	info("Screen constructor called!");

	// std::vector<int> indexes(50);
	// std::iota(indexes.begin(), indexes.end(), 1);
	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 8; y++)
	// 	{
	// 		if (tilemap[x][y])
	// 		{
	// 			tilemap[x][y] = indexes[rand() % indexes.size()];
	// 			indexes.erase(std::find(indexes.begin(), indexes.end(), tilemap[x][y]));
	// 		}
	// 	}

	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 8; y++)
	// 	{
	// 		int index = tilemap[x][y];
	// 		if (index)
	// 		{
	// 			enemy[index - 1].rect.x = x * 96;
	// 			enemy[index - 1].rect.y = y * 96;
	// 		}
	// 	}
}

Screen::~Screen() { info("Screen destructor called!"); }

void Screen::loadSprite(const std::string &name, const std::string &path, Vec2D real_size, int max_frame)
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

void Screen::drawSprite(Sprite &sprite, const Vec2D &pos, const Vec2D &size, float scale, int cur_frame, bool flip)
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

void Screen::loadFont(const std::string &name, const std::string &path, const int &size)
{
	info("Trying to load " + path + " ... ");
	fonts[name] = new Font();
	fonts[name]->font = TTF_OpenFont(path.c_str(), size);
	if (!fonts[name]->font)
	{
		TTF_CloseFont(fonts[name]->font);
		fonts[name] = nullptr;
		fonts.erase(name);
		error(path + " - fail.");
	}
	info(path + " - done.");
	fonts[name]->path = path;
}

void Screen::renderFont(Font &my_font, const std::string &txt, const Vec2D &pos, int txt_type, SDL_Color txt_color, SDL_Color bg_color)
{
	SDL_Surface *surface = nullptr;
	if (txt_type == solid)
		surface = TTF_RenderText_Solid(my_font.font, txt.c_str(), txt_color);
	if (txt_type == blended)
		surface = TTF_RenderText_Blended(my_font.font, txt.c_str(), txt_color);
	if (txt_type == shaded)
		surface = TTF_RenderText_Shaded(my_font.font, txt.c_str(), txt_color, bg_color);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_QueryTexture(texture, nullptr, nullptr, &my_font.w, &my_font.h);
	SDL_Rect dst_rect = {int(pos.x), int(pos.y), my_font.w, my_font.h};
	SDL_RenderCopy(Game::renderer, texture, nullptr, &dst_rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void Screen::deleteFonts()
{
	info("Deleting all fonts ...");
	std::string path;
	for (auto &&font : fonts)
	{
		path = font.second->path;
		TTF_CloseFont(font.second->font);
		delete font.second;
		font.second = nullptr;
		if (font.second)
			error(path + "- fail.");
		else
			info(path + "- done.");
	}
}

void Screen::updateEnemies()
{
	if (enemies.size() <= 3)
		for (int i = 0; i < 10; i++)
			enemies[i]->spawn();
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->move();
		enemies[i]->attack();
		enemies[i]->takeDamage();
	}
}

void Screen::updatePlayer()
{
	if (event->moving)
		players[0]->move();
	players[0]->attack();
	players[0]->takeDamage();
}

void Screen::drawBackground()
{
	drawSprite(
		*sprites["bg1"],
		Vec2D(),
		Vec2D(Game::win_w, Game::win_h),
		1, 1, false);
}

void Screen::drawEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		drawSprite(
			*sprites["enemy" + std::to_string(i + 1)],
			Vec2D(enemies[i]->x, enemies[i]->y),
			Vec2D(96, 96),
			1, 1, false);
		renderFont(
			*fonts["small"],
			enemies[i]->name,
			Vec2D(enemies[i]->x + (96 - fonts["small"]->w) / 2, enemies[i]->y + 96));
	}
}

void Screen::drawPlayer()
{
	drawSprite(
		*sprites["arrow"],
		Vec2D(players[0]->x, players[0]->y),
		Vec2D(64, 64),
		1, 1, false);
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
// 	 rawSprite(*dialog, Vec2D(x, y), Vec2D(w, h), 1, 1, false);
// 	}
// }