#include "Tower.h"
#include "Projectile.h"
#include "../TextureManager.h"

Tower::Tower(int x, int y, SDL_Renderer* renderer, const char* texturePath, int damage) : x(x), y(y), renderer(renderer), damage(damage) { //Tower constructor
    texture = TextureManager::LoadTexture(texturePath, renderer);
    src = { 0,0,32,32 }; //Source rectangle = full texture
    dest = { x,y,32,32 }; //Destination rectangle = pos and size
}

Tower::~Tower() { //Tower destructor
    if (texture) SDL_DestroyTexture(texture);
	for (auto projectile : projectiles) {
		delete projectile;
	}
}

void Tower::Update(std::vector<Enemy*>& enemies) {
	if (!enemies.empty()) {
		fireRate++;
		if (fireRate >= 60) { 
			shoot(enemies);
			fireRate = 0;
		}
	}
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		(*it)->Update();
		if ((*it)->isOutOfBounds()) {
			delete* it;
			it = projectiles.erase(it);
		}
		else if ((*it)->enemyHit()) {
			Enemy* target = (*it)->getTarget();
			if (target && target->isAlive()) {
				target->takeDamage(damage);
			}
			delete* it;
			it = projectiles.erase(it);
		}
		else {
			++it;
		}
	}
}

void Tower::Render() {
	SDL_RenderCopy(renderer, texture, nullptr, &dest);
	for (auto& projectile : projectiles) {
		projectile->Render();
	}
}

CrossbowTower::CrossbowTower(int x, int y, SDL_Renderer* renderer): Tower(x, y, renderer, "Assets/Tower/spr_tower_crossbow.png", 25) {}

void CrossbowTower::shoot(std::vector<Enemy*>& enemies) {
	if (enemies.empty()) return;
	for (auto& enemy : enemies) {
		if (enemy && enemy->isAlive()) {
			projectiles.push_back(new Projectile(x + 16, y + 16, enemy, renderer));
			break;
		}
	}
}
