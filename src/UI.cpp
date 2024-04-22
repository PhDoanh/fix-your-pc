#include "UI.hpp"
#include "Game.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Entity.hpp"
#include "Level.hpp"

UI::UI()
{
	info("UI constructor called.\n");
	// std::vector<int> indexes(50);
	// std::iota(indexes.begin(), indexes.end(), 1);
	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 8; y++)
	// 	{
	// 		if (tilemap[x][y])
	// 		{
	// 			tilemap[x][y] = indexes[rand() % indexes.size()];
	// 			indexes.erase(std::find(indexes.begin(), indexes.end(), tilemap[x][y]));
	// 		}
	// 	}

	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 8; y++)
	// 	{
	// 		int index = tilemap[x][y];
	// 		if (index)
	// 		{
	// 			enemy[index - 1].rect.x = x * 96;
	// 			enemy[index - 1].rect.y = y * 96;
	// 		}
	// 	}
	this->i = 0;
	this->j = 1;
	this->cur_txt_pos = Vec2D(Game::win_w / 2.0, Game::win_h / 2.0 + 25);
	this->dynamic_txt = {
		"Hello there ",
		"This is 1-health game, so be careful to play ",
		"Press [Esc] to open setting ",
		"Now enter your password "};
	this->last_render_time = this->trans_time = SDL_GetTicks64();

	// background
	this->fg_size = screen->size + Vec2D(100);
	this->fg_pos = (screen->size - fg_size) / 2.0;
	this->bg_size = this->bg2_size = sprites["space"]->real_size;
	this->bg_pos = this->bg2_pos = -(bg_size - screen->size) / 2.0;

	// avatar
	this->ava_size = sprites["avatar"]->real_size;
	this->ava_pos = Vec2D((Game::win_w - ava_size.x) / 2.0, (Game::win_h / 3.0) - (ava_size.y / 2.0));

	// pass word box
	this->outer_pass_box.w = 0.25 * Game::win_w;
	this->outer_pass_box.h = 30;
	this->outer_pass_box.x = (Game::win_w - outer_pass_box.w) / 2.0;
	this->outer_pass_box.y = (2.0 * Game::win_h / 3.0) - (outer_pass_box.h / 2.0) - 50;
	this->inner_pass_box.w = outer_pass_box.w - 4;
	this->inner_pass_box.h = outer_pass_box.h - 4;
	this->inner_pass_box.x = outer_pass_box.x + (outer_pass_box.w - inner_pass_box.w) / 2.0;
	this->inner_pass_box.y = outer_pass_box.y + (outer_pass_box.h - inner_pass_box.h) / 2.0;
	opb_color = ipb_color = Color::ice_blue(255);
}

UI::~UI() { info("UI destructor called.\n"); }

Vec2D UI::getPassBoxPos() const { return Vec2D(outer_pass_box.x, outer_pass_box.y); }
Vec2D UI::getPassBoxSize() const { return Vec2D(outer_pass_box.w, outer_pass_box.h); }
SDL_Color UI::getPassBoxBorderColor() const { return opb_color; }
SDL_Color UI::getPassBoxColor() const { return ipb_color; }
void UI::setPassBoxBorderColor(const SDL_Color &color) { opb_color = color; }
void UI::setPassBoxColor(const SDL_Color &color) { ipb_color = color; }
void UI::setDynamicText(const std::vector<std::string> &dt)
{
	dynamic_txt.clear();
	for (int i = 0; i < dt.size(); i++)
		dynamic_txt.push_back(dt[i]);
	i = 0;
	j = 1;
}

void UI::updateGamePlay()
{
	updateBackground();
	updateEnemies();
	updatePlayer();
}

void UI::drawGamePlay()
{
	drawBackground();
	drawEnemies();
	drawPlayer();
}

void UI::updateMenu()
{
	updateBackground();
	updatePlayer();
}

void UI::drawMenu()
{
	drawBackground();
	screen->drawSprite(*sprites["avatar"], ava_pos, ava_size);

	SDL_SetRenderDrawColor(Game::renderer, opb_color.r, opb_color.g, opb_color.b, opb_color.a);
	SDL_RenderFillRectF(Game::renderer, &outer_pass_box);
	SDL_SetRenderDrawColor(Game::renderer, ipb_color.r, ipb_color.g, ipb_color.b, ipb_color.a);
	SDL_RenderFillRectF(Game::renderer, &inner_pass_box);

	// render dynamic text
	if (i < dynamic_txt.size())
		drawDynamicText();
	else if (event->is_txt_entered)
	{
		Game::state = play;
		sound->playSoundEffect("unlock", general);
	}
	screen->drawText(cur_txt, cur_txt_pos, true, 24);

	screen->drawText(
		(!event->isTextInputEmpty()) ? std::string(event->cur_txt_inp.size(), '*') : "It's ok if type anything.",
		Vec2D(inner_pass_box.x + 1, inner_pass_box.y + 2), false, 18, "ui", false,
		(!event->isTextInputEmpty()) ? Color::black(255) : Color::blue_gray(255));
	drawPlayer();
}

void UI::updateSetting()
{
}

void UI::drawSetting()
{
}

void UI::updateGameOver()
{
	updateBackground();
}

void UI::drawGameOver()
{
	drawBackground();
}

void UI::drawDynamicText(const Uint64 &delay_per_chr, const Uint64 &delay_per_str)
{
	Uint64 cur_render_time = SDL_GetTicks64();
	if (cur_render_time - last_render_time >= render_time)
	{
		cur_txt = dynamic_txt[i].substr(0, j - 1);
		if (cur_txt == dynamic_txt[i].substr(0, dynamic_txt[i].size() - 1))
		{
			i++;
			j = 1;
			render_time = delay_per_str;
		}
		else
		{
			j++;
			render_time = delay_per_chr;
		}
		last_render_time = cur_render_time;
	}
}

void UI::updateBackground()
{
	if (Game::state == start)
	{
		fg_pos.x = -player->pos.x * 25 / float(Game::win_w);
		fg_pos.y = -player->pos.y * 25 / float(Game::win_h);
	}
	else if (Game::state == play)
	{
		goal_bg_pos.x = goal_bg2_pos.x = fg_pos.x = -player->pos.x * 100 / float(Game::win_w);
		goal_bg_pos.y = goal_bg2_pos.y = fg_pos.y = -player->pos.y * 100 / float(Game::win_h);
		bg_pos.x = lerp(goal_bg_pos.x, bg_pos.x, Game::deltaTime * 10);
		bg_pos.y = lerp(goal_bg_pos.y, bg_pos.y, Game::deltaTime * 10);
		bg2_pos.x = lerp(goal_bg2_pos.x, bg2_pos.x, Game::deltaTime * 20);
		bg2_pos.y = lerp(goal_bg2_pos.y, bg2_pos.y, Game::deltaTime * 20);
	}
	else if (Game::state == pause)
	{
	}
	else // game over
	{
		SDL_SetRenderDrawColor(Game::renderer, 0, 138, 229, 255);
	}
}

void UI::drawBackground()
{
	if (Game::state == start)
	{
		screen->drawSprite(*sprites["full blur"], fg_pos, fg_size);
	}
	else if (Game::state == play)
	{
		screen->drawSprite(*sprites["space"], bg_pos, bg_size);
		screen->drawSprite(*sprites["stars"], bg2_pos, bg2_size);
		screen->drawSprite(*sprites["flower"], fg_pos, fg_size);
	}
	else if (Game::state == pause)
	{
	}
	else // game over
	{
		screen->drawText(":)", Vec2D(), false, 72);
	}
}

void UI::updateEnemies()
{
	if (lvs.empty()) // game over
	{
		log("end game!\n");
	}
	else
	{
		level->spawnEnemyPer(Enemy::spawn_time);
		for (int i = 0; i < enemies.size(); i++) // current displayed enemy
		{
			enemies[i]->showName();
			enemies[i]->move();
			enemies[i]->attack(player);
			enemies[i]->takeDamage(i);
		}
	}
}

void UI::drawEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		screen->drawSprite(*sprites[enemies[i]->id], enemies[i]->pos, enemies[i]->size);
		if (!enemies[i]->name.empty())
			enemies[i]->name_size = screen->drawText(enemies[i]->name, enemies[i]->name_pos, false, 18, "ui", true, enemies[i]->name_color);
		drawLine(player->pos, enemies[i]->pos, (i != player->index) ? Color::white(0) : Color::red(0));
	}
}

void UI::updatePlayer()
{
	if (Game::state == start || Game::state == pause)
	{
		player->pos = event->mouse_pos;
	}
	else // game play
	{
		player->move();
		player->attackNearestEnemy();
		player->updateRotation();
		player->updateScore();
	}
}

void UI::drawPlayer()
{
	if (Game::state == start || Game::state == pause)
	{
		screen->drawSprite(*sprites["arrow"], player->pos, player->size, 1, player->cur_frame, player->cur_layer, player->angle);
	}
	else // game play
	{
		for (int i = 0; i < player->bullets.size(); i++)
			screen->drawSprite(*sprites["bullet"], player->bullets[i]->pos, player->bullets[i]->size, 1, 0, 0, player->bullets[i]->angle);

		screen->drawSprite(*sprites["arrow"], player->pos, player->size, 1, player->cur_frame, player->cur_layer, player->angle);

		for (int i = 0; i < player->circles.size(); i++)
			screen->drawSprite(*sprites["reticle"], player->circles[i]->pos, player->circles[i]->size, 1, 0, 0, player->circles[i]->angle);
		for (int i = 0; i < player->dead_zones.size(); i++)
			screen->drawSprite(*sprites["emp"], player->dead_zones[i]->pos, player->dead_zones[i]->size, 1, 0, 0, player->dead_zones[i]->angle);
	}
}