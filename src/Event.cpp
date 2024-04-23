#include "Game.hpp"
#include "Sound.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "UI.hpp"

void Event::handleKeyboard()
{
	handleTextInput();
	handleLeftAlt();
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_RETURN:
			handleEnter();
			break;
		case SDLK_ESCAPE:
			ui->saveCurScreen();
			std::swap(Game::state, Game::prev_state);
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		default:
			break;
		}
		break;
	}
}

void Event::handleMouse()
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		mouse_pos = Vec2D(e.motion.x, e.motion.y);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			handleLeftClick();
			break;
		case SDL_BUTTON_RIGHT:
			sound->playSoundEffect("rclick", general);
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

void Event::handleTextInput()
{
	if (Game::state == ready || Game::state == pause)
	{
		if (e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN)
		{
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && cur_txt_inp.size() > 0)
				cur_txt_inp = cur_txt_inp.substr(0, cur_txt_inp.size() - 1);
			else if (e.type == SDL_TEXTINPUT && e.key.keysym.sym != SDLK_SPACE)
				cur_txt_inp += e.text.text;
		}
	}
	else if (Game::state == play)
	{
		if (e.type == SDL_TEXTINPUT)
			cur_txt_inp += e.text.text;
	}
}

void Event::handleLeftAlt()
{
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LALT)
	{
		SDL_StopTextInput();
		player->speed = Vec2D(6);
	}
	if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_LALT)
	{
		SDL_StartTextInput();
		player->speed = Vec2D(0);
	}
}

void Event::handleEnter()
{
	if (Game::state == ready)
	{
		if (!cur_txt_inp.empty())
		{
			high_scores.insert({0, cur_txt_inp});
			std::vector<std::string> dt = {"Welcome " + cur_txt_inp + " "};
			ui->setDynamicText(dt);
			player->id = cur_txt_inp;
			is_txt_entered = true;
			cur_txt_inp.clear();
		}
	}
	else if (Game::state == play)
	{
		if (player->num_of_chrs >= 1)
		{
			player->addDeadZone();
			player->num_of_chrs = 0;
		}
	}
}

void Event::handleLeftClick()
{
	sound->playSoundEffect("lclick", general);
	if (Game::state == ready)
	{
		activePassBox();
	}
	else if (Game::state == pause)
	{
	}
}

bool Event::isHoverOn(const Vec2D &pos, const Vec2D &size)
{
	if (mouse_pos.between(pos, pos + size))
		return true;
	return false;
}

void Event::activePassBox()
{
	Vec2D pass_pos = ui->getPassBoxPos();
	Vec2D pass_size = ui->getPassBoxSize();
	if (mouse_pos.between(pass_pos, pass_pos + pass_size))
	{
		SDL_StartTextInput();
		ui->setPassBoxBorderColor(Color::black(255));
		ui->setPassBoxColor(Color::white(255));
	}
	else
	{
		SDL_StopTextInput();
		ui->setPassBoxBorderColor(Color::ice_blue(255));
		ui->setPassBoxColor(Color::ice_blue(255));
	}
}