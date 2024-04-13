#include "Game.hpp"
#include "Screen.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "Background.hpp"

std::map<std::string, Sprite *> sprites;
std::map<int, TTF_Font *> font_sizes;

Screen::Screen()
{
	dev->info("Screen constructor called!");

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

Screen::~Screen() { dev->info("Screen destructor called!"); }

void Screen::loadSprite(const std::string &name, const std::string &path, Vec2D real_size, int max_frame)
{
	dev->info("Trying to load " + path + " ... ");
	sprites[name] = new Sprite();
	SDL_Surface *surface = IMG_Load(path.c_str());
	sprites[name]->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

	if (!sprites[name]->texture)
	{
		delete sprites[name];
		sprites[name] = nullptr;
		sprites.erase(name);
		dev->error(path + " - fail.");
	}

	dev->info(path + " - done.");
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
	SDL_FRect dst_rect = Rect::reScale(pos, size, scale);
	SDL_RenderCopyExF(Game::renderer, sprite.texture, &src_rect, &dst_rect, 0, NULL, (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

void Screen::deleteSprites()
{
	std::string path;
	dev->info("Deleting all sprites ...");
	for (auto &&sprite : sprites)
	{
		path = sprite.second->path;
		delete sprite.second;
		sprite.second = nullptr;
		if (sprite.second)
			dev->error(path + " - fail.");
		else
			dev->info(path + " - done.");
	}
}

void Screen::loadFont(const std::string &path, const int &size)
{
	dev->info("Trying to load " + path + " ... ");
	font_sizes[size] = TTF_OpenFont(path.c_str(), size);
	if (!font_sizes[size])
	{
		TTF_CloseFont(font_sizes[size]);
		font_sizes[size] = nullptr;
		font_sizes.erase(size);
		dev->error(path + " " + std::to_string(size) + "pt - fail.");
	}
	dev->info(path + " " + std::to_string(size) + "pt - done.");
}

SDL_Texture *Screen::loadText(const std::string &txt, const int &font_size, const int &option, SDL_Color txt_color, SDL_Color bg_color)
{
	SDL_Surface *surface = nullptr;
	switch (option)
	{
	case solid:
		surface = TTF_RenderUTF8_Solid(font_sizes[font_size], txt.c_str(), txt_color);
		break;
	case blended:
		surface = TTF_RenderUTF8_Blended(font_sizes[font_size], txt.c_str(), txt_color);
		break;
	case shaded:
		surface = TTF_RenderUTF8_Shaded(font_sizes[font_size], txt.c_str(), txt_color, bg_color);
		break;
	default:
		dev->error("invalid text option.");
		break;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

std::pair<int, int> Screen::drawText(SDL_Texture *texture, const Vec2D &pos)
{
	int w_txt_box, h_txt_box;
	SDL_QueryTexture(texture, nullptr, nullptr, &w_txt_box, &h_txt_box);
	SDL_FRect dst_rect = {pos.x, pos.y, float(w_txt_box), float(h_txt_box)};
	SDL_RenderCopyF(Game::renderer, texture, nullptr, &dst_rect);
	return {w_txt_box, h_txt_box};
}

void Screen::deleteFonts()
{
	dev->info("Deleting all fonts ...");
	for (auto &&font_size : font_sizes)
	{
		TTF_CloseFont(font_size.second);
		font_size.second = nullptr;
		if (font_size.second)
			dev->error("fail.");
		else
			dev->info("done.");
	}
}

void Screen::updateBackground()
{
}

void Screen::updateEnemies()
{
	if (levels.empty())
	{
		dev->log("end game!");
	}
	else
	{
		std::string name;
		Uint64 cur_time = SDL_GetTicks64();

		if (cur_time - Enemy::last_spawn_time >= Enemy::spawn_time) // spawn enemy
		{
			if (enemies.empty()) // new level
			{
				Game::level << levels.front();
				levels.pop();
			}
			if (Game::level >> name)
			{
				if (ispunct(name.front()))
					name.erase(name.begin());
				if (ispunct(name.back()))
					name.pop_back();
				std::replace(name.begin(), name.end(), '_', ' ');
				Enemy *new_enemy = new Enemy(name, "enemy" + std::to_string(rand() % 50 + 1));
				new_enemy->spawnNearTo(players[0]);
				enemies.emplace_back(new_enemy);
			}
			Enemy::last_spawn_time = cur_time;
		}
		for (int i = 0; i < enemies.size(); i++) // current displayed enemy
		{
			enemies[i]->showName();
			enemies[i]->move();
			enemies[i]->attack();
			enemies[i]->takeDamage();
		}
	}
}

void Screen::updatePlayer()
{
	if (players[0]->moving)
		players[0]->move();
	else
	{
		if (!enemies.empty())
		{
			if (Enemy::killed)
			{
				float cur_d, min_d = INT_MAX;
				for (int i = 0; i < enemies.size(); i++)
				{
					cur_d = Vec2D(enemies[i]->x, enemies[i]->y).distance(Vec2D(players[0]->x, players[0]->y));
					if (event->cur_txt_inp.front() == enemies[i]->name.front() && cur_d < min_d)
					{
						min_d = cur_d;
						Enemy::index = i;
					}
				}
				if (Enemy::index >= 0)
					Enemy::killed = false;
			}
			if (Enemy::index >= 0)
				players[0]->attack(enemies[Enemy::index]);
		}
	}
}

void Screen::drawBackground()
{
	drawSprite(
		*sprites["bg"],
		Vec2D(),
		Vec2D(3840, 2400),
		1, 1, false);
}

void Screen::drawEnemies()
{
	// 64x64: easy enemy, 96x96: medium enemy, 128x128: hard enemy
	for (int i = 0; i < enemies.size(); i++)
	{
		drawSprite(
			*sprites[enemies[i]->id],
			Vec2D(enemies[i]->x, enemies[i]->y),
			Vec2D(96, 96),
			1, 1, false);
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		std::pair<int, int> size = drawText(
			enemies[i]->txt_box_texture,
			Vec2D(enemies[i]->x_txt_box, enemies[i]->y_txt_box));
		enemies[i]->w_txt_box = size.first;
		enemies[i]->h_txt_box = size.second;

		dev->drawLine(Vec2D(players[0]->x, players[0]->y), Vec2D(enemies[i]->x, enemies[i]->y), (i != Enemy::index) ? Color::white(0) : Color::red(0));
	}
}

void Screen::drawPlayer()
{
	for (int i = 0; i < sprites["arrow"]->max_frame; i++)
		drawSprite(
			*sprites["arrow"],
			Vec2D(players[0]->x, players[0]->y),
			Vec2D(64, 64),
			1, i, false);
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