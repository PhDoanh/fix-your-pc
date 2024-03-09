#include "GameInit.hpp"
#include "GameLoop.hpp"
#include "Interface.hpp"
#include <bits/stdc++.h>

GameLoop::GameLoop()
{
	// constructor implementation
}

GameLoop::~GameLoop()
{
	// destructor implementation
}

void GameLoop::start()
{
	Interface ui;
	ui.loadMenu();

	SDL_Event event;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_TEXTINPUT && !Interface::is_txt_entered)
			{
				Interface::txt_inp += event.text.text;
			}
			if (event.type == SDL_KEYDOWN && !Interface::is_txt_entered)
			{
				if (event.key.keysym.sym == SDLK_BACKSPACE && Interface::txt_inp.length() > 0 && Interface::txt_inp != PLACEHOLDER)
				{
					Interface::txt_inp.pop_back();
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
				{
					Interface::is_txt_entered = false;
				}
			}
		}

		SDL_RenderClear(GameInit::renderer);
		ui.showInputBar();
	}
}
