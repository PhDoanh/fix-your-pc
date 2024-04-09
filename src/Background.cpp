#include "Background.hpp"
#include "Game.hpp"
#include "Event.hpp"

Background::Background()
{
	x = 0;
	y = 0;
	speed = 1.5;
	info("Background constructor called.");
}

Background::~Background()
{
	info("Background destructor called.");
}

void Background::move()
{
	float dx = 0, dy = 0;
	if (event->state[SDL_SCANCODE_D]) // left
		dx -= speed;
	if (event->state[SDL_SCANCODE_A]) // right
		dx += speed;
	if (event->state[SDL_SCANCODE_W]) // down
		dy += speed;
	if (event->state[SDL_SCANCODE_S]) // up
		dy -= speed;

	float len = std::sqrt(dx * dx + dy * dy);
	if (len > 0)
	{
		dx /= len;
		dy /= len;
	}
	x += dx * speed;
	y += dy * speed;

	if (x >= 0)
		x = 0;
	if (x <= -0.1 * Game::win_w)
		x = -0.1 * Game::win_w;
	if (y >= 0)
		y = 0;
	if (y <= -0.1 * Game::win_h)
		y = -0.1 * Game::win_h;
}