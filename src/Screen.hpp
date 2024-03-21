#include <map>
#include <string>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "util.hpp"

#ifndef SCREEN_HPP
#define SCREEN_HPP

enum txt_types
{
	solid,
	blended,
	shaded
};

struct Sprite
{
	SDL_Texture *texture;
	std::string path;
	Vec2D real_size;
	int max_frame;
};

struct Font
{
	std::string path;
	TTF_Font *font;
	int w, h;
};

class Screen
{
private:
	int tilemap[16][8] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 0},
		{1, 1, 1, 0, 1, 1, 0, 0},
		{1, 1, 1, 1, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1}};
	std::string bg_name;

public:
	Screen();
	~Screen();

	// image
	void loadSprite(const std::string &, const std::string &, Vec2D, int max_frame = 1);
	void drawSprite(Sprite &, const Vec2D &, const Vec2D &, float, int, bool);
	void deleteSprites();

	// font
	void loadFont(const std::string &, const std::string &, const int &);
	void renderFont(Font &, const std::string &, const Vec2D &, int txt_type = shaded, SDL_Color txt_color = Color::white(0), SDL_Color bg_color = Color::black(0.5));
	void deleteFonts();

	void updateEnemies();
	void updatePlayer();

	void drawBackground();
	void drawEnemies();
	void drawPlayer();
	// void drawDialog();
};

extern Screen *screen;
extern std::map<std::string, Sprite *> sprites;
extern std::map<std::string, Font *> fonts;

#endif // SCREEN_HPP