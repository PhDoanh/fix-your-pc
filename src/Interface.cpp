#include "Interface.hpp"
#include "Console.hpp"
#include "GameInit.hpp"

SDL_Surface *Interface::bg_surface = nullptr;
SDL_Texture *Interface::bg_texture = nullptr;
SDL_Surface *Interface::ava_surface = nullptr;
SDL_Texture *Interface::ava_texture = nullptr;
TTF_Font *Interface::font = nullptr;
std::string Interface::txt_inp = PLACEHOLDER;
bool Interface::is_txt_entered = false;

Interface::Interface()
{
}

Interface::~Interface()
{
	SDL_StopTextInput();
	TTF_CloseFont(font);
	SDL_DestroyTexture(ava_texture);
	SDL_DestroyTexture(bg_texture);
}

void Interface::loadMenu()
{
	std::string full_path;
	std::vector<std::string> bg_paths;
	for (int i = 1; i <= NUM_BG_PATHS; i++)
	{
		full_path = pref_paths[0] + "bg" + std::to_string(i) + "_blur" + exts[0];
		bg_paths.push_back(full_path);
	}
	int i = rand() % NUM_BG_PATHS;

	// load menu
	loadBackground(bg_paths[i]);
	loadAvatar();
	loadInputBar();
	loadOptions();
}

void Interface::loadBackground(const std::string &path)
{
	bg_surface = IMG_Load(path.c_str());
	if (bg_surface == nullptr)
		console.error();
	bg_texture = SDL_CreateTextureFromSurface(GameInit::renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	if (bg_texture == nullptr)
		console.error();

	if (SDL_RenderCopy(GameInit::renderer, bg_texture, nullptr, nullptr) != 0)
		console.error();
	SDL_RenderPresent(GameInit::renderer);
}

void Interface::loadAvatar(const std::string &path)
{
	int ava_width, ava_height;

	ava_surface = IMG_Load(path.c_str());
	if (ava_surface == nullptr)
		console.error();
	ava_texture = SDL_CreateTextureFromSurface(GameInit::renderer, ava_surface);
	SDL_FreeSurface(ava_surface);
	if (ava_texture == nullptr)
		console.error();

	if (SDL_QueryTexture(ava_texture, NULL, NULL, &ava_width, &ava_height) != 0)
		console.error();
	int x = (GameInit::win_width - ava_width) / 2;
	int y = (GameInit::win_height - ava_height) / 3;

	SDL_Rect dstRect = {x, y, ava_width, ava_height};

	if (SDL_RenderCopy(GameInit::renderer, ava_texture, nullptr, &dstRect) != 0)
		console.error();
	SDL_RenderPresent(GameInit::renderer);
}

void Interface::loadInputBar()
{
	font = TTF_OpenFont("res/fonts/Sarabun-Light.ttf", STANDARD_FONT_SIZE);
	if (font == nullptr)
		console.error();
	SDL_StartTextInput();
}

void Interface::showInputBar()
{
	SDL_Surface *txt_surface = TTF_RenderText_Solid(font, txt_inp.c_str(), {63, 64, 81});
	SDL_Texture *txt_texture = SDL_CreateTextureFromSurface(GameInit::renderer, txt_surface);
	SDL_FreeSurface(txt_surface);

	int txt_width, txt_height;
	SDL_QueryTexture(txt_texture, nullptr, nullptr, &txt_width, &txt_height);
	int x = (GameInit::win_width - txt_width) / 2;
	int y = (GameInit::win_height - txt_height) / 2;
	SDL_Rect txt_rect = {x, y, txt_width, txt_height};

	SDL_RenderCopy(GameInit::renderer, txt_texture, nullptr, &txt_rect);
	SDL_DestroyTexture(txt_texture);
	SDL_RenderPresent(GameInit::renderer);
}

void Interface::loadOptions()
{
}

void Interface::showOptions()
{
}