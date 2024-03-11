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
	int x;
	int y;
	int w;
	int h;
	float speed = 0;
	int score = 0;
	static SDL_Rect player_rect;
	Player();  // constructor
	~Player(); // destructor

	void updatePosition(SDL_MouseMotionEvent); // a member function
};

extern Player player;

#endif // PLAYER_HPP