#include "Level.hpp"
#include "Game.hpp"
#include "Entity.hpp"
#include "UI.hpp"

Level::Level()
{
	log("Level constructor called.");
}

Level::~Level()
{
	log("Level destructor called.");
}

void Level::newLevel()
{
	lv << lvs.front();
	lvs.pop();
}

void Level::spawnEnemy()
{
	std::string name;
	if (lv >> name)
	{
		// name = processStr(name);
		Enemy *new_enemy = new Enemy(name, "enemy" + std::to_string(rand() % 50 + 1));
		new_enemy->spawnNearTo(players[0]);
		enemies.emplace_back(new_enemy);
	}
}

std::string Level::processStr(const std::string &str)
{
	// will using regex in here
	return "";
}
