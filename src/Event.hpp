#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "Player.hpp"

#ifndef EVENT_HPP
#define EVENT_HPP

class Event
{
private:
protected:
public:
	static SDL_Event e;
	Event();  // constructor
	~Event(); // destructor

	void handleMouse();
	void handleKeyboard();
};

extern Event event;

#endif // EVENT_HPP