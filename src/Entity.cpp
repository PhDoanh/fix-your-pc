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
	if (event->state[SDL_SCANCODE_D])
		dx += speed;
	if (event->state[SDL_SCANCODE_A])
		dx -= speed;
	if (event->state[SDL_SCANCODE_W])
		dy -= speed;
	if (event->state[SDL_SCANCODE_S])
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
	if (x > Game::win_w)
		x = Game::win_w - 10;
	if (y <= 0)
		y = 0;
	if (y > Game::win_h)
		y = Game::win_h - 10;
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
}

void Enemy::spawn(const int &player_x, const int &player_y)
{
	if (player_x < Game::win_w / 2)
		x = std::rand() % (Game::win_w / 2) + Game::win_w / 2;
	else
		x = std::rand() % (Game::win_w / 2);

	if (player_y < Game::win_h / 2)
		y = std::rand() % (Game::win_h / 2) + Game::win_h / 2;
	else
		y = std::rand() % (Game::win_h / 2);
}