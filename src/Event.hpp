#include "../inc/SDL.h"
#include "Screen.hpp"
#include "util.hpp"

#ifndef EVENT_HPP
#define EVENT_HPP

class Event
{
private:
public:
	SDL_Event e;
	const Uint8 *state;

	Event() { info("Event constructor called!"); }
	~Event() { info("Event destructor called!"); }

	void handleKeyboard();
	void handleMouse();
};

extern Event *event;

#endif // EVENT_HPP