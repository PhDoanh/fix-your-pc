#include "Event.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Screen.hpp"
#include "Console.hpp"

Event event;
SDL_Event Event::e;

Event::Event()
{
	// constructor implementation
}

Event::~Event()
{
	// destructor implementation
}

void Event::handleMouse()
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		player.updatePosition(e.motion);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			sound.playSoundEffect("left click", left_click);
			break;
		case SDL_BUTTON_RIGHT:
			sound.playSoundEffect("right click", right_click);
			break;
		default:
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	default:
		break;
	}
}

void Event::handleKeyboard()
{
}
