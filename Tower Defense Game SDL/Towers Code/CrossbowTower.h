#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<vector>
class CrossbowTower
{
public:
	CrossbowTower(int x, int y, SDL_Renderer* renderer);
	~CrossbowTower();
	void Update();
	void Render();
	int GetX() const { return x; }
	int GetY() const { return y; }
private:
	int x, y; //Position
	SDL_Rect src, dest;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
};
