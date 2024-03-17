#include <bits/stdc++.h>
#include "../inc/SDL.h"

#ifndef ENEMY_HPP
#define ENEMY_HPP

struct Enemy
{
	int x, y, w, h;
	int vx = 2, vy = 3;
};

class Enemies
{
public:
	Enemies();	// constructor
	~Enemies(); // destructor

	// void updatePosition();
	void bounceMotion(int);
	void parabolicMotion();
	void randomMotion();
	void circleMotion();
};

extern std::map<int, Enemy *> enemy;

#endif // ENEMY_HPP