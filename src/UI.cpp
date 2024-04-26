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

	this->cur_txt_pos = Vec2D(Game::win_w / 2.0, Game::win_h / 2.0 + 25);
	this->last_render_time = this->last_trans_time = SDL_GetTicks64();

	// background
	this->fg_size = screen->size + Vec2D(100);
	this->fg_pos = (screen->size - fg_size) / 2.0;
	this->bg_size = this->bg2_size = sprites["space"]->real_size;
	this->bg_pos = this->bg2_pos = -(bg_size - screen->size) / 2.0;

	// avatar
	this->ava_size = sprites["avatar"]->real_size;
	this->ava_pos = Vec2D((Game::win_w - ava_size.x) / 2.0, (Game::win_h / 3.0) - (ava_size.y / 2.0));

	// password box
	this->outer_pass_box.w = 0.25 * Game::win_w;
	this->outer_pass_box.h = 30;
	this->outer_pass_box.x = (Game::win_w - outer_pass_box.w) / 2.0;
	this->outer_pass_box.y = (2.0 * Game::win_h / 3.0) - (outer_pass_box.h / 2.0) - 50;
	this->inner_pass_box.w = outer_pass_box.w - 4;
	this->inner_pass_box.h = outer_pass_box.h - 4;
	this->inner_pass_box.x = outer_pass_box.x + (outer_pass_box.w - inner_pass_box.w) / 2.0;
	this->inner_pass_box.y = outer_pass_box.y + (outer_pass_box.h - inner_pass_box.h) / 2.0;

	this->saved_screen = nullptr;
	this->trans_screen = nullptr;
	this->pause_bg = {0.0, 0.0, float(Game::win_w), float(Game::win_h)};
	this->margin = Vec2D(100);
	this->num_of_cells = Vec2D(4, 11);
	this->layout_size = screen->size - margin * 2.0;
	this->cell_size.x = int(layout_size.x / num_of_cells.x);
	this->cell_size.y = int(layout_size.y / num_of_cells.y);
	this->layout_pos.x = margin.x + (layout_size.x - (num_of_cells.x * cell_size.x)) / 2.0;
	this->layout_pos.y = margin.y + (layout_size.y - (num_of_cells.y * cell_size.y)) / 2.0;
	options["1"] = new TextElement("Settings", 48, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 0.5 * cell_size.y));

	options["2"] = new TextElement("Music:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["3"] = new TextElement("-", 60, left, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 50, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);
	options["4"] = new TextElement("50%", 36, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["5"] = new TextElement("+", 60, right, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + cell_size.x - 50, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);

	options["21"] = new TextElement("Custom music:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 2.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["6"] = new TextElement("Sound:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["7"] = new TextElement("-", 60, left, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 50, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);
	options["8"] = new TextElement("50%", 36, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["9"] = new TextElement("+", 60, right, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + cell_size.x - 50, layout_pos.y + 3.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);

	options["10"] = new TextElement("Numbers:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["11"] = new TextElement("[x]", 48, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);

	options["12"] = new TextElement("Case Sensitive:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["13"] = new TextElement("[ ]", 48, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);

	options["14"] = new TextElement("Punctuations and Symbols:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	options["15"] = new TextElement("[ ]", 48, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);

	options["16"] = new TextElement("Custom text:", 36, left, Vec2D(layout_pos.x, layout_pos.y + 7.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	options["17"] = new TextElement("Resume", 36, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 8.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);
	options["18"] = new TextElement("Lock Screen", 36, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 9.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);
	options["19"] = new TextElement("Shutdown", 36, center, Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), true);

	options["20"] = new TextElement("High Scores", 36, center, Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x, layout_pos.y + 1.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	over_infos["1"] = new TextElement("High Scores", 36, center, Vec2D(layout_pos.x + 1.0 / num_of_cells.x * layout_size.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	over_infos["2"] = new TextElement("True Typing Stats", 36, center, Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	over_infos["3"] = new TextElement("Longest Streak:", 36, center, Vec2D(layout_pos.x + 2.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	over_infos["4"] = new TextElement("0", 36, center, Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 5.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	over_infos["5"] = new TextElement("Current Streak:", 36, center, Vec2D(layout_pos.x + 2.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	over_infos["6"] = new TextElement("0", 36, center, Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 6.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	over_infos["7"] = new TextElement("Accuracy:", 36, center, Vec2D(layout_pos.x + 2.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 7.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));
	over_infos["8"] = new TextElement("0", 36, center, Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x + 0.5 * cell_size.x, layout_pos.y + 7.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y));

	// music path box
	this->outer_music_path_box.w = cell_size.x - 25;
	this->outer_music_path_box.h = cell_size.y - 25;
	this->outer_music_path_box.x = layout_pos.x + cell_size.x + 0.5 * (cell_size.x - outer_music_path_box.w);
	this->outer_music_path_box.y = layout_pos.y + 2 * cell_size.y + 0.5 * (cell_size.y - outer_music_path_box.h);
	this->inner_music_path_box.w = outer_music_path_box.w - 4;
	this->inner_music_path_box.h = outer_music_path_box.h - 4;
	this->inner_music_path_box.x = outer_music_path_box.x + (outer_music_path_box.w - inner_music_path_box.w) / 2.0;
	this->inner_music_path_box.y = outer_music_path_box.y + (outer_music_path_box.h - inner_music_path_box.h) / 2.0;

	// txt path box
	this->outer_txt_path_box.w = cell_size.x - 25;
	this->outer_txt_path_box.h = cell_size.y - 25;
	this->outer_txt_path_box.x = layout_pos.x + cell_size.x + 0.5 * (cell_size.x - outer_music_path_box.w);
	this->outer_txt_path_box.y = layout_pos.y + 7 * cell_size.y + 0.5 * (cell_size.y - outer_music_path_box.h);
	this->inner_txt_path_box.w = outer_txt_path_box.w - 4;
	this->inner_txt_path_box.h = outer_txt_path_box.h - 4;
	this->inner_txt_path_box.x = outer_txt_path_box.x + (outer_txt_path_box.w - inner_txt_path_box.w) / 2.0;
	this->inner_txt_path_box.y = outer_txt_path_box.y + (outer_txt_path_box.h - inner_txt_path_box.h) / 2.0;

	this->count_down_time = 18; // seconds
	this->play_once = true;
	this->shutdown = false;

	// default paths
	this->cur_music_path_txt = "res/music/orientation.ogg";
	this->cur_txt_path_txt = "res/game_data/levels.txt";
}

UI::~UI()
{
	info("UI destructor called.\n");

	for (auto &&option : options)
	{
		delete option.second;
		option.second = nullptr;
	}

	for (auto &&over_info : over_infos)
	{
		delete over_info.second;
		over_info.second = nullptr;
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
	this->pass_box = false;
	this->music_box = false;
	this->txt_box = false;
	this->opb_color = this->ipb_color = Color::ice_blue(255);
	this->ompb_color = this->impb_color = Color::ice_blue(255);
	this->otpb_color = this->itpb_color = Color::ice_blue(255);
	this->order = 0;
	Level::level_order = 1;
	Enemy::spawn_time = 3000;
	Enemy::last_spawn_time = SDL_GetTicks64();
	this->trans_alpha = 255;

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
				static_enemies.emplace_back(new_enemy);
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
	Game::data.open(cur_txt_path_txt);
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

	sound->loadMusic(cur_music_path_txt, cur_music_path_txt);
	Mix_VolumeMusic(settings["musicVol"]);
	for (auto &&sound : sounds)
		Mix_VolumeChunk(sound.second, settings["soundVol"]);
	options["4"]->text = std::to_string(settings["musicVol"]) + "%";
	options["8"]->text = std::to_string(settings["soundVol"]) + "%";
	options["11"]->text = settings["enableNum"] ? "[x]" : "[ ]";
	options["13"]->text = settings["enablePunct"] ? "[x]" : "[ ]";
	options["15"]->text = settings["enableCaseSensitive"] ? "[x]" : "[ ]";

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
	sound->stopMusic();
}

Vec2D UI::getPassBoxPos() const { return Vec2D(outer_pass_box.x, outer_pass_box.y); }
Vec2D UI::getPassBoxSize() const { return Vec2D(outer_pass_box.w, outer_pass_box.h); }
SDL_Color UI::getPassBoxBorderColor() const { return opb_color; }
SDL_Color UI::getPassBoxColor() const { return ipb_color; }
void UI::setPassBoxBorderColor(const SDL_Color &color) { opb_color = color; }
void UI::setPassBoxColor(const SDL_Color &color) { ipb_color = color; }
void UI::turnOnPassBox() { pass_box = true; }
void UI::turnOffPassBox() { pass_box = false; }
bool UI::getPassBoxState() const { return pass_box; }

Vec2D UI::getMusicPathBoxPos() const { return Vec2D(outer_music_path_box.x, outer_music_path_box.y); }
Vec2D UI::getMusicPathBoxSize() const { return Vec2D(outer_music_path_box.w, outer_music_path_box.h); }
SDL_Color UI::getMusicPathBoxBorderColor() const { return ompb_color; }
SDL_Color UI::getMusicPathBoxColor() const { return impb_color; }
void UI::setMusicPathBoxBorderColor(const SDL_Color &color) { ompb_color = color; }
void UI::setMusicPathBoxColor(const SDL_Color &color) { impb_color = color; }
void UI::turnOnMusicPathBox() { music_box = true; }
void UI::turnOffMusicPathBox() { music_box = false; }
bool UI::getMusicPathBoxState() const { return music_box; }

Vec2D UI::getTextPathBoxPos() const { return Vec2D(outer_txt_path_box.x, outer_txt_path_box.y); }
Vec2D UI::getTextPathBoxSize() const { return Vec2D(outer_txt_path_box.w, outer_txt_path_box.h); }
SDL_Color UI::getTextPathBoxBorderColor() const { return otpb_color; }
SDL_Color UI::getTextPathBoxColor() const { return itpb_color; }
void UI::setTextPathBoxBorderColor(const SDL_Color &color) { otpb_color = color; }
void UI::setTextPathBoxColor(const SDL_Color &color) { itpb_color = color; }
void UI::turnOnTextPathBox() { txt_box = true; }
void UI::turnOffTextPathBox() { txt_box = false; }
bool UI::getTextPathBoxState() const { return txt_box; }

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

	if (i < dynamic_txt.size())
		drawDynamicText(cur_txt_pos);
	else if (event->is_txt_entered)
	{
		sound->playSoundEffect("unlocked", general);
		sound->playMusic(cur_music_path_txt);
		ui->saveCurScreen();
		player->size = Vec2D(43, 60);
		player->goal_angle = -26;
		Game::state = play;
	}
	cur_txt_box = screen->drawText(cur_txt, cur_txt_pos, center, 24);

	screen->drawText(
		(pass_box) ? std::string(event->cur_txt_inp.size(), '*') : "It's ok if type anything.",
		Vec2D(inner_pass_box.x, inner_pass_box.y + inner_pass_box.h / 2.0), left, 18, "ui", false,
		(pass_box) ? Color::black(255) : Color::blue_gray(255));

	drawTransScreen();
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

	for (int i = 0; i < static_enemies.size(); i++)
		screen->drawSprite(*sprites[static_enemies[i]->id], static_enemies[i]->pos, static_enemies[i]->size);
	drawDynamicText(Vec2D(Game::win_w / 2.0, Game::win_h / 2.0), center, 36);

	drawTransScreen();
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
	drawDynamicText(Vec2D(Game::win_w / 2.0, Game::win_h / 2.0), center, 48, Color::white(255));
	drawLine(Vec2D(cur_txt_box.x, cur_txt_box.y - 0.3 * cur_txt_box.h), Vec2D(cur_txt_box.x + cur_txt_box.w, cur_txt_box.y - 0.3 * cur_txt_box.h), Color::light_orange(255));
	drawLine(Vec2D(cur_txt_box.x, cur_txt_box.y + cur_txt_box.h + 0.3 * cur_txt_box.h), Vec2D(cur_txt_box.x + cur_txt_box.w, cur_txt_box.y + cur_txt_box.h + 0.3 * cur_txt_box.h), Color::light_orange(255));
	drawTransScreen();
	drawPlayer();
}

void UI::updateGamePause()
{
	updateBackground();
	updateStates();
	updatePlayer();
	for (auto &&high_score : high_scores)
		if (high_score.second == player->id)
		{
			high_scores.erase(high_score.first);
			high_scores.insert({(player->score + player->num_of_chrs + player->true_chrs) / 3, player->id});
			break;
		}
}

void UI::drawGamePause()
{
	drawBackground();

	SDL_SetRenderDrawColor(Game::renderer, ompb_color.r, ompb_color.g, ompb_color.b, ompb_color.a);
	SDL_RenderFillRectF(Game::renderer, &outer_music_path_box);
	SDL_SetRenderDrawColor(Game::renderer, impb_color.r, impb_color.g, impb_color.b, impb_color.a);
	SDL_RenderFillRectF(Game::renderer, &inner_music_path_box);
	SDL_SetRenderDrawColor(Game::renderer, otpb_color.r, otpb_color.g, otpb_color.b, otpb_color.a);
	SDL_RenderFillRectF(Game::renderer, &outer_txt_path_box);
	SDL_SetRenderDrawColor(Game::renderer, itpb_color.r, itpb_color.g, itpb_color.b, itpb_color.a);
	SDL_RenderFillRectF(Game::renderer, &inner_txt_path_box);

	screen->drawText(
		(music_box) ? event->cur_txt_inp : "Paste your music file path here.",
		Vec2D(inner_music_path_box.x, inner_music_path_box.y + inner_music_path_box.h / 2.0), left, 18, "ui", false,
		(music_box) ? Color::black(255) : Color::blue_gray(255));
	screen->drawText(
		(txt_box) ? event->cur_txt_inp : "Paste your text file path here.",
		Vec2D(inner_txt_path_box.x, inner_txt_path_box.y + inner_txt_path_box.h / 2.0), left, 18, "ui", false,
		(txt_box) ? Color::black(255) : Color::blue_gray(255));

	// drawRect(layout_pos, layout_size);
	for (auto &&option : options)
	{
		SDL_FRect rect = screen->drawText(option.second->text, option.second->pos, option.second->align, option.second->font_size, "ui", false, option.second->color);
		option.second->real_pos = Vec2D(rect.x, rect.y);
		option.second->size = Vec2D(rect.w, rect.h);
		// drawRect(option.second->real_pos, option.second->size);
	}
	drawHighScores(Vec2D(2, 2));

	drawLine(Vec2D(layout_pos.x, layout_pos.y + cell_size.y), Vec2D(layout_pos.x + layout_size.x, layout_pos.y + cell_size.y));																								 // top break line
	drawLine(Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + cell_size.y + 0.5 * cell_size.y), Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 8.0 / num_of_cells.y * layout_size.y - 0.5 * cell_size.y)); // split line
	drawLine(Vec2D(layout_pos.x + 0.25 * layout_size.x, layout_pos.y + 8.0 / num_of_cells.y * layout_size.y), Vec2D(layout_pos.x + 0.75 * layout_size.x, layout_pos.y + 8.0 / num_of_cells.y * layout_size.y));				 // bottom break line

	drawTransScreen();
	drawPlayer();
}

void UI::updateGameOver()
{
	if (i >= dynamic_txt.size() || shutdown)
	{
		if (count_down_time > 0)
		{
			Uint64 cur_time = SDL_GetTicks64();
			if (cur_time - last_trans_time > 1000)
			{
				count_down_time--;
				last_trans_time = cur_time;
			}
		}
		else
			Game::running = false;
	}
	over_infos["4"]->text = std::to_string(player->score) + " chars";
	over_infos["6"]->text = std::to_string(player->num_of_chrs) + " chars";
	over_infos["8"]->text = std::to_string(player->true_chrs / (player->true_chrs + player->wrong_chrs) * 100) + "%";
	updateBackground();
}

void UI::drawGameOver()
{
	if (count_down_time > 3)
	{
		drawBackground();
		// drawRect(layout_pos, layout_size);

		screen->drawSprite(*sprites["smile"], Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y), Vec2D(452, 222), 1, top_mid);
		screen->drawSprite(*sprites["game src"], Vec2D(layout_pos.x + 3.0 / num_of_cells.x * layout_size.x, layout_pos.y + 9.0 / num_of_cells.y * layout_size.y), Vec2D(2 * cell_size.y), 1, center);

		for (auto &&over_info : over_infos)
		{
			SDL_FRect rect = screen->drawText(over_info.second->text, over_info.second->pos, over_info.second->align, over_info.second->font_size, "ui", false, over_info.second->color);
			over_info.second->real_pos = Vec2D(rect.x, rect.y);
			over_info.second->size = Vec2D(rect.w, rect.h);
			// drawRect(over_info.second->real_pos, over_info.second->size);
		}
		drawHighScores(Vec2D(0, 5));
		if (i < dynamic_txt.size())
			drawDynamicText(Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), center, 24, Color::light_orange(255));
		else
			screen->drawText("Shutdown in " + std::to_string(count_down_time - 3) + "s", Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), center, 24, "ui", false, Color::light_orange(255));

		// draw borders
		drawLine(Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y + 0.5 * cell_size.y), Vec2D(layout_pos.x + 0.5 * layout_size.x, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y - 0.5 * cell_size.y)); // split line
		drawLine(Vec2D(cur_txt_box.x, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y), Vec2D(cur_txt_box.x + cur_txt_box.w, layout_pos.y + 10.0 / num_of_cells.y * layout_size.y));																   // break line
		drawLine(Vec2D(layout_pos.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y), Vec2D(layout_pos.x + layout_size.x, layout_pos.y + 4.0 / num_of_cells.y * layout_size.y));

		drawTransScreen();
	}
	else
	{
		if (play_once)
		{
			sound->playSoundEffect("shutdown", general);
			play_once = false;
		}
	}
}

void UI::saveCurScreen()
{
	SDL_Surface *surface1 = SDL_CreateRGBSurfaceWithFormat(0, Game::win_w, Game::win_h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0, Game::win_w, Game::win_h, 23, SDL_PIXELFORMAT_RGBA32);
	SDL_RenderReadPixels(Game::renderer, nullptr, SDL_PIXELFORMAT_RGBA32, surface1->pixels, surface1->pitch);
	SDL_RenderReadPixels(Game::renderer, nullptr, SDL_PIXELFORMAT_RGBA32, surface2->pixels, surface2->pitch);
	saved_screen = SDL_CreateTextureFromSurface(Game::renderer, surface1);
	trans_screen = SDL_CreateTextureFromSurface(Game::renderer, surface2);
	trans_alpha = 255;
	SDL_FreeSurface(surface1);
	SDL_FreeSurface(surface2);
}

void UI::setShutdownTime(const int &shutdown_time) { count_down_time = shutdown_time; }

void UI::drawHighScores(const Vec2D &pos)
{
	for (auto &&high_score : high_scores)
	{
		SDL_FRect rect1, rect2;
		rect1 = screen->drawText(
			" " + std::to_string(order + 1) + ". " + high_score.second,
			Vec2D(layout_pos.x + pos.x / num_of_cells.x * layout_size.x, layout_pos.y + (order + pos.y) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y),
			left, 36, "ui", false, (high_score.second == player->id) ? Color::light_orange(255) : Color::white(255));
		rect2 = screen->drawText(
			std::to_string(high_score.first) + " ",
			Vec2D(layout_pos.x + (pos.x + 1) / num_of_cells.x * layout_size.x + cell_size.x, layout_pos.y + (order + pos.y) / num_of_cells.y * layout_size.y + 0.5 * cell_size.y),
			right, 36, "ui", false, (high_score.second == player->id) ? Color::light_orange(255) : Color::white(255));
		// drawRect(Vec2D(rect1.x, rect1.y), Vec2D(rect1.w, rect1.h));
		// drawRect(Vec2D(rect2.x, rect2.y), Vec2D(rect2.w, rect2.h));
		order++;
	}
	order = 0;
}

void UI::setDynamicText(const std::vector<std::string> &dt)
{
	dynamic_txt.clear();
	for (int i = 0; i < dt.size(); i++)
		dynamic_txt.push_back(dt[i]);
	i = 0;
	j = 1;
}

void UI::drawDynamicText(const Vec2D &pos, const int &align, const int &font_size, SDL_Color txt_color, const Uint64 &delay_per_chr, const Uint64 &delay_per_str)
{
	if (i < dynamic_txt.size())
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
	cur_txt_box = screen->drawText(cur_txt, pos, align, font_size, "ui", false, txt_color);
}

void UI::updateStates()
{
	for (auto &&option : options)
		option.second->color = Color::white(255);

	if (event->mouse_pos.y <= margin.y + 2 * cell_size.y) // music
	{
		options["2"]->color = options["3"]->color = options["4"]->color = options["5"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 3 * cell_size.y) // custom music
	{
		options["21"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 4 * cell_size.y) // sound
	{
		options["6"]->color = options["7"]->color = options["8"]->color = options["9"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 5 * cell_size.y) // number
	{
		options["10"]->color = options["11"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 6 * cell_size.y) // case
	{
		options["12"]->color = options["13"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 7 * cell_size.y) // punct
	{
		options["14"]->color = options["15"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 8 * cell_size.y) // custom text
	{
		options["16"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 9 * cell_size.y) // resume
	{
		options["17"]->color = Color::light_orange(255);
	}
	else if (event->mouse_pos.y <= margin.y + 10 * cell_size.y) // lock screen
	{
		options["18"]->color = Color::light_orange(255);
	}
	else // shutdown
	{
		options["19"]->color = Color::light_orange(255);
	}
}

void UI::drawTransScreen()
{
	SDL_SetTextureBlendMode(trans_screen, SDL_BLENDMODE_BLEND);
	trans_alpha = (trans_alpha - 25 > 0) ? trans_alpha - 25 : 0;
	SDL_SetTextureAlphaMod(trans_screen, trans_alpha);
	SDL_RenderCopy(Game::renderer, trans_screen, nullptr, nullptr);
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
		if (lvs.empty() && enemies.empty()) // game over
		{
			sound->stopMusic();
			sound->playSoundEffect("notify", general);
			saveCurScreen();
			std::vector<std::string> dt = {
				"Congratulations! ",
				"You have fixed all your PC errors ",
				"You can now turn off your PC ",
				"Or play again by pressing [Esc] and selecting \"Lock Screen\" ",
				"Thank you for playing ",
				""};
			setDynamicText(dt);
			Game::state = start;
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
			// drawLine(player->pos, enemies[i]->pos, (i != player->index) ? Color::white(0) : Color::red(0));
		}
	}
}

void UI::updatePlayer()
{
	if (Game::state == ready || Game::state == start || Game::state == pause)
	{
		player->size = Vec2D(43, 60);
		player->goal_angle = -26;
		player->updateRotation();
		player->state = "arrow";
		if (Game::state == ready)
			if (event->isHoverOn(getPassBoxPos(), getPassBoxSize()))
			{
				player->size = Vec2D(64, 44);
				player->goal_angle = 0;
				player->updateRotation();
				player->state = "beam";
			}
		if (Game::state == start)
		{
			for (auto &&static_enemy : static_enemies)
				if (Rect::isCollide(static_enemy->pos, static_enemy->size, player->pos, player->size))
				{
					player->size = Vec2D(64);
					player->goal_angle = 0;
					player->updateRotation();
					player->state = "move";
				}
		}
		if (Game::state == pause)
		{
			for (auto &&option : options)
			{
				if (option.second->can_interact && event->isHoverOn(option.second->real_pos, option.second->size))
				{
					player->size = Vec2D(64);
					player->state = "link";
				}
			}
			if (event->isHoverOn(getMusicPathBoxPos(), getMusicPathBoxSize()) || event->isHoverOn(getTextPathBoxPos(), getTextPathBoxSize()))
			{
				player->size = Vec2D(64, 44);
				player->goal_angle = 0;
				player->updateRotation();
				player->state = "beam";
			}
		}
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
		screen->drawSprite(*sprites[player->state], event->mouse_pos, player->size, 1, top_left, player->cur_frame, player->cur_layer, player->angle);
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