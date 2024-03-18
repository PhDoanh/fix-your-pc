#include "Enemies.hpp"
#include "Screen.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "util.hpp"

std::vector<Enemy> enemy;

Enemies::Enemies()
{
	info("Enemies constructor called!");
	for (int i = 0; i < 50; i++)
	{
		Enemy new_enemy;
		new_enemy.rect = Rect::square(96);
		new_enemy.speed_x = rand() % 5 + 1;
		new_enemy.speed_y = rand() % 5 + 1;
		enemy.emplace_back(new_enemy);
	}
}

Enemies::~Enemies()
{
	info("Enemies destructor called!");
}

void Enemies::spawn()
{
	int player_x = Player::rect.x;
	int player_y = Player::rect.y;
	int spawn_x, spawn_y;

	if (player_x < Game::win_w / 2)
		spawn_x = std::rand() % (Game::win_w / 2) + Game::win_w / 2;
	else
		spawn_x = std::rand() % (Game::win_w / 2);

	if (player_y < Game::win_h / 2)
		spawn_y = std::rand() % (Game::win_h / 2) + Game::win_h / 2;
	else
		spawn_y = std::rand() % (Game::win_h / 2);

	for (int i = 0; i < 10; i++)
	{
		Enemy new_enemy;
		new_enemy.rect = Rect::square(96);
		new_enemy.rect.x = spawn_x;
		new_enemy.rect.y = spawn_y;
		new_enemy.speed_x = std::rand() % 5 + 1;
		new_enemy.speed_y = std::rand() % 5 + 1;
		enemy.emplace_back(new_enemy);
	}
}

void Enemies::move()
{
	for (int i = 0; i < enemy.size(); i++)
	{
		chaseMotion(enemy[i]);
	}
}

void Enemies::chaseMotion(Enemy &enemy)
{
	const SDL_Rect &player_rect = Player::rect;
	float dirx = player_rect.x - enemy.rect.x;
	float diry = player_rect.y - enemy.rect.y;
	float hyp = std::sqrt(dirx * dirx + diry * diry);
	dirx /= hyp;
	diry /= hyp;
	enemy.rect.x += dirx * enemy.speed_x;
	enemy.rect.y += diry * enemy.speed_y;
}

void Enemies::bounceMotion(Enemy &enemy)
{
	// SDL_Rect &enemy_rect = enemy[i]->rect;
	// enemy_rect.x += enemy[i]->speed_x;
	// enemy_rect.y += enemy[i]->speed_y;

	// if (enemy_rect.x < 0 || enemy_rect.x + enemy_rect.w > Game::win_w)
	// 	enemy[i]->speed_x = -enemy[i]->speed_x;
	// if (enemy_rect.y < 0 || enemy_rect.y + enemy_rect.h > Game::win_h)
	// 	enemy[i]->speed_y = -enemy[i]->speed_y;
}

void Enemies::parabolMotion(Enemy &enemy)
{
}

void Enemies::randomMotion(Enemy &enemy)
{
}

void Enemies::circleMotion(Enemy &enemy)
{
}