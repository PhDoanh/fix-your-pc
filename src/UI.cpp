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
	// this->i = 0;
	// this->j = 1;
	this->cur_txt_pos = Vec2D(Game::win_w / 2.0, Game::win_h / 2.0 + 25);
	// this->dynamic_txt = {
	// 	"Hello there ",
	// 	"This is 1-health game, so be careful to play ",
	// 	"Press [Esc] to open setting ",
	// 	"Now enter your password "};
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
	// this->opb_color = this->ipb_color = Color::ice_blue(255);

	this->saved_screen = nullptr;
	this->pause_bg = {0.0, 0.0, float(Game::win_w), float(Game::win_h)};
	this->margin = Vec2D(100);
	this->num_of_cells = Vec2D(4, 11);
	this->layout_size = screen->size - margin * 2.0;
	this->cell_size.x = int(layout_size.x / num_of_cells.x);
	this->cell_size.y = int(layout_size.y / num_of_cells.y);
	this->layout_pos.x = margin.x + (layout_size.x - (num_of_cells.x * cell_size.x)) / 2.0;
	this->layout_pos.y = margin.y + (layout_size.y - (num_of_cells.y * cell_size.y)) / 2.0;
	// this->order = 1;
	options["1"] = new TextElement("Settings", 48, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 0.5 * cell_size.y));

	options["2"] = new TextElement("Music:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["3"] = new TextElement("-", 60, left, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 50, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["4"] = new TextElement("50%", 36, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["5"] = new TextElement("+", 60, right, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + cell_size.x - 50, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["6"] = new TextElement("Sound:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["7"] = new TextElement("-", 60, left, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 50, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["8"] = new TextElement("50%", 36, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["9"] = new TextElement("+", 60, right, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + cell_size.x - 50, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["10"] = new TextElement("Numbers:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["11"] = new TextElement("[x]", 48, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["12"] = new TextElement("Case Sensitive:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["13"] = new TextElement("[ ]", 48, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["14"] = new TextElement("Punctuations and Symbols:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["15"] = new TextElement("[ ]", 48, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["16"] = new TextElement("Custom text:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 7.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["17"] = new TextElement("Resume", 36, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 8.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["18"] = new TextElement("Lock Screen", 36, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 9.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["19"] = new TextElement("Shutdown", 36, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["20"] = new TextElement("High Scores", 36, center, Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	// for (auto &&high_score : high_scores)
	// {
	// 	options[]=std::to_string(order) + ". " + high_score.second, new TextElement(left, 36, Vec2D(layout_pos.x + 2.0 / num_of_cells.x * layout_size.x, layout_pos.y + (order + 1) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	// 	options[]=std::to_string(high_score.first), new TextElement(right, 36, Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x + cell_size.x, layout_pos.y + (order + 1) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	// 	this->order++;
	// }

	this->count_down_time = 33; // seconds
}

UI::~UI()
{
	info("UI destructor called.\n");

	for (auto &&option : options)
	{
		delete option.second;
		option.second = nullptr;
	}

	SDL_DestroyTexture(saved_screen);
}

void UI::loadElements()
{
	int value;
	std::string key, sentence, word;

	this->i = 0;
	this->j = 1;
	this->dynamic_txt = {
		"Hello there ",
		"This is 1-health game, so be careful to play ",
		"Press [Esc] to open setting ",
		"Now enter your password "};
	this->opb_color = this->ipb_color = Color::ice_blue(255);
	this->order = 1;

	this->tilemap = {
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
		{1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
		{1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	std::vector<int> idxs(50);
	std::iota(idxs.begin(), idxs.end(), 1);
	for (int y = 0; y < 9; y++)
		for (int x = 0; x < 16; x++)
		{
			if (this->tilemap[y][x])
			{
				this->tilemap[y][x] = idxs[rand() % idxs.size()];
				idxs.erase(std::find(idxs.begin(), idxs.end(), this->tilemap[y][x]));
			}
		}
	for (int y = 0; y < 9; y++)
		for (int x = 0; x < 16; x++)
		{
			if (this->tilemap[y][x])
			{
				Enemy *new_enemy = new Enemy("", "enemy" + std::to_string(this->tilemap[y][x]), Vec2D(x * 96, y * 96), Vec2D(medium), Vec2D(1.5));
				enemies.emplace_back(new_enemy);
			}
		}

	// load settings
	Game::data.open("res/game_data/settings.txt");
	while (!Game::data.eof())
	{
		Game::data >> key >> value;
		settings[key] = value;
	}
	Game::data.close();

	// load levels
	Game::data.open("res/game_data/levels.txt");
	while (getline(Game::data, sentence, '.'))
	{
		std::queue<std::string> lv;
		std::stringstream ss(sentence);
		while (ss >> word)
			lv.push(word);
		lvs.push(lv);
	}
	Game::data.close();

	// load high scores
	Game::data.open("res/game_data/high_scores.txt");
	while (!Game::data.eof())
	{
		Game::data >> key >> value;
		high_scores.insert({value, key});
	}
	Game::data.close();
	while (high_scores.size() >= 5)
		high_scores.erase(high_scores.begin());

	event->is_txt_entered = false;

	player = new Player("player", Vec2D(Game::win_w / 2.0, Game::win_h / 2.0), Vec2D(43, 60));
	player->shield.time = 7000;		  // 7s
	player->shield_state.time = 2000; // 2s

	Game::state = ready;
	Game::prev_state = pause;
}

void UI::deleteElements()
{
	Game::data.open("res/game_data/high_scores.txt", std::ios::trunc | std::ios::out);
	for (auto &&hs : high_scores)
		Game::data << hs.second << ' ' << hs.first << '\n';
	Game::data.close();

	for (int i = 0; i < enemies.size(); i++)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}
	enemies.clear();

	delete player;
	player = nullptr;

	while (!lvs.empty())
		lvs.pop();
	while (!level->lv.empty())
		level->lv.pop();
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
void UI::setShutdownTime(const int &shutdown_time) { count_down_time = shutdown_time; }

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
		sound->playSoundEffect("unlocked", general);
		Game::state = start;
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
	updateBackground();
	updatePlayer();
}

void UI::drawGameStart()
{
	drawBackground();

	for (int i = 0; i < enemies.size(); i++)
		screen->drawSprite(*sprites[enemies[i]->id], enemies[i]->pos, enemies[i]->size);

	drawPlayer();
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
	// for (auto &&option : options)
	// {
	// 	if (event->isHoverOn(option.second->real_pos, option.second->size))
	// 		option.second->color = Color::light_orange(255);
	// 	else
	// 		option.second->color = Color::white(255);
	// }
	updatePlayer();
}

void UI::drawGamePause()
{
	drawBackground();
	drawRect(layout_pos, layout_size);

	for (auto &&option : options)
	{
		SDL_FRect rect = screen->drawText(option.second->text, option.second->pos, option.second->align, option.second->font_size, "ui", false, option.second->color);
		option.second->real_pos = Vec2D(rect.x, rect.y);
		option.second->size = Vec2D(rect.w, rect.h);
		drawRect(option.second->real_pos, option.second->size);
	}

	drawHighScores();
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
	if (count_down_time > 3)
	{
		drawBackground();
		drawRect(layout_pos, layout_size);

		screen->drawText(":)", Vec2D(), top_left, 48);
	}
	else
	{
	}
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
		fg_pos.x = -event->mouse_pos.x * 25 / float(Game::win_w);
		fg_pos.y = -event->mouse_pos.y * 25 / float(Game::win_h);
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
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 191);
	}
	else // game over
	{
		if (count_down_time > 3)
			SDL_SetRenderDrawColor(Game::renderer, 0, 138, 229, 255);
		else
			SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
	}
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
		SDL_RenderCopy(Game::renderer, saved_screen, nullptr, nullptr);
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
			{
				SDL_FRect rect = screen->drawText(enemies[i]->name, enemies[i]->name_pos, top_left, 18, "ui", true, enemies[i]->name_color);
				enemies[i]->name_size = Vec2D(rect.w, rect.h);
			}
			drawLine(player->pos, enemies[i]->pos, (i != player->index) ? Color::white(0) : Color::red(0));
		}
	}
}

void UI::updatePlayer()
{
	if (Game::state == ready || Game::state == start || Game::state == pause)
	{
		player->goal_angle = -26;
		player->updateRotation();
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
		screen->drawSprite(*sprites["arrow"], event->mouse_pos, player->size, 1, top_left, player->cur_frame, player->cur_layer, player->angle);
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
	SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, Game::win_w, Game::win_h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_RenderReadPixels(Game::renderer, nullptr, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);
	saved_screen = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);
}

void UI::drawHighScores()
{
	for (auto &&high_score : high_scores)
	{
		screen->drawText(
			std::to_string(order) + ". " + high_score.second,
			Vec2D(layout_pos.x + 2.0 / num_of_cells.x * layout_size.x, layout_pos.y + (order + 1) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y),
			left, 36, "ui", false, Color::white(255));
		screen->drawText(
			std::to_string(high_score.first),
			Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x + cell_size.x, layout_pos.y + (order + 1) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y),
			right, 36, "ui", false, Color::white(255));
		// drawRect(high_score.second->real_pos, high_score.second->size);
		order++;
	}
	order = 1;
}