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
protected:
public:
	Sound();														// constructor
	~Sound();														// destructor
	void loadSoundEffect(const std::string &, const std::string &); // a member function
	void playSoundEffect(const std::string &name, int chanel, int loop = 0);
	void stopSoundEffect(int);
	void loadMusic(const std::string &, const std::string &);
	void playMusic(const std::string &name, int loop = -1);
	void stopMusic();
	void deleteSoundEffect();
	void deleteMusic();
};

extern Sound sound;
extern std::map<std::string, Mix_Chunk *> sounds;
extern std::map<std::string, Mix_Music *> musics;

#endif // SOUND_HPP