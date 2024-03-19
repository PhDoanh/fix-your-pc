#include <vector>
#include <string>
#include <cmath>
#include "../inc/SDL.h"
#include "util.hpp"

#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity
{
public:
	std::string name;
	int x, y;
	float speed;
	Entity(std::string name, int x, int y, float speed) : name(name), x(x), y(y), speed(speed) {}
};

class Player : public Entity
{
private:
	int score;
	int health;

public:
	Player(std::string name, int x = 0, int y = 0, float speed = 8, int score = 0, int health = 100) : Entity(name, x, y, speed), score(score), health(health) {}

	void move();
	void attack();
	void takeDamage();
};

class Enemy : public Entity
{
private:
public:
	Enemy(std::string name, int x = 0, int y = 0, float speed = 1.69) : Entity(name, x, y, speed) {}

	void move();
	void attack();
	void takeDamage(); // using name for health
	void spawn();
};

extern std::vector<Player *> players;
extern std::vector<Enemy *> enemies;

#endif // ENTITY_HPP