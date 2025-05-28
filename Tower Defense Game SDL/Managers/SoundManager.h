#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

class Sound {
public:
	static Sound& Instance();
	bool Init(int freq = 44100, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 2048);
	void Clean();
	bool LoadSound(const std::string& id, const char* filename);
	bool LoadMusic(const std::string& id, const char* filename);
	void PlaySound(const std::string& id, int loops = 0, int channel = -1);
	void PlayMusic(const std::string& id, int loops = -1);

	void StopMusic();
	void PauseMusic();
	void ResumeMusic();

	void SetMusicVolume(int volume);
	void SetSoundVolume(const std::string& id, int volume);
	void SetAllSoundVolume(int volume);

	bool IsMusicPlaying() const;
	bool IsMusicPaused() const;
private:
	Sound();
	~Sound();
	std::map<std::string, Mix_Chunk*> sfxMap;
	std::map<std::string, Mix_Music*> musicMap;
	static Sound* s_pInstance;
	bool m_bInitialized;
};
typedef Sound SoundManager;