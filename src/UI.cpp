#include "UI.hpp"
#include "Game.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Entity.hpp"

UI::UI()
{
	info("UI constructor called.\n");
	// std::vector<int> indexes(50);
	// std::iota(indexes.begin(), indexes.end(), 1);
	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 8; y++)
	// 	{
	// 		if (tilemap[x][y])
	// 		{
	// 			tilemap[x][y] = indexes[rand() % indexes.size()];
	// 			indexes.erase(std::find(indexes.begin(), indexes.end(), tilemap[x][y]));
	// 		}
	// 	}

	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 8; y++)
	// 	{
	// 		int index = tilemap[x][y];
	// 		if (index)
	// 		{
	// 			enemy[index - 1].rect.x = x * 96;
	// 			enemy[index - 1].rect.y = y * 96;
	// 		}
	// 	}
	this->fg_pos = Vec2D(-100);
	this->fg_size = Vec2D(Game::win_w, Game::win_h) + Vec2D(200);
	this->bg_pos = this->bg2_pos = Vec2D(-1280, -1616);
	this->bg_size = this->bg2_size = Vec2D(4096);
}

UI::~UI()
{
	info("UI destructor called.\n");
}

void UI::updateBackground()
{
	goal_bg_pos.x = goal_bg2_pos.x = fg_pos.x = -players[0]->pos.x * 200 / float(Game::win_w);
	goal_bg_pos.y = goal_bg2_pos.y = fg_pos.y = -players[0]->pos.y * 200 / float(Game::win_h);
	bg_pos.x = lerp(goal_bg_pos.x, bg_pos.x, Game::deltaTime * 10);
	bg_pos.y = lerp(goal_bg_pos.y, bg_pos.y, Game::deltaTime * 10);
	bg2_pos.x = lerp(goal_bg2_pos.x, bg2_pos.x, Game::deltaTime * 20);
	bg2_pos.y = lerp(goal_bg2_pos.y, bg2_pos.y, Game::deltaTime * 20);
}

void UI::drawBackground()
{
	screen->drawSprite(*sprites["space"], bg_pos, bg_size, 1, 1, false);
	screen->drawSprite(*sprites["stars"], bg2_pos, bg2_size, 1, 1, false);
	screen->drawSprite(*sprites["flower"], fg_pos, fg_size, 1, 1, false);
}

void UI::drawMenu()
{
}

void UI::drawSetting()
{
}