#include <sstream>
#include <queue>
#include <string>
#include <regex>
#include "util.hpp"

#ifndef LEVEL_HPP
#define LEVEL_HPP

class Level
{
public:
	static std::stringstream lv;

	Level();  // constructor
	~Level(); // destructor

	void newLevel();
	void spawnEnemy();

	// utils
	std::string processStr(const std::string &); // a member function
};

extern Level *level;
extern std::queue<std::string> lvs;

#endif // LEVEL_HPP