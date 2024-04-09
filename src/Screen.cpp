#include "Game.hpp"
#include "Screen.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "Background.hpp"

std::map<std::string, Sprite *> sprites;

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

SDL_Texture *Screen::loadText(const std::string &txt, TTF_Font *font, const int &option, SDL_Color txt_color, SDL_Color bg_color)
{
	SDL_Surface *surface = nullptr;
	switch (option)
	{
	case solid:
		surface = TTF_RenderText_Solid(font, txt.c_str(), txt_color);
		break;
	case blended:
		surface = TTF_RenderText_Blended(font, txt.c_str(), txt_color);
		break;
	case shaded:
		surface = TTF_RenderText_Shaded(font, txt.c_str(), txt_color, bg_color);
		break;
	default:
		error("invalid text option.");
		break;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void Screen::renderText(SDL_Texture *texture, const Vec2D &pos)
{
	int w_txt_box, h_txt_box;
	SDL_QueryTexture(texture, nullptr, nullptr, &w_txt_box, &h_txt_box);
	SDL_Rect dst_rect = {int(pos.x), int(pos.y), w_txt_box, h_txt_box};
	SDL_RenderCopy(Game::renderer, texture, nullptr, &dst_rect);
}

void Screen::updateBackground()
{
	bg->move();
}

void Screen::updateEnemies()
{
	if (levels.empty())
	{
		// end game
	}
	else
	{
		std::string word;
		Uint64 cur_time = SDL_GetTicks64();

		if (cur_time - Enemy::last_spawn_time >= Enemy::spawn_time) // spawn enemy
		{
			if (enemies.empty()) // new cur_level
			{
				Game::cur_level << levels.front();
				levels.pop();
			}
			if (Game::cur_level >> word)
			{
				Enemy *new_enemy = new Enemy(word, "enemy" + std::to_string(rand() % 50 + 1));
				new_enemy->spawnNearTo(players[0]);
				enemies.emplace_back(new_enemy);
			}
			Enemy::last_spawn_time = cur_time;
		}
		for (int i = 0; i < enemies.size(); i++) // current displayed enemy
		{
			enemies[i]->move();
			enemies[i]->attack();
			enemies[i]->takeDamage();
		}
	}
}

void Screen::updatePlayer()
{
	players[0]->move();
	// players[0]->attack(0);
}

void Screen::drawBackground()
{
	drawSprite(
		*sprites["bg"],
		Vec2D(),
		Vec2D(Game::win_w, Game::win_h),
		1, 1, false);
}

void Screen::drawEnemies()
{
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
		renderText(
			enemies[i]->texture,
			Vec2D(enemies[i]->x, enemies[i]->y + 96));
	}
}

void Screen::drawPlayer()
{
	for (int i = 1; i <= sprites["arrow"]->max_frame; i++)
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