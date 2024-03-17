#include "Enemies.hpp"
#include "Screen.hpp"
#include "Game.hpp"
#include "util.hpp"

std::map<int, Enemy *> enemy;

Enemies::Enemies()
{
	info("Enemies constructor called!");

	for (int i = 1; i <= 50; i++)
		enemy[i] = new Enemy();
}

Enemies::~Enemies()
{
	info("Enemies destructor called!");

	// info("Deleting all sprites ...");
	for (auto &&e : enemy)
	{
		delete e.second;
		e.second = nullptr;
		enemy.erase(e.first);
		// if (e.second)
		// 	error(path + " - fail.");
		// else
		// 	info(path + " - done.");
	}
}

// void Enemies::updatePosition()
// {
// 	for (int i = 1; i <= 50; i++)
// 	{
// 		bounceMotion();
// 	}
// }

void Enemies::bounceMotion(int i)
{
	enemy[i]->x += enemy[i]->vx;
	enemy[i]->y += enemy[i]->vy;

	if (enemy[i]->x < 0 || enemy[i]->x + enemy[i]->w > Game::win_w)
		enemy[i]->vx = -enemy[i]->vx;
	if (enemy[i]->y < 0 || enemy[i]->y + enemy[i]->h > Game::win_h)
		enemy[i]->vy = -enemy[i]->vy;
}

void Enemies::parabolicMotion()
{
}

void Enemies::randomMotion()
{
}

void Enemies::circleMotion()
{
}