// #include "Player.hpp"
// #include "Screen.hpp"
// #include "Game.hpp"
// #include "Sound.hpp"
// #include "Console.hpp"

// Player::Player()
// {
// 	// constructor implementation
// }

// Player::~Player()
// {
// 	// destructor implementation
// }

// void Player::updatePosition(SDL_MouseMotionEvent mouse)
// {
// 	Screen::player_rect.x = mouse.x;
// 	Screen::player_rect.y = mouse.y;
// }

// void Player::playSoundEffect(sound_chanels st)
// {
// 	switch (st)
// 	{
// 	case left_click:
// 		playSoundLeftClick();
// 		break;
// 	case right_click:
// 		playSoundRightClick();
// 		break;
// 	default:
// 		break;
// 	}
// }

// void Player::playSoundLeftClick()
// {
// 	Mix_Chunk *sound = Mix_LoadWAV("res/sounds/lclick.wav");
// 	if (!sound)
// 		console.error("Could not load sound left click!");
// 	else
// 		console.log("Sound left click loaded!");
// 	if (Mix_PlayChannel(-1, sound, 0) != 0)
// 		console.error("Could not play sound left click!");
// 	else
// 		console.log("Sound left click played!");
// }

// void Player::playSoundRightClick()
// {
// 	Mix_Chunk *sound = Mix_LoadWAV("res/sounds/rclick.wav");
// 	if (!sound)
// 		console.error("Could not load sound right click!");
// 	else
// 		console.log("Sound right click loaded!");
// 	if (Mix_PlayChannel(-1, sound, 0) != 0)
// 		console.error("Cound not play sound right click!");
// 	else
// 		console.log("Sound right click played!");
// }
