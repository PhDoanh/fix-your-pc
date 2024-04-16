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

enum entity_size
{
	extra = 256,
	big = 128,
	medium = 96,
	small = 64,
	mini = 32
};

class Entity
{
public:
	std::string id;
	Vec2D pos; // position
	Vec2D size;
	Vec2D vel; // velocity
	Vec2D goal_vel;
	Vec2D speed; // max velocity
	int health;

	// Entity();
	Entity(const std::string &, const Vec2D &, const Vec2D &, const Vec2D &, const int &);
	~Entity();
};

class Player : public Entity
{
private:
	int score;

public:
	bool moving;
	Player(const std::string &, const Vec2D &, const Vec2D &size = Vec2D(small), const Vec2D &speed = Vec2D(6), const int &health = 5);
	~Player();

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

	std::string name;
	Vec2D name_pos;
	Vec2D name_size;

	Enemy(const std::string &, const std::string &, const Vec2D &pos, const Vec2D &size, const Vec2D &speed);
	~Enemy();

	void showName();
	void move();
	void attack();
	void takeDamage();
	void spawnNearTo(Player *);
};

extern std::vector<Player *> players;
extern std::vector<Enemy *> enemies;

#endif // ENTITY_HPP