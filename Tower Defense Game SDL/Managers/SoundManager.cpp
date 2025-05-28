#include "SoundManager.h"
#include <iostream>

Sound::Sound() : m_bInitialized(false) {}

Sound::~Sound() { Clean();}

Sound& Sound::Instance() {
	static Sound instance;
	return instance;
}

bool Sound::Init(int freq, Uint16 format, int channels, int chunksize) {
	if (Mix_OpenAudio(freq, format, channels, chunksize) < 0) {
		std::cout << "SDL_mixer not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	Mix_AllocateChannels(16);
	m_bInitialized = true;
	std::cout << "Sound system initialized successfully!" << std::endl;
	return true;
}

void Sound::Clean() {
	for (auto& sound : sfxMap) {
		if (sound.second != nullptr) {
			Mix_FreeChunk(sound.second);
			sound.second = nullptr;
		}
	}
	sfxMap.clear();
	for (auto& music : musicMap) {
		if (music.second != nullptr) {
			Mix_FreeMusic(music.second);
			music.second = nullptr;
		}
	}
	musicMap.clear();
	if (m_bInitialized) {
		Mix_CloseAudio();
		m_bInitialized = false;
		std::cout << "Sound system cleaned up!" << std::endl;
	}	
}

bool Sound::LoadSound(const std::string& id, const char* filename) {
	if (sfxMap.find(id) != sfxMap.end()) {
		std::cout << "Sound with ID " << id << " already loaded!" << std::endl;
		return false;
	}
	Mix_Chunk* sound = Mix_LoadWAV(filename);
	if (sound == nullptr) {
		std::cout << "Failed to load sound: " << filename << " Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sfxMap[id] = sound;
	return true;
}

bool Sound::LoadMusic(const std::string& id, const char* filename) {
	if (musicMap.find(id) != musicMap.end()) {
		std::cout << "Music with ID " << id << " already loaded!" << std::endl;
		return false;
	}
	Mix_Music* music = Mix_LoadMUS(filename);
	if (music == nullptr) {
		std::cout << "Failed to load music: " << filename << " Error: " << Mix_GetError() << std::endl;
		return false;
	}
	musicMap[id] = music;
	return true;
}

void Sound::PlaySound(const std::string& id, int loops, int channel) {
	auto it = sfxMap.find(id);
	if (it != sfxMap.end()) {
		Mix_PlayChannel(channel, it->second, loops);
	}
	else {
		std::cout << "Sound with ID " << id << " not found!" << std::endl;
	}
}

void Sound::PlayMusic(const std::string& id, int loops) {
	auto it = musicMap.find(id);
	if (it != musicMap.end()) {
		if (Mix_PlayingMusic()) {
			Mix_HaltMusic();
		}
		Mix_PlayMusic(it->second, loops);
	}
	else {
		std::cout << "Music '" << id << "' not found!" << std::endl;
	}
}

void Sound::StopMusic() {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
}

void Sound::PauseMusic() {
	if (Mix_PlayingMusic()) {
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

void Sound::SetSoundVolume(const std::string& id, int volume) {
	auto it = sfxMap.find(id);
	if (it != sfxMap.end()) {
		Mix_VolumeChunk(it->second, volume);
	}
	else {
		std::cout << "Sound with ID " << id << " not found!" << std::endl;
	}
}

void Sound::SetAllSoundVolume(int volume) {
	Mix_Volume(-1, volume);	
}

bool Sound::IsMusicPlaying() const {
	return Mix_PlayingMusic() != 0;
}

bool Sound::IsMusicPaused() const {
	return Mix_PausedMusic() != 0;
}