#include "Game.hpp"
#include "Entity.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"

int Enemy::count = 0;
int Enemy::name_index = 0;

// Player
void Player::move()
{
	float dx = 0, dy = 0;
	if (event->state[SDL_SCANCODE_D]) // right
		dx += speed;
	if (event->state[SDL_SCANCODE_A]) // left
		dx -= speed;
	if (event->state[SDL_SCANCODE_W]) // up
		dy -= speed;
	if (event->state[SDL_SCANCODE_S]) // down
		dy += speed;

	float len = std::sqrt(dx * dx + dy * dy);
	if (len > 0)
	{
		dx /= len;
		dy /= len;
	}
	x += dx * speed;
	y += dy * speed;

	if (x <= 0)
		x = 0;
	if (x + 35 > Game::win_w)
		x = Game::win_w - 35;
	if (y <= 0)
		y = 0;
	if (y + 64 > Game::win_h)
		y = Game::win_h - 64;
}

void Player::attack(const int &i)
{
}

void Player::takeDamage()
{
}

// Enemy
void Enemy::move()
{
	float dx = players[0]->x - x;
	float dy = players[0]->y - y;
	float len = std::sqrt(dx * dx + dy * dy);
	if (len > 0)
	{
		dx /= len;
		dy /= len;
	}
	x += dx * speed;
	y += dy * speed;
}

void Enemy::attack()
{
}

void Enemy::takeDamage()
{
	// do something
	if (name != prev_name)
	{
		texture = screen->loadText(name, Game::font18);
		prev_name = name;
	}
}

void Enemy::spawnNearTo(Player *player)
{
	x = 0;
	y = 0;
}