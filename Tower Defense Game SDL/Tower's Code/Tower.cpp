#include "Tower.h"
#include "Projectile.h"
#include "../TextureManager.h"

Tower::Tower(int x, int y, SDL_Renderer* renderer, const char* texturePath) : x(x), y(y), renderer(renderer) { //Tower constructor
    texture = TextureManager::LoadTexture(texturePath, renderer);
    src = { 0,0,32,32 }; //Source rectangle = full texture
    dest = { x,y,32,32 }; //Destination rectangle = pos and size
}

Tower::~Tower() { //Tower destructor
    SDL_DestroyTexture(texture);
	for (auto projectile : projectiles) {
		delete projectile;
	}
}

void Tower::Update(std::vector<Enemy*>& enemies) {
	fireRate++;
	if (fireRate >= 60) {
		shoot(enemies);
		fireRate = 0;
	}
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		(*it)->Update();
		if ((*it)->isOutOfBounds() || (*it)->enemyHit()) {
			delete *it;
			it = projectiles.erase(it);
		} else
		{
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

CrossbowTower::CrossbowTower(int x, int y, SDL_Renderer* renderer)
	: Tower(x, y, renderer, "Assets/Tower/spr_tower_crossbow.png") {
}

void CrossbowTower::shoot(std::vector<Enemy*>& enemies) {
	if (enemies.empty()) return;
	Enemy* target = enemies.front();  // Target first enemy
	projectiles.push_back(new Projectile(x + 16, y + 16, target, renderer)); //Shoot from middle (32/2=16)
}
