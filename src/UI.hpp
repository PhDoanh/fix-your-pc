#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <numeric>
#include "../inc/SDL.h"
#include "util.hpp"

#ifndef UI_HPP
#define UI_HPP

struct TextElement // using both text and img
{
	std::string text;
	int font_size;
	int align;
	Vec2D pos;
	Vec2D size;
	Vec2D real_pos;
	SDL_Color color;
	TextElement(std::string text, int font_size, int align, Vec2D pos = Vec2D(), Vec2D size = Vec2D(), SDL_Color color = Color::white(255))
		: text(text), font_size(font_size), align(align), pos(pos), size(size), color(color) {}
};

class UI
{
private:
	int i, j;
	Vec2D cur_txt_pos;
	std::string cur_txt;
	std::vector<std::string> dynamic_txt;
	Uint64 last_render_time, render_time, last_trans_time, trans_time;

	Vec2D goal_bg_pos, bg_pos, bg_size;	   // background 1
	Vec2D goal_bg2_pos, bg2_pos, bg2_size; // background 2
	Vec2D goal_fg_pos, fg_pos, fg_size;	   // foreground

	// game ready
	Vec2D ava_pos, ava_size;
	SDL_Color opb_color, ipb_color;
	SDL_FRect outer_pass_box, inner_pass_box;

	// game start
	std::vector<std::vector<int>> tilemap;

	// game pause
	int order;
	SDL_FRect pause_bg;
	SDL_Texture *saved_screen;
	Vec2D layout_pos, layout_size;
	Vec2D margin, num_of_cells, cell_size;

	// game over
	int count_down_time;

public:
	std::map<std::string, TextElement *> options;

	UI();  // constructor
	~UI(); // destructor

	void loadElements();
	void deleteElements();

	Vec2D getPassBoxPos() const;
	Vec2D getPassBoxSize() const;
	SDL_Color getPassBoxBorderColor() const;
	SDL_Color getPassBoxColor() const;
	void setPassBoxBorderColor(const SDL_Color &color = Color::ice_blue(255));
	void setPassBoxColor(const SDL_Color &color = Color::ice_blue(255));
	void setDynamicText(const std::vector<std::string> &);
	void setShutdownTime(const int &);

	void updateGameReady();
	void drawGameReady();
	void updateGameStart();
	void drawGameStart();
	void updateGamePlay();
	void drawGamePlay();
	void updateGamePause();
	void drawGamePause();
	void updateGameOver();
	void drawGameOver();

	void drawDynamicText(const Uint64 &delay_per_chr = 50, const Uint64 &delay_per_str = 2000);
	void updateBackground();
	void drawBackground();
	void updateEnemies();
	void drawEnemies();
	void updatePlayer();
	void drawPlayer();
	void saveCurScreen();
	void drawHighScores();
};

extern UI *ui;
extern std::map<std::string, int> settings;
extern std::multimap<int, std::string> high_scores;

#endif // UI_HPP