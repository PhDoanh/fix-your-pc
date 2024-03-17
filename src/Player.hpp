#include <bits/stdc++.h>
#include "../inc/SDL.h"

#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player
{
private:
protected:
public:
	float speed = 8.5;
	int score = 0;
	int x, y, w = 64, h = 64;

	Player();  // constructor
	~Player(); // destructor

	void update();
};

#endif // PLAYER_HPP