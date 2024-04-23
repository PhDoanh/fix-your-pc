#include "UI.hpp"
#include "Game.hpp"
#include "Event.hpp"
#include "Screen.hpp"
#include "Sound.hpp"
#include "Level.hpp"
#include "Entity.hpp"

UI::UI()
{
	info("UI constructor called.\n");

	// map_size = screen->size / small;

	// tilemap = {
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{1, 1, 0, 0, 0, 0, 0, 0},
	// 	{1, 1, 0, 0, 1, 1, 1, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0},
	// 	{1, 1, 0, 1, 1, 1, 0, 0},
	// 	{1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1}};

	// std::vector<int> idxs(28);
	// std::iota(idxs.begin(), idxs.end(), 1);
	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 8; y++)
	// 	{
	// 		if (tilemap[x][y])
	// 		{
	// 			tilemap[x][y] = idxs[rand() % idxs.size()];
	// 			idxs.erase(std::find(idxs.begin(), idxs.end(), tilemap[x][y]));
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
	this->last_render_time = this->last_trans_time = SDL_GetTicks64();

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
	this->opb_color = this->ipb_color = Color::ice_blue(255);

	this->saved_screen = nullptr;
	this->pause_bg = {0.0, 0.0, float(Game::win_w), float(Game::win_h)};
	this->margin = Vec2D(100);
	this->num_of_cells = Vec2D(4, 11);
	this->layout_size = screen->size - margin * 2.0;
	this->cell_size.x = int(layout_size.x / num_of_cells.x);
	this->cell_size.y = int(layout_size.y / num_of_cells.y);
	this->layout_pos.x = margin.x + (layout_size.x - (num_of_cells.x * cell_size.x)) / 2.0;
	this->layout_pos.y = margin.y + (layout_size.y - (num_of_cells.y * cell_size.y)) / 2.0;
	this->order = 1;
	elements["Settings"] = new UIElement(center, 48);
	elements["Music:"] = new UIElement(left, 36);
	elements["- 50% +"] = new UIElement(center, 36);
	elements["Sound:"] = new UIElement(left, 36);
	elements["- 50% +"] = new UIElement(center, 36);
	elements["Numbers:"] = new UIElement(left, 36);
	elements["[x]"] = new UIElement(center, 36);
	elements["Case Sensitive:"] = new UIElement(left, 36);
	elements["[ ]"] = new UIElement(center, 36);
	elements["Punctuations and Symbols:"] = new UIElement(left, 36);
	elements["[x]"] = new UIElement(center, 36);
	elements["Custom text:"] = new UIElement(left, 36);
	elements["Resume"] = new UIElement(center, 36);
	elements["Lock Screen"] = new UIElement(center, 36);
	elements["Shutdown"] = new UIElement(center, 36);
	elements["High Scores"] = new UIElement(center, 36);
	for (auto &&high_score : high_scores)
	{
		elements[std::to_string(order) + ". " + high_score.second] = new UIElement(left, 36);
		elements[std::to_string(high_score.first)] = new UIElement(right, 36);
		this->order++;
	}
	this->order = 1;

	this->count_down_time = 3; // seconds
}

UI::~UI()
{
	info("UI destructor called.\n");

	for (auto &&element : elements)
	{
		delete element.second;
		element.second = nullptr;
	}
	SDL_DestroyTexture(saved_screen);
}

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

void UI::updateGameReady()
{
	updateBackground();
	updatePlayer();
}

void UI::drawGameReady()
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
	screen->drawText(cur_txt, cur_txt_pos, center, 24);

	screen->drawText(
		(!event->isTextInputEmpty()) ? std::string(event->cur_txt_inp.size(), '*') : "It's ok if type anything.",
		Vec2D(inner_pass_box.x, inner_pass_box.y + inner_pass_box.h / 2.0), left, 18, "ui", false,
		(!event->isTextInputEmpty()) ? Color::black(255) : Color::blue_gray(255));
	drawPlayer();
}

void UI::updateGameStart()
{
}

void UI::drawGameStart()
{
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

void UI::updateGamePause()
{
	updateBackground();
	elements["Settings"]->pos = Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 0.5 * cell_size.y);
	elements["Music:"]->pos = Vec2D(layout_pos.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["- 50% +"]->pos = Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Sound:"]->pos = Vec2D(layout_pos.x, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["- 50% +"]->pos = Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Numbers:"]->pos = Vec2D(layout_pos.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["[x]"]->pos = Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Case Sensitive:"]->pos = Vec2D(layout_pos.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["[ ]"]->pos = Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Punctuations and Symbols:"]->pos = Vec2D(layout_pos.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["[x]"]->pos = Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Custom text:"]->pos = Vec2D(layout_pos.x, layout_pos.y + 7.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Resume"]->pos = Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 8.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Lock Screen"]->pos = Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 9.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["Shutdown"]->pos = Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	elements["High Scores"]->pos = Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
	for (auto &&high_score : high_scores)
	{
		elements[std::to_string(order) + ". " + high_score.second]->pos = Vec2D(layout_pos.x + 2.0 / num_of_cells.x * layout_size.x, layout_pos.y + (order + 1) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
		elements[std::to_string(high_score.first)]->pos = Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x + cell_size.x, layout_pos.y + (order + 1) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y);
		order++;
	}
	for (auto &&element : elements)
	{
		if (event->isHoverOn(element.second->pos, element.second->pos + element.second->size))
			element.second->color = Color::light_orange(255);
		else
			element.second->color = Color::white(255);
	}

	order = 1;

	updatePlayer();
}

void UI::drawGamePause()
{
	screen->drawSprite(*sprites["avatar"], ava_pos, ava_size);
	drawBackground();
	drawRect(layout_pos, layout_size);
	for (auto &&element : elements)
		screen->drawText(element.first, element.second->pos, element.second->align, element.second->font_size, "ui", false, element.second->color);
	drawPlayer();
}

void UI::updateGameOver()
{
	if (count_down_time > 0)
	{
		Uint64 cur_time = SDL_GetTicks64();
		if (cur_time - last_trans_time > 1000)
		{
			count_down_time--;
			last_trans_time = cur_time;
		}

		updateBackground();
	}
	else
		Game::running = false;
}

void UI::drawGameOver()
{
	drawBackground();
	screen->drawText(":)", Vec2D(), top_left, 48);
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
	if (Game::state == ready || Game::state == start)
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
		SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 127);
	}
	else // game over
		SDL_SetRenderDrawColor(Game::renderer, 0, 138, 229, 255);
}

void UI::drawBackground()
{
	if (Game::state == ready)
		screen->drawSprite(*sprites["full blur"], fg_pos, fg_size);
	else if (Game::state == start)
		screen->drawSprite(*sprites["full"], fg_pos, fg_size);
	else if (Game::state == play)
	{
		screen->drawSprite(*sprites["space"], bg_pos, bg_size);
		screen->drawSprite(*sprites["stars"], bg2_pos, bg2_size);
		screen->drawSprite(*sprites["flower"], fg_pos, fg_size);
	}
	else if (Game::state == pause)
	{

		SDL_RenderFillRectF(Game::renderer, &pause_bg);
	}
	else // game over
	{
	}
}

void UI::updateEnemies()
{
	if (Game::state == start)
	{
	}
	else // game play
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
}

void UI::drawEnemies()
{
	if (Game::state == start)
	{
	}
	else // game play
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			screen->drawSprite(*sprites[enemies[i]->id], enemies[i]->pos, enemies[i]->size);
			if (!enemies[i]->name.empty())
				enemies[i]->name_size = screen->drawText(enemies[i]->name, enemies[i]->name_pos, top_left, 18, "ui", true, enemies[i]->name_color);
			drawLine(player->pos, enemies[i]->pos, (i != player->index) ? Color::white(0) : Color::red(0));
		}
	}
}

void UI::updatePlayer()
{
	if (Game::state == ready || Game::state == start || Game::state == pause)
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
	if (Game::state == ready || Game::state == start || Game::state == pause)
		screen->drawSprite(*sprites["arrow"], player->pos, player->size, 1, top_left, player->cur_frame, player->cur_layer, player->angle);
	else // game play
	{
		for (int i = 0; i < player->bullets.size(); i++)
			screen->drawSprite(*sprites["bullet"], player->bullets[i]->pos, player->bullets[i]->size, 1, top_left, 0, 0, player->bullets[i]->angle);

		screen->drawSprite(*sprites["arrow"], player->pos, player->size, 1, top_left, player->cur_frame, player->cur_layer, player->angle);

		for (int i = 0; i < player->circles.size(); i++)
			screen->drawSprite(*sprites["reticle"], player->circles[i]->pos, player->circles[i]->size, 1, top_left, 0, 0, player->circles[i]->angle);
		for (int i = 0; i < player->dead_zones.size(); i++)
			screen->drawSprite(*sprites["emp"], player->dead_zones[i]->pos, player->dead_zones[i]->size, 1, top_left, 0, 0, player->dead_zones[i]->angle);
	}
}

void UI::saveCurScreen()
{
	saved_screen = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Game::win_w, Game::win_h);
	SDL_SetRenderTarget(Game::renderer, saved_screen);
	SDL_RenderCopy(Game::renderer, nullptr, nullptr, nullptr);
	SDL_SetRenderTarget(Game::renderer, nullptr);
}