#include "Player.hpp"
#include "Screen.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "util.hpp"
#include "Event.hpp"

Player::Player()
{
	info("Player constructor called!");
}

Player::~Player()
{
	info("Player destructor called!");
}

void Player::update()
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
	x += dx * speed;
	y += dy * speed;

	if (x <= 0)
		x = 0;
	if (x + w - 10 > Game::win_w)
		x = Game::win_w + 10 - w;
	if (y <= 0)
		y = 0;
	if (y + h > Game::win_h)
		y = Game::win_h - h;
}