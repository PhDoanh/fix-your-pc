#include "Game.hpp"
#include "Entity.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"

int Enemy::index = 0;

// Player
void Player::move()
{
	if (event->mouse_control)
	{
		x = event->mouse_x;
		y = event->mouse_y;
	}

	if (event->keyboard_control)
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
	}

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
	// log(event->cur_txt_inp);
	std::string enemy_cur_chr;
	enemy_cur_chr += enemies[i]->name[Enemy::index];
	if (event->cur_chr_inp == enemy_cur_chr)
		enemies[i]->takeDamage();
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
	name[index] = ' ';
	index++;
}

void Enemy::spawn()
{
	if (players[0]->x < Game::win_w / 2)
		x = std::rand() % (Game::win_w / 2) + Game::win_w / 2;
	else
		x = std::rand() % (Game::win_w / 2);

	if (players[0]->y < Game::win_h / 2)
		y = std::rand() % (Game::win_h / 2) + Game::win_h / 2;
	else
		y = std::rand() % (Game::win_h / 2);
}