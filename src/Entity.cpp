#include "Game.hpp"
#include "Entity.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"

bool Enemy::killed = true;
int Enemy::count = 0;
int Enemy::index = -1;
int Enemy::name_index = 0;

// Entity
Entity::Entity(const std::string &id, const Vec2D &pos, const Vec2D &size, const Vec2D &speed, const int &health)
{
	this->id = id;
	this->pos = pos;
	this->size = size;
	this->speed = speed;
	this->health = health;
	this->center_pos = Rect::getCenter(pos, size);
}

Entity::~Entity() {}

// Player
Player::Player(const std::string &id, const Vec2D &pos, const Vec2D &size, const Vec2D &speed, const int &health)
	: Entity(id, pos, size, speed, health)
{
	this->killed = false;
	this->cur_frame = 0;
	this->cur_layer = 0;
	this->score = 0;
	this->angle = this->goal_angle = -25;
}

Player::~Player() {}

void Player::move()
{
	goal_vel = {0, 0};
	if (event->state[SDL_SCANCODE_D]) // right
		goal_vel.x = speed.x;
	if (event->state[SDL_SCANCODE_A]) // left
		goal_vel.x = -speed.x;
	if (event->state[SDL_SCANCODE_W]) // up
		goal_vel.y = -speed.y;
	if (event->state[SDL_SCANCODE_S]) // down
		goal_vel.y = speed.y;
	if (event->state[SDL_SCANCODE_A] && event->state[SDL_SCANCODE_D])
		goal_vel.x = 0;
	if (event->state[SDL_SCANCODE_W] && event->state[SDL_SCANCODE_S])
		goal_vel.y = 0;

	vel.x = lerp(goal_vel.x, vel.x, Game::deltaTime * 30);
	vel.y = lerp(goal_vel.y, vel.y, Game::deltaTime * 30);

	Vec2D dpos;
	dpos += vel;
	float len = std::sqrt(dpos.x * dpos.x + dpos.y * dpos.y);
	if (len > 0)
		dpos /= len;
	pos.x += fabs(dpos.x) * vel.x;
	pos.y += fabs(dpos.y) * vel.y;
	center_pos = Rect::getCenter(pos, size);

	if (pos.x <= 0)
		pos.x = 0;
	if (pos.x + 35 > Game::win_w)
		pos.x = Game::win_w - 35;
	if (pos.y <= 0)
		pos.y = 0;
	if (pos.y + 64 > Game::win_h)
		pos.y = Game::win_h - 64;
}

void Player::attackNearestEnemy()
{
	if (Enemy::killed) // find enemy nearest to player
	{
		float cur_d, min_d = INT_MAX;
		for (int i = 0; i < enemies.size(); i++)
		{
			cur_d = enemies[i]->pos.distance(pos);
			if (event->cur_txt_inp.front() == enemies[i]->name.front() && cur_d < min_d)
			{
				min_d = cur_d;
				Enemy::index = i;
			}
		}
		if (Enemy::index >= 0)
			Enemy::killed = false;
	}
	if (Enemy::index >= 0) // kill cur nearest enemy
	{
		Vec2D dpos = enemies[Enemy::index]->center_pos - center_pos;
		goal_angle = (std::atan2(dpos.y, dpos.x) * 180 / PI) + 90;
		if (event->cur_txt_inp.front() == enemies[Enemy::index]->name[Enemy::name_index])
		{
			enemies[Enemy::index]->name[Enemy::name_index++] = ' ';
			enemies[Enemy::index]->name_color = Color::light_orange(0);
		}
	}
}

void Player::updateRotation()
{
	if (abs(angle - goal_angle) < 1.0)
	{
		if (angle > goal_angle)
			goal_angle -= 0.8;
		else if (angle < goal_angle)
			goal_angle += 0.8;
		delta_angle = Game::deltaTime;
	}
	else
		delta_angle = 0.25;
	angle = lerpAngle(angle, goal_angle, delta_angle);
}

void Player::takeDamage()
{
	if (killed)
	{
	}
}

// Enemy
Enemy::Enemy(const std::string &name, const std::string &id, const Vec2D &pos, const Vec2D &size, const Vec2D &speed)
	: Entity(id, pos, size, speed, name.size())
{
	this->name = name;
	this->name_color = Color::white(0);
	count++;
}

Enemy::~Enemy() {}

void Enemy::showName()
{
	float a = float(player->pos.y - pos.y - size.y) / (player->pos.x - pos.x);
	float b = player->pos.y - a * player->pos.x;
	if (pos.x < 0) // left
		name_pos = Vec2D(0, b);
	else if (pos.y + size.y < 0) // top
		name_pos = Vec2D(-b / a, 0);
	else if (pos.x > Game::win_w - name_size.x) // right
		name_pos = Vec2D(Game::win_w - name_size.x, a * (Game::win_w - name_size.x) + b);
	else if (pos.y + size.y > Game::win_h - name_size.y) // bottom
		name_pos = Vec2D((Game::win_h - name_size.y - b) / a, Game::win_h - name_size.y);
	else // inside
		name_pos = Vec2D(pos.x, pos.y + size.y);
}

void Enemy::move()
{
	goal_vel = {0, 0};
	if (pos.x < player->pos.x) // right
		goal_vel.x = speed.x;
	if (pos.x > player->pos.x) // left
		goal_vel.x = -speed.x;
	if (pos.y > player->pos.y) // up
		goal_vel.y = -speed.y;
	if (pos.y < player->pos.y) // down
		goal_vel.y = speed.y;

	vel.x = lerp(goal_vel.x, vel.x, Game::deltaTime * 20);
	vel.y = lerp(goal_vel.y, vel.y, Game::deltaTime * 20);
	// std::clog << vel;

	Vec2D dpos;
	dpos = player->pos - pos;
	float len = std::sqrt(dpos.x * dpos.x + dpos.y * dpos.y);
	if (len > 0)
		dpos /= len;
	pos.x += fabs(dpos.x) * vel.x;
	pos.y += fabs(dpos.y) * vel.y;
	center_pos = Rect::getCenter(pos, size);
}

void Enemy::attack(Player *player)
{
	if (Rect::isCollide(player->pos, player->size, pos, size))
	{
		if (player->health == 0)
			player->killed = true;
		player->health--;
		player->cur_frame++;
	}
}

void Enemy::takeDamage()
{
	if (name == std::string(name.size(), ' '))
	{
		delete enemies[index];
		enemies[index] = nullptr;
		enemies.erase(enemies.begin() + index);
		index = -1;
		name_index = 0;
		killed = true;
	}
}

void Enemy::spawnNearTo(Player *player)
{
	if (player->pos.x <= Game::win_w / 2.0)
	{
		if (player->pos.y <= Game::win_h / 2.0) // 1st corner (top left)
		{
			if (player->pos.x <= player->pos.y) // left edge
				pos = Vec2D(-(Game::win_w / 2 - player->pos.x), rand() % (Game::win_h / 2));
			else // top edge
				pos = Vec2D(rand() % (Game::win_w / 2), -(Game::win_h / 2 - player->pos.y));
		}
		else // 2nd corner (down left)
		{
			if (player->pos.x <= Game::win_h - player->pos.y) // left edge
				pos = Vec2D(-(Game::win_w / 2 - player->pos.x), rand() % (Game::win_h / 2) + Game::win_h / 2);
			else // down edge
				pos = Vec2D(rand() % (Game::win_w / 2), player->pos.y + Game::win_h / 2);
		}
	}
	else
	{
		if (player->pos.y <= Game::win_h / 2.0) // 3rd corner (top right)
		{
			if (Game::win_w - player->pos.x <= player->pos.y) // right edge
				pos = Vec2D(player->pos.x + Game::win_w / 2, rand() % (Game::win_h / 2));
			else // top edge
				pos = Vec2D(rand() % (Game::win_w / 2) + Game::win_w / 2, -(Game::win_h / 2 - player->pos.y));
		}
		else // 4th corner (down right)
		{
			if (Game::win_w - player->pos.x <= Game::win_h - player->pos.y) // right edge
				pos = Vec2D(player->pos.x + Game::win_w / 2, rand() % (Game::win_h / 2) + Game::win_h / 2);
			else // down edge
				pos = Vec2D(rand() % (Game::win_w / 2) + Game::win_w / 2, player->pos.y + Game::win_h / 2);
		}
	}
	center_pos = Rect::getCenter(pos, size);
}