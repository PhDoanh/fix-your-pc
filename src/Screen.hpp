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
	SDL_Color prev_txt_color;
};

enum aligns
{
	top_left,
	top_mid,
	top_right,
	left,
	center,
	right,
	bottom_left,
	bottom_mid,
	bottom_right
};

class Screen
{
public:
	Vec2D size;

	Screen();
	~Screen();

	// image
	void loadSprite(const std::string &, const std::string &, Vec2D, int max_frame = 1, int max_layer = 1);
	void drawSprite(Sprite &, const Vec2D &, const Vec2D &, const float &scale = 1, const int &align = top_left, const int &cur_frame = 0, const int &cur_layer = 0, const double &angle = 0, const SDL_FPoint *center_pos = nullptr, const bool &flip = false);
	void deleteSprites();

	// text
	void loadFont(const std::string &, const std::string &, const std::initializer_list<int> &);
	SDL_FRect drawText(const std::string &, const Vec2D &, const int &align = top_left, const int &font_size = 18, const std::string &font_name = "ui", const bool &shaded_mode = false, const SDL_Color &txt_color = Color::white(255), const SDL_Color &bg_color = Color::black(255));
	void deleteTexts();
	void deleteFonts();

	void update();
	void draw();
};

extern Screen *screen;
extern std::map<std::string, Sprite *> sprites;
extern std::map<std::string, TTF_Font *> fonts;
extern std::map<std::string, Text *> texts;

#endif // SCREEN_HPP