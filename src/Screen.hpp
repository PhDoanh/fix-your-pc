#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include "Utility.hpp"

#ifndef SCREEN_HPP
#define SCREEN_HPP

struct Sprite
{
	SDL_Texture *texture;
	std::string path;
	Vector real_size;
	int max_frame;
};

class Screen // Player Screen
{

public:
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
	SDL_Rect tile[16][8];

	Screen();  // constructor
	~Screen(); // destructor

	// image
	void loadSprite(const std::string &, const std::string &, Vector, int max_frame = 1);
	void drawSprite(Sprite &, const Vector &, const Vector &, float, int, bool);
	void deleteSprite();
	void drawTileMap();
	// font
};

extern std::map<std::string, Sprite *> sprites;
extern Screen screen;

#endif // SCREEN_HPP