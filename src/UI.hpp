#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <numeric>
#include "../inc/SDL.h"
#include "util.hpp"

#ifndef UI_HPP
#define UI_HPP

struct TextElement
{
	bool can_interact;
	std::string text;
	int font_size;
	int align;
	Vec2D pos;
	Vec2D size;
	Vec2D real_pos;
	SDL_Color color;
	TextElement(std::string text, int font_size, int align, Vec2D pos = Vec2D(), bool can_interact = false, Vec2D size = Vec2D(), SDL_Color color = Color::white(255))
		: text(text), font_size(font_size), align(align), can_interact(can_interact), pos(pos), size(size), color(color) {}
};

class UI
{
private:
	int i, j;
	Vec2D cur_txt_pos;
	std::string cur_txt;
	SDL_FRect cur_txt_box;
	std::vector<std::string> dynamic_txt;
	Uint64 last_render_time, render_time;

	Vec2D goal_bg_pos, bg_pos, bg_size;	   // background 1
	Vec2D goal_bg2_pos, bg2_pos, bg2_size; // background 2
	Vec2D goal_fg_pos, fg_pos, fg_size;	   // foreground

	// game ready
	Vec2D ava_pos, ava_size;
	bool pass_box;
	std::string cur_pass_txt;
	SDL_Color opb_color, ipb_color;
	SDL_FRect outer_pass_box, inner_pass_box;

	// game start
	std::vector<std::vector<int>> tilemap;

	// game pause
	int order;
	SDL_FRect pause_bg;
	Vec2D layout_pos, layout_size;
	Vec2D margin, num_of_cells, cell_size;
	bool music_box, txt_box;
	SDL_Color ompb_color, impb_color;
	SDL_Color otpb_color, itpb_color;
	SDL_FRect outer_music_path_box, inner_music_path_box;
	SDL_FRect outer_txt_path_box, inner_txt_path_box;

	// game over
	int count_down_time;
	bool play_once;

	int trans_alpha;
	SDL_Texture *trans_screen;
	SDL_Texture *saved_screen;
	Uint64 last_trans_time, trans_time;

public:
	bool shutdown;
	std::string cur_txt_path_txt;
	std::string cur_music_path_txt;
	std::map<std::string, TextElement *> options;
	std::map<std::string, TextElement *> over_infos;

	UI();  // constructor
	~UI(); // destructor

	void loadElements();
	void deleteElements();

	Vec2D getPassBoxPos() const;
	Vec2D getPassBoxSize() const;
	SDL_Color getPassBoxBorderColor() const;
	SDL_Color getPassBoxColor() const;
	void setPassBoxBorderColor(const SDL_Color &color = Color::ice_blue(255));
	void setPassBoxColor(const SDL_Color &color = Color::ice_blue(255));
	void turnOnPassBox();
	void turnOffPassBox();
	bool getPassBoxState() const;

	Vec2D getMusicPathBoxPos() const;
	Vec2D getMusicPathBoxSize() const;
	SDL_Color getMusicPathBoxBorderColor() const;
	SDL_Color getMusicPathBoxColor() const;
	void setMusicPathBoxBorderColor(const SDL_Color &color = Color::ice_blue(255));
	void setMusicPathBoxColor(const SDL_Color &color = Color::ice_blue(255));
	void turnOnMusicPathBox();
	void turnOffMusicPathBox();
	bool getMusicPathBoxState() const;

	Vec2D getTextPathBoxPos() const;
	Vec2D getTextPathBoxSize() const;
	SDL_Color getTextPathBoxBorderColor() const;
	SDL_Color getTextPathBoxColor() const;
	void setTextPathBoxBorderColor(const SDL_Color &color = Color::ice_blue(255));
	void setTextPathBoxColor(const SDL_Color &color = Color::ice_blue(255));
	void turnOnTextPathBox();
	void turnOffTextPathBox();
	bool getTextPathBoxState() const;

	void updateGameReady();
	void drawGameReady();
	void updateGameStart();
	void drawGameStart();
	void updateGamePlay();
	void drawGamePlay();
	void updateGamePause();
	void drawGamePause();
	void updateGameOver();
	void drawGameOver();

	void updateBackground();
	void drawBackground();
	void updateEnemies();
	void drawEnemies();
	void updatePlayer();
	void drawPlayer();
	void saveCurScreen();
	void setShutdownTime(const int &);
	void drawHighScores(const Vec2D &);
	void setDynamicText(const std::vector<std::string> &);
	void drawDynamicText(const Vec2D &, const int &align = 4, const int &font_size = 24, SDL_Color txt_color = Color::white(255), const Uint64 &delay_per_chr = 50, const Uint64 &delay_per_str = 2000);
	void updateStates();
	void drawTransScreen();
};

extern UI *ui;
extern std::map<std::string, int> settings;
extern std::multimap<int, std::string, std::greater<int>> high_scores;

#endif // UI_HPP