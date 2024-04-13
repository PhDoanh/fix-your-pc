#include "Event.hpp"
#include "Entity.hpp"

void Event::handleKeyboard()
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_LALT:
			SDL_StopTextInput();
			players[0]->moving = true;
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
			players[0]->moving = false;
			break;
		default:
			break;
		}

	case SDL_TEXTINPUT:
		cur_txt_inp = e.text.text;
		// dev->log(cur_txt_inp);
		break;
	case SDL_TEXTEDITING:
		break;
	}
}

void Event::handleMouse()
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
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