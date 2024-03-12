#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "Sound.hpp"

#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player
{
private:
protected:
public:
	float speed = 8.5;
	int score = 0;
	static SDL_Rect rect;
	Player();  // constructor
	~Player(); // destructor

	void updatePosition(SDL_MouseMotionEvent); // a member function
	void move();
};

extern Player player;

#endif // PLAYER_HPP