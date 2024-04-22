#include "Game.hpp"
#include "Screen.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "Level.hpp"
#include "UI.hpp"

std::map<std::string, Sprite *> sprites;
std::map<std::string, Text *> texts;
std::map<std::string, TTF_Font *> fonts;

Screen::Screen()
{
	info("Screen constructor called!\n");
	size = Vec2D(Game::win_w, Game::win_h);
}

Screen::~Screen() { info("Screen destructor called!\n"); }

void Screen::loadSprite(const std::string &name, const std::string &path, Vec2D real_size, int max_frame, int max_layer)
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
	sprites[name]->max_layer = max_layer;

	SDL_FreeSurface(surface);
}

void Screen::drawSprite(Sprite &sprite, const Vec2D &pos, const Vec2D &size, const float &scale, const int &cur_frame, const int &cur_layer, const double &angle, const SDL_FPoint *center, const bool &flip)
{
	int x = (cur_frame % sprite.max_frame) * sprite.real_size.x;
	int y = (cur_layer % sprite.max_layer) * sprite.real_size.y;
	int w = sprite.real_size.x;
	int h = sprite.real_size.y;
	SDL_Rect src_rect = {x, y, w, h};
	SDL_FRect dst_rect = Rect::reScale(pos, size, scale);
	SDL_RenderCopyExF(Game::renderer, sprite.texture, &src_rect, &dst_rect, angle, center, (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
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

Vec2D Screen::drawText(const std::string &txt, const Vec2D &pos, const bool &center_pos, const int &font_size, const std::string &font_name, const bool &shaded_mode, const SDL_Color &txt_color, const SDL_Color &bg_color)
{
	if (texts.find(txt) == texts.end())
		texts[txt] = new Text();
	if (txt != texts[txt]->prev_txt)
	{
		SDL_Surface *surface = nullptr;
		if (shaded_mode)
			surface = TTF_RenderUTF8_Shaded(fonts[font_name + std::to_string(font_size)], txt.c_str(), txt_color, bg_color);
		else
			surface = TTF_RenderUTF8_Blended(fonts[font_name + std::to_string(font_size)], txt.c_str(), txt_color);
		texts[txt]->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);
		texts[txt]->prev_txt = txt;
	}
	int w_txt_box, h_txt_box;
	SDL_QueryTexture(texts[txt]->texture, nullptr, nullptr, &w_txt_box, &h_txt_box);
	SDL_FRect dst_rect;
	if (center_pos)
		dst_rect = {pos.x - float(w_txt_box / 2.0), pos.y - float(h_txt_box / 2.0), float(w_txt_box), float(h_txt_box)};
	else
		dst_rect = {pos.x, pos.y, float(w_txt_box), float(h_txt_box)};
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

void Screen::update()
{
	switch (Game::state)
	{
	case start:
		ui->updateMenu();
		break;
	case play:
		ui->updateGamePlay();
		break;
	case pause:
		ui->updateSetting();
		break;
	case over:
		ui->updateGameOver();
		break;
	default:
		break;
	}
}

void Screen::draw()
{
	switch (Game::state)
	{
	case start:
		ui->drawMenu();
		break;
	case play:
		ui->drawGamePlay();
		break;
	case pause:
		ui->drawSetting();
		break;
	case over:
		ui->drawGameOver();
		break;
	default:
		break;
	}
}