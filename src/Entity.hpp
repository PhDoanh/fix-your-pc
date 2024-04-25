#include <map>
#include <vector>
#include <string>
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
	Vec2D center_pos;
	Vec2D size;
	Vec2D vel; // velocity
	Vec2D goal_vel;
	Vec2D speed; // max velocity
	int health;
	bool killed; // bullet is killed when collide enemy
	double angle;
	double goal_angle;
	double delta_angle;

	Entity(const std::string &, const Vec2D &pos = Vec2D(), const Vec2D &size = Vec2D());  // static objects
	Entity(const Vec2D &, const Vec2D &, const Vec2D &);								   // dynamic objects
	Entity(const std::string &, const Vec2D &, const Vec2D &, const Vec2D &, const int &); // characters
	~Entity();
};

class Player : public Entity
{
private:
public:
	Time shield, shield_state;
	std::string state;
	int index; // using for nearest enemy index
	int score; // longest streak
	int true_chrs;	 // total true chars
	int wrong_chrs;	 // total true + false
	int num_of_chrs; // current streak
	int num_of_dead_zones;
	int cur_frame, cur_layer;
	int max_frame, max_layer;
	std::vector<Entity *> circles;
	std::vector<Entity *> bullets;
	std::vector<Entity *> dead_zones;

	Player(const std::string &, const Vec2D &, const Vec2D &size = Vec2D(mini), const Vec2D &speed = Vec2D(0), const int &health = 2);
	~Player();

	void move();
	void attackNearestEnemy();
	void makeCircleOn(Enemy *);
	void moveCircleOn(Enemy *);
	void shootBullet();
	void moveBulletTo(Enemy *);
	void addDeadZone();
	void releaseDeadZone();
	void updateRotation();
	void updateScore();
	void takeDamage();
};

class Enemy : public Entity
{
private:
public:
	static int count;
	static const Uint64 spawn_time;
	static Uint64 last_spawn_time;

	std::string name;
	int name_index;
	Vec2D name_pos;
	Vec2D name_size;
	SDL_Color name_color;

	Enemy(const std::string &, const std::string &, const Vec2D &pos, const Vec2D &size, const Vec2D &speed);
	~Enemy();

	void showName();
	void move();
	void attack(Player *);
	void takeDamage(const int&);
	void spawnNearTo(Player *);
};

extern Player *player;
extern std::vector<Enemy *> enemies;

#endif // ENTITY_HPP