#include <string>
#include <map>
#include "../inc/SDL.h"
#include "../inc/SDL_mixer.h"
#include "util.hpp"

#ifndef SOUND_HPP
#define SOUND_HPP

enum sound_chanels
{
	// window sound
	lclick,
	rclick,
	win_error,
	unlock_pc,
	critical_stop,
	shutdown,
	notify,
	new_level,

	// player sound
	cancel,
	typing,
	hit,
	emp,
	explosion_player,

	// enemy sound
	explosion_large,
	explosion_small,
	explosion,
	plasma,
	spawn,
	target
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