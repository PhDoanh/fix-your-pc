// #include "Event.hpp"
// #include "Player.hpp"
// #include "Enemy.hpp"
// #include "Screen.hpp"

// SDL_Event Event::event;

// Event::Event()
// {
// 	// constructor implementation
// }

// Event::~Event()
// {
// 	// destructor implementation
// }

// void Event::handle(Player p)
// {
// 	while (SDL_PollEvent(&event))
// 	{
// 		handlerMouse(p);
// 		handlerKeyboard(p);
// 	}
// }

// void Event::update()
// {
// }

// void Event::handleMouse(Player p)
// {
// 	switch (event.type)
// 	{
// 	case SDL_MOUSEMOTION:
// 		p.updatePosition(event.motion);
// 		break;
// 	case SDL_MOUSEBUTTONDOWN:
// 		switch (event.button.button)
// 		{
// 		case SDL_BUTTON_LEFT:
// 			p.playSoundEffect(left_click);
// 			break;
// 		case SDL_BUTTON_RIGHT:
// 			p.playSoundEffect(right_click);
// 			break;
// 		default:
// 			break;
// 		}
// 		break;
// 	case SDL_MOUSEBUTTONUP:
// 		break;
// 	default:
// 		break;
// 	}
// }

// void Event::handleKeyboard(Player p)
// {
// }
