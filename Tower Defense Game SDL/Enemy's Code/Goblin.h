#pragma once
#include "Enemy.h"
#include "../TextureManager.h"
#include <vector>
#include <string>
#include <cmath>

class Goblin : public Enemy
{
public:
	Goblin(int x, int y, SDL_Renderer* renderer, int map[20][25]);
	~Goblin();
	void display(SDL_Renderer* renderer) override;
private:
	static const int GOBLIN_HP = 100;
	static const int GOBLIN_SPEED = 2;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	SDL_Texture* LoadTexture(const std::string& filename);
};