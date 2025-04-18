#pragma once
#include <SDL.h>
#include "../Enemy's Code/Enemy.h"
class Projectile {
public:
	Projectile(int x, int y, Enemy* target, SDL_Renderer* renderer);
	~Projectile();
	void Update();
	void Render();
	bool isOutOfBounds() const;
	bool enemyHit();
	Enemy* getTarget() const;
	SDL_Texture* getProjectileTexture() const { return texture; }
	void setProjectileTexture(SDL_Texture* newTexture) {
		if (texture) SDL_DestroyTexture(texture);
		texture = newTexture;
	}
private:
	int x, y;
	SDL_Texture* texture;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_Renderer* renderer;
	Enemy* target;
	float speed;
};