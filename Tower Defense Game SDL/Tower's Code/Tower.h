#pragma once
#include<SDL.h>
#include "../TextureManager.h"
class Tower
{
public:
	Tower(int x, int y, SDL_Renderer* renderer, const char* texturePath);
	~Tower();
	void Update();
	void Render();
private:
	int x, y; //Position
	SDL_Texture* texture;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_Renderer* renderer;
};