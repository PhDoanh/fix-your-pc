#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "util.hpp"

#ifndef SCREEN_HPP
#define SCREEN_HPP

enum text_options
{
	solid,
	blended,
	shaded,
};

struct Sprite
{
	SDL_Texture *texture;
	std::string path;
	Vec2D real_size;
	int max_frame;
};

struct Text
{
	SDL_Texture *texture;
	std::string txt;
};

class Screen
{
private:
	SDL_Texture *txt_texture;
	std::string prev_txt;

public:
	Screen();
	~Screen();

	// image
	void loadSprite(const std::string &, const std::string &, Vec2D, int max_frame = 1);
	void drawSprite(Sprite &, const Vec2D &, const Vec2D &, float, int, bool);
	void deleteSprites();

	// text
	void loadFont(const std::string &, const int &);
	SDL_Texture *loadText(const std::string &, const int &font_size = 18, const int &option = shaded, SDL_Color txt_color = Color::white(0), SDL_Color bg_color = Color::black(0));
	Vec2D drawText(SDL_Texture *, const Vec2D &);
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
extern std::map<int, TTF_Font *> font_sizes;

#endif // SCREEN_HPP