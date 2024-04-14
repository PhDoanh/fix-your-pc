#include "Sound.hpp"

std::map<std::string, Mix_Chunk *> sounds;
std::map<std::string, Mix_Music *> musics;

void Sound::loadSoundEffect(const std::string &name, const std::string &path)
{
	info("Trying to load " + path + " ... ");
	sounds[name] = Mix_LoadWAV(path.c_str());
	if (!sounds[name])
	{
		Mix_FreeChunk(sounds[name]);
		sounds[name] = nullptr;
		sounds.erase(name);
		error(path + " - fail.");
	}
	info(path + " - done.");
}

void Sound::playSoundEffect(const std::string &name, int chanel, int loop)
{
	stopSoundEffect(chanel);
	Mix_FadeInChannel(chanel, sounds[name], loop, 10);
}

void Sound::stopSoundEffect(int chanel)
{
	Mix_FadeOutChannel(chanel, 10);
}

void Sound::loadMusic(const std::string &name, const std::string &path)
{
	info("Trying to load " + path + " ... ");
	musics[name] = Mix_LoadMUS(path.c_str());
	if (!musics[name])
	{
		Mix_FreeMusic(musics[name]);
		musics[name] = nullptr;
		musics.erase(name);
		error(path + " - fail.");
	}
	info(path + " - done.");
}

void Sound::playMusic(const std::string &name, int loop)
{
	Mix_PlayMusic(musics[name], loop);
}

void Sound::stopMusic()
{
	Mix_HaltMusic();
}

void Sound::deleteSoundEffects()
{
	info("Deleting all sound effects ...");
	for (auto &&sound : sounds)
	{
		Mix_FreeChunk(sound.second);
		sound.second = nullptr;
		if (sound.second)
			error(sound.first + " - fail.");
		else
			info(sound.first + " - done.");
	}
}

void Sound::deleteMusics()
{
	info("Deleting all musics ...");
	for (auto &&music : musics)
	{
		Mix_FreeMusic(music.second);
		music.second = nullptr;
		if (music.second)
			error(music.first + " - fail.");
		else
			info(music.first + " - done.");
	}
}