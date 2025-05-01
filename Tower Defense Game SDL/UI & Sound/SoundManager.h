#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

class Sound {
public:
	static bool Init();
	static void Clean();
	static Mix_Chunk* LoadSound(const char* filename);
	static void PlaySound(Mix_Chunk* sound, int loops = 0);
	static Mix_Chunk* GetSound(const char* filename);

	static Mix_Music* LoadMusic(const char* filename);
	static void PlayMusic(Mix_Music* music, int loops = -1);  
	static void StopMusic();
	static void PauseMusic();
	static void ResumeMusic();
	static void SetMusicVolume(int volume);  
	static Mix_Music* GetMusic(const char* filename);
private:
	static std::map<std::string, Mix_Chunk*> soundMap;
	static std::map<std::string, Mix_Music*> musicMap;
	static bool s_initialized;
};