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
	Screen();  // constructor
	~Screen(); // destructor

	// image
	void loadSprite(const std::string &, const std::string &, Vector, int max_frame = 1);
	void showSprite(const std::string &);
	void deleteSprite();

	// font
};

extern std::map<std::string, Sprite *> sprites;
extern Screen screen;

#endif // SCREEN_HPP