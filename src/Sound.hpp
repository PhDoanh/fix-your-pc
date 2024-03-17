#include <bits/stdc++.h>
#include "../inc/SDL.h"
#include "../inc/SDL_mixer.h"

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
private:
	/* data */
public:
	Sound(/* args */);
	~Sound();

	void loadSoundEffect(const std::string &, const std::string &);
	static void playSoundEffect(const std::string &name, int chanel, int loop = 0);
	static void stopSoundEffect(int);

	void loadMusic(const std::string &, const std::string &);
	static void playMusic(const std::string &name, int loop = -1);
	static void stopMusic();

	void deleteSoundEffects();
	void deleteMusics();
};

extern std::map<std::string, Mix_Chunk *> sounds;
extern std::map<std::string, Mix_Music *> musics;

#endif // SOUND_HPP