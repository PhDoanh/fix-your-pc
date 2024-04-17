#include "Level.hpp"
#include "Game.hpp"
#include "Entity.hpp"
#include "UI.hpp"

Level::Level()
{
	info("Level constructor called.\n");
}

Level::~Level()
{
	info("Level destructor called.\n");
}

// void Level::newLevel()
// {
// }

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
		{
			lv = lvs.front();
			lvs.pop();
		}
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
