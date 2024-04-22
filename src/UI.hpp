#include <map>
#include <vector>
#include <string>
#include "../inc/SDL.h"
#include "util.hpp"

#ifndef UI_HPP
#define UI_HPP

class UI
{
private:
	Vec2D goal_bg_pos, bg_pos, bg_size;	   // background 1
	Vec2D goal_bg2_pos, bg2_pos, bg2_size; // background 2
	Vec2D goal_fg_pos, fg_pos, fg_size;	   // foreground

	Vec2D ava_pos, ava_size; // avatar

	SDL_Color opb_color, ipb_color;
	SDL_FRect outer_pass_box, inner_pass_box; // password

	Uint64 last_render_time, render_time, last_trans_time, trans_time;
	std::vector<std::string> dynamic_txt;
	std::string cur_txt;
	Vec2D cur_txt_pos;
	int i, j;

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

public:
	UI();  // constructor
	~UI(); // destructor

	Vec2D getPassBoxPos() const;
	Vec2D getPassBoxSize() const;
	SDL_Color getPassBoxBorderColor() const;
	SDL_Color getPassBoxColor() const;
	void setPassBoxBorderColor(const SDL_Color &color = Color::ice_blue(255));
	void setPassBoxColor(const SDL_Color &color = Color::ice_blue(255));
	void setDynamicText(const std::vector<std::string> &);

	void updateGamePlay();
	void drawGamePlay();
	void updateMenu();
	void drawMenu();
	void updateSetting();
	void drawSetting();
	void updateGameOver();
	void drawGameOver();

	void drawDynamicText(const Uint64 &delay_per_chr = 50, const Uint64 &delay_per_str = 2000);
	void updateBackground();
	void drawBackground();
	void updateEnemies();
	void drawEnemies();
	void updatePlayer();
	void drawPlayer();
};

extern UI *ui;
extern std::map<std::string, int> settings;
extern std::multimap<int, std::string> high_scores;

#endif // UI_HPP