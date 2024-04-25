#include <string>
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
	std::string cur_txt_inp;
	bool is_txt_entered;
	Vec2D mouse_pos;

	Event() { info("Event constructor called!\n"); }
	~Event() { info("Event destructor called!\n"); }

	void handleKeyboard();
	void handleMouse();

	void handleTextInput();
	void handleLeftAlt();
	void handleEnter();
	void handleLeftClick();

	bool isTextInputEmpty() { return cur_txt_inp.empty(); }
	bool isHoverOn(const Vec2D &pos, const Vec2D &size);
	void activatePassBox();
	void activateMusicFileBox();
	void activateTextFileBox();
	void activateOption();
};

extern Event *event;

#endif // EVENT_HPP