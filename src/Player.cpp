#include "Player.hpp"
#include "Screen.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Console.hpp"
#include "Utility.hpp"

Player player;
SDL_Rect Player::player_rect;

Player::Player()
{
}

Player::~Player()
{
	// destructor implementation
}

void Player::updatePosition(SDL_MouseMotionEvent mouse)
{
	x = mouse.x;
	y = mouse.y;
}
