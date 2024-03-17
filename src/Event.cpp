#include "Event.hpp"
#include "Player.hpp"
#include "Enemies.hpp"
#include "Screen.hpp"
#include "Game.hpp"
#include "util.hpp"

SDL_Event Event::e;
const Uint8 *Event::state;

Event::Event(/* args */)
{
	info("Event constructor called!");
}

Event::~Event()
{
	info("Event destructor called!");
}

void Event::handleKeyboard()
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game::running = false;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		break;
	default:
		break;
	}
}

void Event::handleMouse()
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		// player.mouseMovement(e.motion);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			// sound.playSoundEffect("left click", left_click);
			// player.enableDialog();
			break;
		case SDL_BUTTON_RIGHT:
			// sound.playSoundEffect("right click", right_click);
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