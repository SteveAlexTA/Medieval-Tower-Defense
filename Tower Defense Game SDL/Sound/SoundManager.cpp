#include "SoundManager.h"
#include <iostream>
std::map<std::string, Mix_Chunk*> Sound::soundMap;
std::map<std::string, Mix_Music*> Sound::musicMap;
bool Sound::s_initialized = false;

bool Sound::Init() {
	if (s_initialized) {
		return true; 
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	s_initialized = true;
	return true;
}

void Sound::Clean() {
	for (auto& sound : soundMap) {
		Mix_FreeChunk(sound.second);
	}
	soundMap.clear();
	Mix_CloseAudio();
	s_initialized = false;
}

Mix_Chunk* Sound::LoadSound(const char* filename) {
	if (!s_initialized) {
		std::cerr << "Sound system not initialized!" << std::endl;
		return nullptr;
	}
	Mix_Chunk* sound = Mix_LoadWAV(filename);
	if (!sound) {
		std::cerr << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	return sound;
}

void Sound::PlaySound(Mix_Chunk* sound, int loops) {
	if (sound) {
		Mix_PlayChannel(-1, sound, loops);
	}
}

Mix_Chunk* Sound::GetSound(const char* filename) {
	std::string loaded = filename;
	if (soundMap.find(loaded) != soundMap.end()) {
		return soundMap[loaded];
	}
	Mix_Chunk* sound = LoadSound(filename);
	if (sound) {
		soundMap[loaded] = sound;
	}
	return sound;
}

Mix_Music* Sound::LoadMusic(const char* filename) {
	if (!s_initialized) {
		std::cerr << "Sound system not initialized!" << std::endl;
		return nullptr;
	}
	Mix_Music* music = Mix_LoadMUS(filename);
	if (!music) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	return music;
}

void Sound::PlayMusic(Mix_Music* music, int loops) {
	if (music) {
		if (Mix_PlayingMusic()) {
			Mix_HaltMusic(); 
		}
		Mix_PlayMusic(music, loops);
	}
}

void Sound::StopMusic() {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
}

void Sound::PauseMusic() {
	if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
		Mix_PauseMusic();
	}
}

void Sound::ResumeMusic() {
	if (Mix_PausedMusic()) {
		Mix_ResumeMusic();
	}
}

void Sound::SetMusicVolume(int volume) {
	Mix_VolumeMusic(volume); 
}

Mix_Music* Sound::GetMusic(const char* filename) {
	std::string loaded = filename;
	if (musicMap.find(loaded) != musicMap.end()) {
		return musicMap[loaded];
	}
	Mix_Music* music = LoadMusic(filename);
	if (music) {
		musicMap[loaded] = music;
	}
	return music;
}