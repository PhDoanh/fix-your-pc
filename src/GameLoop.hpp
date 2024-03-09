#include "../inc/SDL.h"

#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

class GameLoop
{
	bool running = true;

public:
	GameLoop();	  // constructor
	~GameLoop();  // destructor
	void start(); // a member function
};

#endif // GAME_LOOP_HPP