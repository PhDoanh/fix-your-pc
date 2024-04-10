#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <cstdlib>
#include "../inc/SDL.h"
#include "util.hpp"

#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity
{
public:
	std::string id;
	int x, y;
	float speed;
	Entity(std::string id, int x, int y, float speed) : id(id), x(x), y(y), speed(speed) {}
};

class Player : public Entity
{
private:
	int score;
	int health;

public:
	Player(std::string id, int x = 0, int y = 0, float speed = 4.13, int score = 0, int health = 100) : Entity(id, x, y, speed), score(score), health(health) {}

	void move();
	void attack(const int &);
	void takeDamage();
};

class Enemy : public Entity
{
private:
public:
	static int count;
	static int name_index;
	static const Uint64 spawn_time;
	static Uint64 last_spawn_time;
	SDL_Texture *txt_box_texture;
	int x_txt_box, y_txt_box;
	int w_txt_box, h_txt_box;
	std::string name, prev_name;

	Enemy(std::string name, std::string id, int x = 0, int y = 0, float speed = rand() % 3 + 1) : name(name), Entity(id, x, y, speed) { count++; }

	void showName();
	void move();
	void attack();
	void takeDamage();
	void spawnNearTo(Player *);
};

extern std::vector<Player *> players;
extern std::vector<Enemy *> enemies;

#endif // ENTITY_HPP