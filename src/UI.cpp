#include "UI.hpp"
#include "Game.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Entity.hpp"

UI::UI()
{
	info("UI constructor called.");
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
	this->bg_pos = Vec2D(-100);
	this->bg_size = Vec2D(Game::win_w, Game::win_h) + Vec2D(200);
}

UI::~UI()
{
	info("UI destructor called.");
}

void UI::updateBackground()
{
	bg_pos.x = -players[0]->pos.x * 200 / float(Game::win_w);
	bg_pos.y = -players[0]->pos.y * 200 / float(Game::win_h);
}

void UI::drawBackground()
{
	screen->drawSprite(*sprites["full"], bg_pos, bg_size, 1, 1, false);
}

void UI::drawMenu()
{
}

void UI::drawSetting()
{
}