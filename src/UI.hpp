#include <map>
#include <string>
#include "util.hpp"

#ifndef UI_HPP
#define UI_HPP

class UI
{
private:
	Vec2D goal_bg_pos, bg_pos, bg_size;	   // background 1
	Vec2D goal_bg2_pos, bg2_pos, bg2_size; // background 2
	Vec2D fg_pos, fg_size;				   // foreground

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
	void updateBackground();
	void drawBackground();
	void drawMenu();
	void updateSetting();
	void drawSetting();
};

extern UI *ui;
extern std::map<std::string, int> settings;

#endif // UI_HPP