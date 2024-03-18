#include <bits/stdc++.h>
#include "../inc/SDL.h"

#ifndef ENEMIES_HPP
#define ENEMIES_HPP

enum motion_type
{
	chase, // default
	bounce,
	parabol,
	circle,
	random
};

struct Enemy
{
	SDL_Rect rect;
	int speed_x, speed_y;
};

class Enemies
{
public:
	Enemies();	// constructor
	~Enemies(); // destructor

	void spawn();
	void move();
	void chaseMotion(Enemy &);
	void bounceMotion(Enemy &);
	void parabolMotion(Enemy &);
	void circleMotion(Enemy &);
	void randomMotion(Enemy &);
};

extern std::vector<Enemy> enemy;

#endif // ENEMIES_HPP