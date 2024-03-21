#include "Event.hpp"

void Event::handleKeyboard()
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_LALT:
			SDL_StopTextInput();
			keyboard_control = true;
			mouse_control = false;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_LALT:
			SDL_StartTextInput();
			keyboard_control = false;
			break;
		default:
			break;
		}

	case SDL_TEXTINPUT:
		cur_chr_inp = e.text.text;
		break;
	}
}

void Event::handleMouse()
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		mouse_control = true;
		keyboard_control = false;
		mouse_x = e.motion.x;
		mouse_y = e.motion.y;
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