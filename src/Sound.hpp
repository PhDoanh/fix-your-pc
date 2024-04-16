#include <string>
#include <map>
#include "../inc/SDL.h"
#include "../inc/SDL_mixer.h"
#include "util.hpp"

#ifndef SOUND_HPP
#define SOUND_HPP

enum sound_chanels
{
	music,
	left_click,
	right_click,
	win_error,
	shoot,
	damaged,
	healing,
	die
};

class Sound
{
public:
	Sound() { info("Sound constructor called.\n"); }
	~Sound() { info("Sound destructor called.\n"); }

	void loadSoundEffect(const std::string &, const std::string &);
	void playSoundEffect(const std::string &name, int chanel, int loop = 0);
	void stopSoundEffect(int);

	void loadMusic(const std::string &, const std::string &);
	void playMusic(const std::string &name, int loop = -1);
	void stopMusic();

	void deleteSoundEffects();
	void deleteMusics();
};

extern Sound *sound;
extern std::map<std::string, Mix_Chunk *> sounds;
extern std::map<std::string, Mix_Music *> musics;

#endif // SOUND_HPP