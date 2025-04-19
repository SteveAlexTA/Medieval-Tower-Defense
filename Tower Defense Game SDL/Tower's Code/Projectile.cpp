#include "Projectile.h"
#include "../TextureManager.h"
#include <cmath>
Projectile::Projectile(int x, int y, Enemy* target, SDL_Renderer* renderer): x(x), y(y), target(target), renderer(renderer), speed(10) {
    texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow_projectile.png", renderer);
    src = { 0, 0, 32, 32 }; 
    dest = { x - 8, y - 8, 16, 16 };
}

Projectile::~Projectile() {
    if (texture) SDL_DestroyTexture(texture);
}

void Projectile::Update() {
	if (!target || !target->isAlive()) {
		return; // If no target or target is dead, do nothing
	}
    float targetX = target->getX() + 16; // Target center X
    float targetY = target->getY() + 16; // Target center Y
    float deltaX = targetX - x;
    float deltaY = targetY - y;
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    if (distance > 0) {
        float moveAmount = std::min(distance, static_cast<float>(speed));
        float dirX = deltaX / distance;
        float dirY = deltaY / distance;
        x += static_cast<int>(dirX * moveAmount);
        y += static_cast<int>(dirY * moveAmount);
        dest.x = x - 8;  // Center the projectile sprite
        dest.y = y - 8;
    }
}

void Projectile::Render() {
    SDL_RenderCopy(renderer, texture, &src, &dest);
}

bool Projectile::isOutOfBounds() const {
    return x < 0 || x > 800 || y < 0 || y > 600; 
}

bool Projectile::enemyHit() {
	if (!target || !target->isAlive()) return false;
	int dx = x - (target->getX() + 16);
	int dy = y - (target->getY() + 16);
	float distance = sqrt(dx * dx + dy * dy);
    return distance < 24;
}

Enemy* Projectile::getTarget() const {
	return target;
}