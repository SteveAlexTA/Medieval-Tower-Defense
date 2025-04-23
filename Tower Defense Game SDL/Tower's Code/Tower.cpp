#include "Tower.h"
#include "Projectile.h"
#include "../TextureManager.h"
SDL_Texture* Tower::s_archerTexture = nullptr;
SDL_Texture* Tower::s_canonTexture = nullptr;
SDL_Texture* Tower::s_deleteTexture = nullptr;
bool Tower::s_textureLoaded = false;

Tower::Tower(int x, int y, SDL_Renderer* renderer, int damage) : x(x), y(y), renderer(renderer), damage(damage), range(100.0f) { //Tower constructor
    texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow.png", renderer);
	if (!s_textureLoaded) {
		s_archerTexture = TextureManager::LoadTexture("Assets/Tower/spr_tower_archer.png", renderer);
		s_canonTexture = TextureManager::LoadTexture("Assets/Tower/spr_tower_cannon.png", renderer);
		s_deleteTexture = TextureManager::LoadTexture("Assets/UI/X_icon.png", renderer);
		s_textureLoaded = true;
	}
	src = {0,0,32,32}; //Source rectangle = full texture
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
		int fireThreshold = 0;
		if (m_level == TowerLevel::LEVEL1) {
			fireThreshold = 40;
		}
		else if (m_level == TowerLevel::LEVEL2) {
			fireThreshold = 25;
		}
		else if (m_level == TowerLevel::LEVEL3) {
			fireThreshold = 15;
		}
		if (fireRate >= fireThreshold) {
			shoot(enemies);
			fireRate = 0;
		}
	}
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		(*it)->Update();
		Enemy* target = (*it)->getTarget();
		bool isTargetInvalid = !target || !target->isAlive();
		if (!isTargetInvalid && (*it)->enemyHit()) {
			int damageToApply = damage;
			if (m_level == TowerLevel::LEVEL1) {
				damageToApply = damage * 1;
			}
			else if (m_level == TowerLevel::LEVEL2) {
				damageToApply = damage * 1.5;
			}
			else if (m_level == TowerLevel::LEVEL3) {
				damageToApply = damage * 2.5;
			}
			target->takeDamage(damageToApply);
			delete* it;
			it = projectiles.erase(it);
			continue;
		}
		if ((*it)->isOutOfBounds() || isTargetInvalid) {
			delete* it;
			it = projectiles.erase(it);
		}
		else {
			++it;
		}
	}
}
void Tower::upgrade() {
	if (m_level == TowerLevel::LEVEL1) {
		m_level = TowerLevel::LEVEL2;
		texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_archer.png", renderer);
		range = 400.0f;
	}
	else if (m_level == TowerLevel::LEVEL2) {
		m_level = TowerLevel::LEVEL3;
		texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_cannon.png", renderer);
		range = 800.0f;
	}
	else if (m_level == TowerLevel::LEVEL3) {
		return;
	}
}

void Tower::Render() {
	SDL_RenderCopy(renderer, texture, nullptr, &dest);
	for (auto& projectile : projectiles) {
		projectile->Render();
	}
	if (m_isSelected) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect highlightRect = { dest.x - 2, dest.y - 2, dest.w + 4, dest.h + 4 };
		SDL_RenderDrawRect(renderer, &highlightRect);
		SDL_SetRenderDrawColor(renderer, 0, 200, 255, 100);
		for (int angle = 0; angle < 360; angle += 10) {
			float radian = angle * (M_PI / 180.0f);
			int circleX = x + 16 + static_cast<int>(range * cos(radian));
			int circleY = y + 16 + static_cast<int>(range * sin(radian));
			SDL_RenderDrawPoint(renderer, circleX, circleY);
		}
		RenderUpgradeUI();
	}
}

void Tower::shoot(std::vector<Enemy*>& enemies) {
	if (enemies.empty()) return;
	for (auto& enemy : enemies) {
		if (enemy && enemy->isAlive() && isInRange(enemy)) {
			Projectile* projectile = new Projectile(x + 16, y + 16, enemy, renderer);
			if (m_level == TowerLevel::LEVEL1) {
				projectile->setProjectileTexture(TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow_projectile.png", renderer));
			}
			else if (m_level == TowerLevel::LEVEL2) {
				projectile->setProjectileTexture(TextureManager::LoadTexture("Assets/Tower/spr_tower_archer_projectile.png", renderer));
			}
			else if (m_level == TowerLevel::LEVEL3) {
				projectile->setProjectileTexture(TextureManager::LoadTexture("Assets/Tower/spr_tower_cannon_projectile.png", renderer));
			}
			projectiles.push_back(projectile);
			break;
		}
	}
}

void Tower::RenderUpgradeUI() {
	if (canUpgrade()) {
		SDL_Rect upgradeIconRect = { x - 16, y - 40, 32, 32 };
		SDL_Texture* nextLevelTexture = nullptr;
		if (m_level == TowerLevel::LEVEL1) {
			nextLevelTexture = s_archerTexture;
		}
		else if (m_level == TowerLevel::LEVEL2) {
			nextLevelTexture = s_canonTexture;
		}
		if (nextLevelTexture) {
			SDL_RenderCopy(renderer, nextLevelTexture, nullptr, &upgradeIconRect);
		}
	}
	SDL_Rect deleteIconRect = { x + 16, y - 40, 32, 32 };
	SDL_RenderCopy(renderer, s_deleteTexture, nullptr, &deleteIconRect);
}

BaseTowerOne::BaseTowerOne(int x, int y, SDL_Renderer* renderer, int damage) : Tower(x, y, renderer, damage) {
	texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow.png", renderer);
	range = 200.0f; //Default range
}
void BaseTowerOne::shoot(std::vector<Enemy*>& enemies) {
	if (enemies.empty()) return;
	for (auto& enemy : enemies) {
		if (enemy && enemy->isAlive() && isInRange(enemy)) {
			Projectile* projectile = new Projectile(x + 16, y + 16, enemy, renderer);
			if (m_level == TowerLevel::LEVEL1) {
				projectile->setProjectileTexture(TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow_projectile.png", renderer));
			}
			else if (m_level == TowerLevel::LEVEL2) {
				projectile->setProjectileTexture(TextureManager::LoadTexture("Assets/Tower/spr_tower_archer_projectile.png", renderer));
			}
			else if (m_level == TowerLevel::LEVEL3) {
				projectile->setProjectileTexture(TextureManager::LoadTexture("Assets/Tower/spr_tower_cannon_projectile.png", renderer));
			}
			projectiles.push_back(projectile);
			break;
		}
	}
}
