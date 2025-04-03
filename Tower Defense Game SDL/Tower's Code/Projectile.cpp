#include "Projectile.h"
#include "../TextureManager.h"
#include <cmath>
Projectile::Projectile(int x, int y, Enemy* target, SDL_Renderer* renderer): x(x), y(y), target(target), renderer(renderer), speed(10) {
    texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow_projectile.png", renderer);
    src = { 0, 0, 32, 32 }; 
    dest = { x - 8, y - 8, 16, 16 };
}

Projectile::~Projectile() {
    SDL_DestroyTexture(texture);
}

void Projectile::Update() {
    // Move the projectile towards the target
    int deltaX = target->getX() - x; //Target distance X-axis
	int deltaY = target->getY() - y; //Target distance Y-axis
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY); //Euclidean to calculate distance
	x += static_cast<int>(speed * (deltaX / distance)); //Move the projectile X-axis
	y += static_cast<int>(speed * (deltaY / distance)); //Move the projectile Y-axis
    dest.x = x - 8;
    dest.y = y - 8;
}

void Projectile::Render() {
    SDL_RenderCopy(renderer, texture, &src, &dest);
}

bool Projectile::isOutOfBounds() const {
    return x < 0 || x > 800 || y < 0 || y > 600; 
}

bool Projectile::enemyHit() {
	if (!target) return false;
	int dx = x - (target->getX() + 16);
	int dy = y - (target->getY() + 16);
	return sqrt(dx * dx + dy * dy) < 16; 
}
