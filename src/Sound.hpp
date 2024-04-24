#include <string>
#include <map>
#include "../inc/SDL.h"
#include "../inc/SDL_mixer.h"
#include "util.hpp"

#ifndef SOUND_HPP
#define SOUND_HPP

enum sound_chanels
{
	general,
	player_channel,
	enemy_channel
};

class Sound
{
public:
	Sound() { info("Sound constructor called.\n"); }
	~Sound() { info("Sound destructor called.\n"); }

	void loadSoundEffect(const std::string &, const std::string &);
	void playSoundEffect(const std::string &name, int chanel, int ms = 5, int loop = 0);
	void stopSoundEffect(int, int ms = 5);

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