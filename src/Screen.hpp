#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <initializer_list>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "util.hpp"

#ifndef SCREEN_HPP
#define SCREEN_HPP

struct Sprite
{
	SDL_Texture *texture;
	std::string path;
	Vec2D real_size;
	int max_frame;
	int max_layer;
};

struct Text
{
	SDL_Texture *texture;
	std::string prev_txt;
};

class Screen
{
public:
	Screen();
	~Screen();

	// image
	void loadSprite(const std::string &, const std::string &, Vec2D, int max_frame = 1, int max_layer = 1);
	void drawSprite(Sprite &, const Vec2D &, const Vec2D &, const float &scale = 1, const int &cur_frame = 0, const int &cur_layer = 0, const double &angle = 0, const SDL_FPoint *center = nullptr, const bool &flip = false);
	void deleteSprites();

	// text
	void loadFont(const std::string &, const std::string &, const std::initializer_list<int> &);
	Vec2D drawText(const std::string &, const Vec2D &, const int &font_size = 18, const std::string &font_name = "ui", const SDL_Color &txt_color = Color::white(0), const SDL_Color &bg_color = Color::black(0));
	void deleteTexts();
	void deleteFonts();

	void updateUI();
	void updateEnemies();
	void updatePlayer();

	void drawUI();
	void drawEnemies();
	void drawPlayer();
};

extern Screen *screen;
extern std::map<std::string, Sprite *> sprites;
extern std::map<std::string, TTF_Font *> fonts;
extern std::map<std::string, Text *> texts;

#endif // SCREEN_HPP