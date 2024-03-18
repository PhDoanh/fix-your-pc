#include <bits/stdc++.h>
#include "../inc/SDL.h"

#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player
{
private:
protected:
public:
	static float speed;
	static int score;
	static SDL_Rect rect;

	Player();  // constructor
	~Player(); // destructor

	void move();
};

#endif // PLAYER_HPP