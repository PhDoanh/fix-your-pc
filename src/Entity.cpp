#include "Game.hpp"
#include "Entity.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Event.hpp"
#include "UI.hpp"

int Enemy::count = 0;

// Entity
Entity::Entity(const std::string &id, const Vec2D &pos, const Vec2D &size)
{
	this->id = id;
	this->pos = pos;
	this->size = size;
}

Entity::Entity(const Vec2D &pos, const Vec2D &size, const Vec2D &speed)
{
	this->pos = pos;
	this->size = size;
	this->speed = speed;
}

Entity::Entity(const std::string &id, const Vec2D &pos, const Vec2D &size, const Vec2D &speed, const int &health)
{
	this->id = id;
	this->pos = pos;
	this->size = size;
	this->speed = speed;
	this->health = health;
	this->killed = false;
	this->center_pos = Rect::getCenter(pos, size);
}

Entity::~Entity() {}

// Player
Player::Player(const std::string &id, const Vec2D &pos, const Vec2D &size, const Vec2D &speed, const int &health)
	: Entity(id, pos, size, speed, health)
{
	this->state = "arrow";
	this->index = -1;
	this->score = 0;
	this->num_of_chrs = 0;
	this->num_of_dead_zones = 0;
	this->angle = this->goal_angle = -26;
	this->cur_frame = this->cur_layer = 0;
	this->max_frame = 5;
	this->max_layer = 5;
	this->shield.last_time = this->shield_state.last_time = SDL_GetTicks64();
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

	vel.x = lerp(goal_vel.x, vel.x, Game::deltaTime * 20);
	vel.y = lerp(goal_vel.y, vel.y, Game::deltaTime * 20);

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
	if (index < 0) // find nearest enemy index
	{
		if (!event->cur_txt_inp.empty())
		{
			float cur_d, min_d = INT_MAX;
			for (int i = 0; i < enemies.size(); i++)
			{
				cur_d = enemies[i]->pos.distance(pos);
				if (event->cur_txt_inp[0] == enemies[i]->name[0] && cur_d < min_d)
				{
					min_d = cur_d;
					index = i;
				}
			}
			if (index < 0)
				event->cur_txt_inp.clear();
			else
				makeCircleOn(enemies[index]);
		}
	}
	else // valid index -> attack nearest enemy
	{
		Enemy *enemy = enemies[index];
		Vec2D dpos = enemy->center_pos - center_pos;
		goal_angle = (std::atan2(dpos.y, dpos.x) * 180 / PI) + 90;
		if (!event->cur_txt_inp.empty())
		{
			if (event->cur_txt_inp[0] == enemy->name[enemy->name_index])
			{
				shootBullet();
				num_of_chrs++;
				true_chrs++;
				enemy->name[enemy->name_index++] = ' ';
				enemy->name_color = Color::light_orange(0);
				event->cur_txt_inp.clear();
			}
			else // wrong type
			{
				sound->playSoundEffect("typing", player_channel);
				num_of_chrs = 0;
				wrong_chrs++;
				event->cur_txt_inp.clear();
			}
		}
		moveCircleOn(enemy);
		moveBulletTo(enemy);
	}
	releaseDeadZone();
	if (num_of_chrs > score)
		score = num_of_chrs;
}

void Player::makeCircleOn(Enemy *enemy)
{
	sound->playSoundEffect("target", general);
	Vec2D circle_size = sprites["reticle"]->real_size * 3;
	Vec2D circle_pos = enemy->pos - (circle_size - enemy->size) / 2.0;
	Vec2D circle_speed = Vec2D(100);
	Entity *circle = new Entity(circle_pos, circle_size, circle_speed);
	circle->goal_angle = 0;
	circle->delta_angle = 0.5;
	circles.push_back(circle);
}

void Player::moveCircleOn(Enemy *enemy)
{
	for (int i = 0; i < circles.size(); i++)
	{
		if (circles[i]->size.x <= 0 || circles[i]->size.y <= 0 || enemy->health == 0)
		{
			delete circles[i];
			circles[i] = nullptr;
			circles.erase(circles.begin() + i);
		}
		else
		{
			circles[i]->pos = enemy->pos - (circles[i]->size - enemy->size) / 2.0;

			circles[i]->vel.x = lerp(circles[i]->speed.x, circles[i]->vel.x, Game::deltaTime * 100);
			circles[i]->vel.y = lerp(circles[i]->speed.y, circles[i]->vel.y, Game::deltaTime * 100);
			circles[i]->size -= circles[i]->vel;

			circles[i]->goal_angle += 1;
			circles[i]->angle = lerpAngle(circles[i]->angle, circles[i]->goal_angle, circles[i]->delta_angle);
		}
	}
}

void Player::shootBullet()
{
	sound->playSoundEffect("plasma", player_channel);
	Vec2D bullet_size = sprites["bullet"]->real_size;
	Vec2D buillet_pos = pos + (size - bullet_size) / 2.0;
	Vec2D bullet_speed = Vec2D(20);
	Entity *bullet = new Entity(buillet_pos, bullet_size, bullet_speed);
	bullets.push_back(bullet);
}

void Player::moveBulletTo(Enemy *enemy)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->goal_vel = {0, 0};
		if (bullets[i]->pos.x < enemy->pos.x) // right
			bullets[i]->goal_vel.x = bullets[i]->speed.x;
		if (bullets[i]->pos.x > enemy->pos.x) // left
			bullets[i]->goal_vel.x = -bullets[i]->speed.x;
		if (bullets[i]->pos.y > enemy->pos.y) // up
			bullets[i]->goal_vel.y = -bullets[i]->speed.y;
		if (bullets[i]->pos.y < enemy->pos.y) // down
			bullets[i]->goal_vel.y = bullets[i]->speed.y;

		bullets[i]->vel.x = lerp(bullets[i]->goal_vel.x, bullets[i]->vel.x, Game::deltaTime * 30);
		bullets[i]->vel.y = lerp(bullets[i]->goal_vel.y, bullets[i]->vel.y, Game::deltaTime * 30);

		Vec2D dpos;
		dpos = enemy->pos - bullets[i]->pos;
		float len = std::sqrt(dpos.x * dpos.x + dpos.y * dpos.y);
		if (len > 0)
			dpos /= len;
		bullets[i]->pos.x += fabs(dpos.x) * bullets[i]->vel.x;
		bullets[i]->pos.y += fabs(dpos.y) * bullets[i]->vel.y;
		bullets[i]->center_pos = Rect::getCenter(bullets[i]->pos, bullets[i]->size);

		Vec2D dcenter_pos = enemy->center_pos - bullets[i]->center_pos;
		bullets[i]->goal_angle = (std::atan2(dcenter_pos.y, dcenter_pos.x) * 180 / PI) + 90;
		bullets[i]->angle = lerpAngle(bullets[i]->angle, bullets[i]->goal_angle, 0.25);
	}
}

void Player::addDeadZone()
{
	sound->playSoundEffect("emp", player_channel);
	Vec2D zone_size = Vec2D();
	Vec2D zone_pos = pos + (size - zone_size) / 2.0;
	Vec2D zone_speed = Vec2D();
	Entity *zone = new Entity(zone_pos, zone_size, zone_speed);
	zone->vel = Vec2D(30);
	zone->goal_angle = 0;
	zone->delta_angle = 0.5;
	dead_zones.push_back(zone);
}

void Player::releaseDeadZone()
{
	for (int i = 0; i < dead_zones.size(); i++)
	{
		if (dead_zones[i]->size.x >= 5 * sprites["emp"]->real_size.x ||
			dead_zones[i]->size.y >= 5 * sprites["emp"]->real_size.y)
		{
			delete dead_zones[i];
			dead_zones[i] = nullptr;
			dead_zones.erase(dead_zones.begin() + i);
		}
		else
		{
			dead_zones[i]->pos = pos + (size - dead_zones[i]->size) / 2.0;

			dead_zones[i]->vel.x = lerp(dead_zones[i]->speed.x, dead_zones[i]->vel.x, Game::deltaTime * 10);
			dead_zones[i]->vel.y = lerp(dead_zones[i]->speed.y, dead_zones[i]->vel.y, Game::deltaTime * 10);
			dead_zones[i]->size += dead_zones[i]->vel;

			dead_zones[i]->goal_angle += 1;
			dead_zones[i]->angle = lerpAngle(dead_zones[i]->angle, dead_zones[i]->goal_angle, dead_zones[i]->delta_angle);
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

void Player::updateScore()
{
	score += num_of_chrs;
}

void Player::takeDamage()
{
	if (health == 0) // game over
	{
		sound->stopMusic();
		sound->playSoundEffect("critical stop", general);
		std::vector<std::string> dt = {
			"Well... ",
			"You failed to fix the PC ",
			"Above is your achivements ",
			"Whatever, if you see some \"real\" game bugs ",
			"Contact me by above QRCode ",
			"Thank for playing :) ",
			"Shutdown in 15s "};
		ui->setDynamicText(dt);
		Game::state = over;
	}
	else
	{
		Uint64 cur_time = SDL_GetTicks64();
		if (cur_time - shield.last_time >= shield.time)
		{
			sound->playSoundEffect("health loss", player_channel);
			cur_layer = 0;
			health--;
			cur_frame = (cur_frame == max_frame) ? cur_frame : cur_frame + 1;
			shield.last_time = cur_time;
		}
	}
}

// Enemy
Enemy::Enemy(const std::string &name, const std::string &id, const Vec2D &pos, const Vec2D &size, const Vec2D &speed)
	: Entity(id, pos, size, speed, name.size())
{
	this->name = name;
	this->name_index = 0;
	this->name_color = Color::white(0);
	this->count++;
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
		player->takeDamage();
}

void Enemy::takeDamage(const int &i)
{
	if (name == std::string(name.size(), ' ')) // erase name
		name.clear();

	if (health == 0) // erase itselfs
	{
		int sz = size.x;
		switch (sz)
		{
		case extra:
			sound->playSoundEffect("explosion large", enemy_channel);
			break;
		case big:
		case medium:
			sound->playSoundEffect("explosion", enemy_channel);
			break;
		case small:
		case mini:
			sound->playSoundEffect("explosion small", enemy_channel);
			break;
		default:
			break;
		}
		enemies.erase(enemies.begin() + player->index);
		player->index = -1;
		event->cur_txt_inp.clear();
	}
	else
	{
		if (i == player->index)
		{
			for (int i = 0; i < player->bullets.size(); i++)
			{
				if (Rect::isCollide(pos, size, player->bullets[i]->pos, player->bullets[i]->size))
				{
					sound->playSoundEffect("hit", enemy_channel);
					health--;
					delete player->bullets[i];
					player->bullets[i] = nullptr;
					player->bullets.erase(player->bullets.begin());
				}
			}
		}
		for (int i = 0; i < player->dead_zones.size(); i++)
		{
			SDL_Rect rect1 = {
				int(player->dead_zones[i]->pos.x),
				int(player->dead_zones[i]->pos.y),
				int(player->dead_zones[i]->size.x),
				int(player->dead_zones[i]->size.y)};
			SDL_Rect rect2 = {int(pos.x), int(pos.y), int(size.x), int(size.y)};
			if (Rect::isPixelCollide(sprites["emp"]->texture, rect1, sprites[id]->texture, rect2))
				health = 0;
		}
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