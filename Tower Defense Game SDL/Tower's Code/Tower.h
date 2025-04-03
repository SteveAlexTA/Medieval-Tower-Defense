#pragma once
#include<SDL.h>
#include "../TextureManager.h"
#include "../Enemy's Code/Enemy.h"
#include <vector>
class Projectile; // Forward declaration
class Tower
{
public:
	Tower(int x, int y, SDL_Renderer* renderer, const char* texturePath);
	virtual ~Tower();
	virtual void Update(std::vector<Enemy*>& enemies);
	virtual void Render();
	virtual void shoot(std::vector<Enemy*>& enemies) = 0;

	int getX() const { return x; }
	int getY() const { return y; }
protected:
	int x, y; //Position
	SDL_Texture* texture;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_Renderer* renderer;
	int fireRate = 0;
	std::vector<Projectile*> projectiles;
};
class CrossbowTower : public Tower {
public:
	CrossbowTower(int x, int y, SDL_Renderer* renderer);
	void shoot(std::vector<Enemy*>& enemies) override;
};