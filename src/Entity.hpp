#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <cstdlib>
#include "../inc/SDL.h"
#include "util.hpp"

#ifndef ENTITY_HPP
#define ENTITY_HPP

class Enemy;

class Entity
{
public:
	std::string id;
	float x, y;
	float speed;
	Entity(std::string id, float x, float y, float speed) : id(id), x(x), y(y), speed(speed) {}
};

class Player : public Entity
{
private:
	int score;
	int health;

public:
	bool moving;
	Player(std::string id, float x = 0.0, float y = 0.0, float speed = 5.0, int score = 0, int health = 100) : Entity(id, x, y, speed)
	{
		this->score = score;
		this->health = health;
		this->moving = false;
	}

	void move();
	void attack(Enemy *);
	void takeDamage();
};

class Enemy : public Entity
{
private:
public:
	static bool killed;
	static int count;
	static int index;
	static int name_index;
	static const Uint64 spawn_time;
	static Uint64 last_spawn_time;
	SDL_Texture *txt_box_texture;
	float x_txt_box, y_txt_box;
	int w_txt_box, h_txt_box;
	std::string name, prev_name;

	Enemy(std::string name, std::string id, float x = 0.0, float y = 0.0, float speed = 2) : Entity(id, x, y, speed)
	{
		this->name = name;
		count++;
	}

	void showName();
	void move();
	void attack();
	void takeDamage();
	void spawnNearTo(Player *);
};

extern std::vector<Player *> players;
extern std::vector<Enemy *> enemies;

#endif // ENTITY_HPP