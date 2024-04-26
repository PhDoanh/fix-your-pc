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
			if (Game::state != over)
			{
				if (Game::state != pause)
					sound->playSoundEffect("balloon", general);
				ui->saveCurScreen();
				std::swap(Game::state, Game::prev_state);
			}
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
			sound->playSoundEffect("rclick", player_channel);
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
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				std::string tmp = SDL_GetClipboardText();
				cur_txt_inp += tmp;
			}
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
			std::vector<std::string> dt = {"Welcome " + cur_txt_inp + " ", ""};
			ui->setDynamicText(dt);
			player->id = cur_txt_inp;
			is_txt_entered = true;
			cur_txt_inp.clear();
		}
	}
	else if (Game::state == play)
	{
		if (player->num_of_chrs >= 10)
		{
			player->addDeadZone();
			player->num_of_chrs = 0;
		}
	}
	else if (Game::state == pause)
	{
		if (!cur_txt_inp.empty())
		{
			if (ui->getMusicPathBoxState() == true)
			{
				ui->cur_music_path_txt = cur_txt_inp;
				sound->playSoundEffect("locked", general);
				ui->deleteElements();
				ui->loadElements();
			}
			if (ui->getTextPathBoxState() == true)
			{
				ui->cur_txt_path_txt = cur_txt_inp;
				sound->playSoundEffect("locked", general);
				ui->deleteElements();
				ui->loadElements();
			}
			cur_txt_inp.clear();
		}
	}
}

void Event::handleLeftClick()
{
	sound->playSoundEffect("lclick", player_channel);
	if (Game::state == ready)
	{
		activatePassBox();
	}
	else if (Game::state == pause)
	{
		activateMusicFileBox();
		activateTextFileBox();
		activateOption();
	}
}

bool Event::isHoverOn(const Vec2D &pos, const Vec2D &size)
{
	if (mouse_pos.between(pos, pos + size))
		return true;
	return false;
}

void Event::activatePassBox()
{
	Vec2D pass_pos = ui->getPassBoxPos();
	Vec2D pass_size = ui->getPassBoxSize();
	if (isHoverOn(pass_pos, pass_size))
	{
		SDL_StartTextInput();
		ui->setPassBoxBorderColor(Color::black(255));
		ui->setPassBoxColor(Color::white(255));
		ui->turnOnPassBox();
	}
	else
	{
		SDL_StopTextInput();
		ui->setPassBoxBorderColor(Color::ice_blue(255));
		ui->setPassBoxColor(Color::ice_blue(255));
		ui->turnOffPassBox();
	}
}

void Event::activateMusicFileBox()
{
	Vec2D music_pos = ui->getMusicPathBoxPos();
	Vec2D music_size = ui->getMusicPathBoxSize();
	if (isHoverOn(music_pos, music_size))
	{

		SDL_StartTextInput();
		ui->setMusicPathBoxBorderColor(Color::black(255));
		ui->setMusicPathBoxColor(Color::white(255));
		ui->turnOnMusicPathBox();
	}
	else
	{
		SDL_StopTextInput();
		ui->setMusicPathBoxBorderColor(Color::ice_blue(255));
		ui->setMusicPathBoxColor(Color::ice_blue(255));
		ui->turnOffMusicPathBox();
		cur_txt_inp.clear();
	}
}

void Event::activateTextFileBox()
{
	Vec2D text_pos = ui->getTextPathBoxPos();
	Vec2D text_size = ui->getTextPathBoxSize();
	if (isHoverOn(text_pos, text_size))
	{
		SDL_StartTextInput();
		ui->setTextPathBoxBorderColor(Color::black(255));
		ui->setTextPathBoxColor(Color::white(255));
		ui->turnOnTextPathBox();
	}
	else
	{
		SDL_StopTextInput();
		ui->setTextPathBoxBorderColor(Color::ice_blue(255));
		ui->setTextPathBoxColor(Color::ice_blue(255));
		ui->turnOffTextPathBox();
		cur_txt_inp.clear();
	}
}

void Event::activateOption()
{
	for (auto &&option : ui->options)
	{
		if (isHoverOn(option.second->real_pos, option.second->size))
		{
			if (option.first == "3") // music down
			{
				int &vol = settings["musicVol"];
				if (vol - 10 >= 0)
					vol -= 10;
				ui->options["4"]->text = std::to_string(vol) + "%";
				Mix_VolumeMusic(vol * MIX_MAX_VOLUME / 100);
			}
			if (option.first == "5") // music up
			{
				int &vol = settings["musicVol"];
				if (vol + 10 <= 100)
					vol += 10;
				ui->options["4"]->text = std::to_string(vol) + "%";
				Mix_VolumeMusic(vol * MIX_MAX_VOLUME / 100);
			}
			if (option.first == "7") // sound down
			{
				int &vol = settings["soundVol"];
				if (vol - 10 >= 0)
					vol -= 10;
				ui->options["8"]->text = std::to_string(vol) + "%";
				for (auto &&sound : sounds)
					Mix_VolumeChunk(sound.second, vol * MIX_MAX_VOLUME / 100);
			}
			if (option.first == "9") // sound up
			{
				int &vol = settings["soundVol"];
				if (vol + 10 <= 100)
					vol += 10;
				ui->options["8"]->text = std::to_string(vol) + "%";
				for (auto &&sound : sounds)
					Mix_VolumeChunk(sound.second, vol * MIX_MAX_VOLUME / 100);
			}
			if (option.first == "11") // enable num
			{
				if (option.second->text == "[ ]")
				{
					settings["enableNum"] = 1;
					option.second->text = "[x]";
				}
				else
				{
					settings["enableNum"] = 0;
					option.second->text = "[ ]";
				}
			}
			if (option.first == "13") // enable case
			{
				if (option.second->text == "[ ]")
				{
					settings["enableCaseSensitive"] = 1;
					option.second->text = "[x]";
				}
				else
				{
					settings["enableCaseSensitive"] = 0;
					option.second->text = "[ ]";
				}
			}
			if (option.first == "15") // enable punct
			{
				if (option.second->text == "[ ]")
				{
					settings["enablePunct"] = 1;
					option.second->text = "[x]";
				}
				else
				{
					settings["enablePunct"] = 0;
					option.second->text = "[ ]";
				}
			}
			if (option.first == "17") // resume
			{
				std::swap(Game::state, Game::prev_state);
			}
			if (option.first == "18") // lock screen
			{
				sound->stopMusic();
				sound->playSoundEffect("locked", general);
				ui->deleteElements();
				ui->loadElements();
			}
			if (option.first == "19") // shutdown
			{
				sound->stopMusic();
				ui->shutdown = true;
				ui->setShutdownTime(3);
				Game::state = over;
			}
		}
	}
}