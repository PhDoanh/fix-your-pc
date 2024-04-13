#include "Game.hpp"
#include "Entity.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"

bool Enemy::killed = true;
int Enemy::count = 0;
int Enemy::index = -1;
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

void Player::attack(Enemy *enemy)
{
	if (event->cur_txt_inp.front() == enemy->name[Enemy::name_index])
		enemy->name[Enemy::name_index++] = ' ';
}

void Player::takeDamage()
{
}

// Enemy
void Enemy::showName()
{
	float a = float(players[0]->y - y - 96) / (players[0]->x - x);
	float b = players[0]->y - a * players[0]->x;
	if (x < 0) // left
	{
		x_txt_box = 0;
		y_txt_box = b;
	}
	else if (y + 96 < 0) // top
	{
		x_txt_box = -b / a;
		y_txt_box = 0;
	}
	else if (x > Game::win_w - w_txt_box) // right
	{
		x_txt_box = Game::win_w - w_txt_box;
		y_txt_box = a * x_txt_box + b;
	}
	else if (y + 96 > Game::win_h - h_txt_box) // bottom
	{
		x_txt_box = (Game::win_h - b) / a;
		y_txt_box = Game::win_h - h_txt_box;
	}
	else // inside
	{
		x_txt_box = x;
		y_txt_box = y + 96;
	}
}

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
	// std::cout << speed << ' ' << dx << ' ' << dy << '\n';
}

void Enemy::attack()
{
}

void Enemy::takeDamage()
{
	if (name == std::string(name.size(), ' '))
	{
		enemies.erase(enemies.begin() + index);
		index = -1;
		name_index = 0;
		killed = true;
	}

	if (name != prev_name)
	{
		txt_box_texture = screen->loadText(name);
		prev_name = name;
	}
}

void Enemy::spawnNearTo(Player *player)
{
	if (player->x <= Game::win_w / 2.0)
	{
		if (player->y <= Game::win_h / 2.0) // 1st corner (top left)
		{
			if (player->x <= player->y) // left edge
			{
				x = -(Game::win_w / 2 - player->x);
				y = rand() % (Game::win_h / 2);
			}
			else // top edge
			{
				x = rand() % (Game::win_w / 2);
				y = -(Game::win_h / 2 - player->y);
			}
		}
		else // 2nd corner (down left)
		{
			if (player->x <= Game::win_h - player->y) // left edge
			{
				x = -(Game::win_w / 2 - player->x);
				y = rand() % (Game::win_h / 2) + Game::win_h / 2;
			}
			else // down edge
			{
				x = rand() % (Game::win_w / 2);
				y = player->y + Game::win_h / 2;
			}
		}
	}
	else
	{
		if (player->y <= Game::win_h / 2.0) // 3rd corner (top right)
		{
			if (Game::win_w - player->x <= player->y) // right edge
			{
				x = player->x + Game::win_w / 2;
				y = rand() % (Game::win_h / 2);
			}
			else // top edge
			{
				x = rand() % (Game::win_w / 2) + Game::win_w / 2;
				y = -(Game::win_h / 2 - player->y);
			}
		}
		else // 4th corner (down right)
		{
			if (Game::win_w - player->x <= Game::win_h - player->y) // right edge
			{
				x = player->x + Game::win_w / 2;
				y = rand() % (Game::win_h / 2) + Game::win_h / 2;
			}
			else // down edge
			{
				x = rand() % (Game::win_w / 2) + Game::win_w / 2;
				y = player->y + Game::win_h / 2;
			}
		}
	}
}