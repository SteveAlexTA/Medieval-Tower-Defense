#pragma once
#include<SDL.h>
#include "../TextureManager.h"
class Tower
{
private:
	int x, y;
	SDL_Texture* texture;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_Renderer* renderer;
public:
	Tower(int x, int y, SDL_Renderer* renderer, const char* texturePath);
	~Tower();
	void Update();
	void Render();
};