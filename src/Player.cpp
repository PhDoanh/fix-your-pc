#include "Player.hpp"
#include "Screen.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Console.hpp"
#include "Utility.hpp"
#include "Event.hpp"

Player player;
SDL_Rect Player::rect = Rect::square(64);

Player::Player()
{
}

Player::~Player()
{
	// destructor implementation
}

void Player::updatePosition(SDL_MouseMotionEvent mouse)
{
	rect.x = mouse.x;
	rect.y = mouse.y;
}

void Player::move()
{
	float dx = 0, dy = 0;

	if (event.keyboard[SDLK_RIGHT])
		dx += speed;
	if (event.keyboard[SDLK_LEFT])
		dx -= speed;
	if (event.keyboard[SDLK_UP])
		dy -= speed;
	if (event.keyboard[SDLK_DOWN])
		dy += speed;

	// vector standardized
	float length = std::sqrt(dx * dx + dy * dy);
	if (length > 0)
	{
		dx /= length;
		dy /= length;
	}

	// update player position
	rect.x += dx * speed;
	rect.y += dy * speed;

	if (rect.x <= 0)
		rect.x = 0;
	if (rect.x + rect.w > Game::win_w)
		rect.x = Game::win_w - rect.w;
	if (rect.y <= 0)
		rect.y = 0;
	if (rect.y + rect.h > Game::win_h)
		rect.y = Game::win_h - rect.h;
}
