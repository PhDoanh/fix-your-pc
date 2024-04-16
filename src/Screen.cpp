#include "Game.hpp"
#include "Screen.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "Level.hpp"
#include "UI.hpp"

std::map<std::string, Sprite *> sprites;
std::map<std::string, Text *> texts;
std::map<std::string, TTF_Font *> fonts;

Screen::Screen() { info("Screen constructor called!\n"); }

Screen::~Screen() { info("Screen destructor called!\n"); }

void Screen::loadSprite(const std::string &name, const std::string &path, Vec2D real_size, int max_frame)
{
	info("Trying to load " + path + " ... \n");
	sprites[name] = new Sprite();
	SDL_Surface *surface = IMG_Load(path.c_str());
	sprites[name]->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

	if (!sprites[name]->texture)
	{
		delete sprites[name];
		sprites[name] = nullptr;
		sprites.erase(name);
		error(path + " - fail.\n");
	}

	info(path + " - done.\n");
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
	info("Deleting all sprites ...\n");
	for (auto &&sprite : sprites)
	{
		path = sprite.second->path;
		delete sprite.second;
		sprite.second = nullptr;
		if (sprite.second)
			error(path + " - fail.\n");
		else
			info(path + " - done.\n");
	}
}

void Screen::loadFont(const std::string &name, const std::string &path, const std::initializer_list<int> &sizes)
{
	info("Trying to load " + path + " ... ");
	for (auto &&size : sizes)
	{
		std::string alias = name + std::to_string(size);
		fonts[alias] = TTF_OpenFont(path.c_str(), size);
		if (!fonts[alias])
		{
			TTF_CloseFont(fonts[alias]);
			fonts[alias] = nullptr;
			fonts.erase(alias);
			error("fail.\n");
		}
	}
	info("done.\n");
}

Vec2D Screen::drawText(const std::string &txt, const Vec2D &pos, const int &font_size, const std::string &font_name, const SDL_Color &txt_color, const SDL_Color &bg_color)
{
	if (texts.find(txt) == texts.end())
		texts[txt] = new Text();
	if (txt != texts[txt]->prev_txt)
	{
		SDL_Surface *surface = nullptr;
		// if ()
		// 	surface = TTF_RenderUTF8_Solid(fonts[font_name + std::to_string(font_size)], txt.c_str(), txt_color);
		// else
		surface = TTF_RenderUTF8_Shaded(fonts[font_name + std::to_string(font_size)], txt.c_str(), txt_color, bg_color);
		texts[txt]->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);
		texts[txt]->prev_txt = txt;
	}
	int w_txt_box, h_txt_box;
	SDL_QueryTexture(texts[txt]->texture, nullptr, nullptr, &w_txt_box, &h_txt_box);
	SDL_FRect dst_rect = {pos.x, pos.y, float(w_txt_box), float(h_txt_box)};
	SDL_RenderCopyF(Game::renderer, texts[txt]->texture, nullptr, &dst_rect);
	return Vec2D(w_txt_box, h_txt_box);
}

void Screen::deleteTexts()
{
	info("Deleting all texts ... ");
	for (auto &&text : texts)
	{
		delete text.second;
		text.second = nullptr;
		if (text.second)
			error("fail.\n");
	}
	info("done.\n");
}

void Screen::deleteFonts()
{
	info("Deleting all fonts ... ");
	for (auto &&font : fonts)
	{
		TTF_CloseFont(font.second);
		font.second = nullptr;
		if (font.second)
			error("fail.\n");
	}
	info("done.\n");
}

void Screen::updateUI()
{
	ui->updateBackground();
}

void Screen::updateEnemies()
{
	if (lvs.empty())
	{
		log("end game!\n");
	}
	else
	{
		Uint64 cur_time = SDL_GetTicks64();
		if (cur_time - Enemy::last_spawn_time >= Enemy::spawn_time) // spawn enemy per 3s
		{
			std::clog << enemies.size() << '\n';
			if (enemies.empty()) // new level
				level->newLevel();
			level->spawnEnemy();
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
					cur_d = enemies[i]->pos.distance(players[0]->pos);
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

void Screen::drawUI()
{
	ui->drawBackground();
}

void Screen::drawEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		drawSprite(
			*sprites[enemies[i]->id],
			enemies[i]->pos,
			enemies[i]->size,
			1, 1, false);
		enemies[i]->name_size = drawText(enemies[i]->name, enemies[i]->name_pos);
		drawLine(players[0]->pos, enemies[i]->pos, (i != Enemy::index) ? Color::white(0) : Color::red(0));
	}
}

void Screen::drawPlayer()
{
	drawSprite(
		*sprites["arrow"],
		players[0]->pos,
		players[0]->size,
		1, 1, false);
}