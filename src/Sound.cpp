#include "Sound.hpp"
#include "Console.hpp"

Sound sound;
std::map<std::string, Mix_Chunk *> sounds;
std::map<std::string, Mix_Music *> musics;

Sound::Sound()
{
	// constructor implementation
}

Sound::~Sound()
{
	// destructor implementation
}

void Sound::loadSoundEffect(const std::string &name, const std::string &path)
{
	console.log("Trying to load " + path + " ... ");
	sounds[name] = Mix_LoadWAV(path.c_str());
	if (!sounds[name])
	{
		Mix_FreeChunk(sounds[name]);
		sounds[name] = nullptr;
		sounds.erase(name);
		console.error("Sound effect could not be loaded!");
	}
	console.log("Sound effect loaded!");
}

void Sound::playSoundEffect(const std::string &name, int chanel, int loop)
{
	stopSoundEffect(chanel);
	Mix_FadeInChannel(chanel, sounds[name], loop, 50);
}

void Sound::stopSoundEffect(int chanel)
{
	Mix_FadeOutChannel(chanel, 50);
}

void Sound::loadMusic(const std::string &name, const std::string &path)
{
	console.log("Trying to load " + path + " ... ");
	musics[name] = Mix_LoadMUS(path.c_str());
	if (!musics[name])
	{
		Mix_FreeMusic(musics[name]);
		musics[name] = nullptr;
		musics.erase(name);
		console.error("Music could not be loaded!");
	}
	console.log("Music loaded!");
}

void Sound::playMusic(const std::string &name, int loop)
{
	Mix_PlayMusic(musics[name], loop);
}

void Sound::stopMusic()
{
	Mix_HaltMusic();
}

void Sound::deleteSoundEffect()
{
	console.log("Deleting all sound effects ...");
	for (auto &&sound : sounds)
	{
		Mix_FreeChunk(sound.second);
		sound.second = nullptr;
		if (sound.second)
			console.error(sound.first + " - fail");
		else
			console.log(sound.first + " - done");
	}
}

void Sound::deleteMusic()
{
	console.log("Deleting all musics ...");
	for (auto &&music : musics)
	{
		Mix_FreeMusic(music.second);
		music.second = nullptr;
		if (music.second)
			console.error(music.first + " - fail");
		else
			console.log(music.first + " - done");
	}
}
