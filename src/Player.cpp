#include "Player.hpp"
#include "Screen.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Event.hpp"
#include "util.hpp"

float Player::speed = 8.5;
int Player::score = 0;
SDL_Rect Player::rect = Rect::square(64);

Player::Player()
{
	info("Player constructor called!");
}

Player::~Player()
{
	info("Player destructor called!");
}

void Player::move()
{

	float dx = 0, dy = 0;

	if (Event::state[SDL_SCANCODE_RIGHT])
		dx += speed;
	if (Event::state[SDL_SCANCODE_LEFT])
		dx -= speed;
	if (Event::state[SDL_SCANCODE_UP])
		dy -= speed;
	if (Event::state[SDL_SCANCODE_DOWN])
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
	if (rect.x + rect.w - 10 > Game::win_w)
		rect.x = Game::win_w + 10 - rect.w;
	if (rect.y <= 0)
		rect.y = 0;
	if (rect.y + rect.h > Game::win_h)
		rect.y = Game::win_h - rect.h;
}