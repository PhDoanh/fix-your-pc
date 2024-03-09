#include "../inc/SDL.h"
#include "../inc/SDL_image.h"
#include "../inc/SDL_ttf.h"
#include <bits/stdc++.h>

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

const std::string PLACEHOLDER = "Enter your name";

class Interface
{
	std::vector<std::string> exts{".png", ".jpg", ".ttf", ".ico"};
	std::vector<std::string> pref_paths{
		"res/backgrounds/",
		"res/cursors/",
		"res/fonts/",
		"res/icons/"};

public:
	static SDL_Surface *bg_surface;
	static SDL_Texture *bg_texture;
	static SDL_Surface *ava_surface;
	static SDL_Texture *ava_texture;
	static TTF_Font *font;
	static std::string txt_inp;
	static bool is_txt_entered;

	Interface();  // constructor
	~Interface(); // destructor
	void loadMenu();
	void loadBackground(const std::string &); // a member function
	void loadAvatar(const std::string &path = "res/user.png");
	void loadInputBar();
	void showInputBar();
	void loadOptions();
	void showOptions();
};

#endif // INTERFACE_HPP
