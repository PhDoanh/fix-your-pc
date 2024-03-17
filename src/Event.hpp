#include <bits/stdc++.h>
#include "../inc/SDL.h"

#ifndef EVENT_HPP
#define EVENT_HPP

class Event
{
private:
public:
	static SDL_Event e;
	static const Uint8 *state;

	Event(/* args */);
	~Event();

	void handleKeyboard();
	void handleMouse();
};

#endif // EVENT_HPP