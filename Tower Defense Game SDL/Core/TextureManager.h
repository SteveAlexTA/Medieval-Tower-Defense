#pragma once
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};