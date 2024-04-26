#include "Level.hpp"
#include "Game.hpp"
#include "Entity.hpp"
#include "UI.hpp"
#include "Sound.hpp"

int Level::level_order = 1;

Level::Level()
{
	info("Level constructor called.\n");
}

Level::~Level()
{
	info("Level destructor called.\n");
}

void Level::newLevel()
{
	if (level_order > 1)
	{
		sound->playSoundEffect("spawn", general);
		if (Enemy::spawn_time - 500 > 0)
			Enemy::spawn_time -= 125;
	}
	std::vector<std::string> dt;
	if (level_order == lvs.size())
		dt = {"Final level ", ""};
	else
		dt = {"Level " + std::to_string(level_order) + " ", ""};
	ui->setDynamicText(dt);
	lv = lvs.front();
	lvs.pop();
	level_order++;
}

void Level::spawnEnemyPer(const int &spawn_time)
{
	Uint64 cur_time = SDL_GetTicks64();
	if (cur_time - Enemy::last_spawn_time >= spawn_time) // spawn enemy per "time"
	{
		std::string name, id;
		Vec2D pos, size, speed;
		if (!lv.empty()) // spawn enemy on current level
		{
			name = lv.front();
			id = "enemy" + std::to_string(rand() % 50 + 1);
			name = processStr(name);
			if (name.size() == 1) // trash mobs (char)
			{
				size = mini;
				speed = 4;
			}
			else if (name.size() <= 6) // grunts (word)
			{
				size = small;
				speed = 2;
			}
			else if (name.size() <= 17) // mini boss (long word)
			{
				size = medium;
				speed = 1.5;
			}
			else if (name.size() <= 2000) // boss (sentence)
			{
				size = big;
				speed = 1;
			}
			else // final boss (paragraph)
			{
				size = extra;
				speed = 0.5;
			}
			Enemy *new_enemy = new Enemy(name, id, pos, size, speed);
			new_enemy->spawnNearTo(player);
			enemies.emplace_back(new_enemy);
			lv.pop();
		}
		else // new level
			newLevel();
		Enemy::last_spawn_time = cur_time;
	}
}

std::string Level::processStr(const std::string &str)
{
	std::string tmp, result;
	for (int i = 0; i < str.size(); i++)
	{
		if (isdigit(str[i]))
		{
			if (settings["enableNum"])
				tmp += str[i];
		}
		else if (ispunct(str[i]))
		{
			if (settings["enablePunct"])
				tmp += str[i];
			else if (str[i] == '_')
				tmp += ' ';
		}
		else
			tmp += str[i];
	}
	std::stringstream ss(tmp);
	while (ss >> tmp)
		result += tmp + " ";
	result.pop_back();
	return result;
}
