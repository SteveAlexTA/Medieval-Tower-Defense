#include "ArcherTower.h"
#include "../TextureManager.h"
#include <cmath>

ArcherTower::ArcherTower(int x, int y, SDL_Renderer* renderer, int damage)
    : Tower(x, y, renderer, damage) {
    texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_archer.png", renderer);
    range = 200.0f;  
	shootSound = Sound::GetSound("Assets/Sound/arrow.wav");
}

ArcherTower::~ArcherTower() {
    for (auto projectile : projectiles) {
        delete projectile;
    }
}

void ArcherTower::Update(std::vector<Enemy*>& enemies) {
    if (!enemies.empty()) {
        fireRate++;

        // Low damage, fast fire rate
        int fireThreshold;
        if (m_level == TowerLevel::LEVEL1) {
            fireThreshold = 20;  
        }
        else if (m_level == TowerLevel::LEVEL2) {
            fireThreshold = 15;  
        }
        else {
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
            // Damage increases w/lvl
            int damageToApply;
            if (m_level == TowerLevel::LEVEL1) {
                damageToApply = damage;
            }
            else if (m_level == TowerLevel::LEVEL2) {
                damageToApply = damage;
            }
            else {
                damageToApply = damage * 1.5;
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

void ArcherTower::Render() {
    SDL_RenderCopy(renderer, texture, nullptr, &dest);

    for (auto& projectile : projectiles) {
        projectile->Render();
    }

    if (m_isSelected) {
        // Highlight the selected tower
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect highlightRect = { dest.x - 2, dest.y - 2, dest.w + 4, dest.h + 4 };
        SDL_RenderDrawRect(renderer, &highlightRect);
        RenderUpgradeUI();
    }
}

void ArcherTower::upgrade() {
    if (m_level == TowerLevel::LEVEL1) {
        m_level = TowerLevel::LEVEL2;
    }
    else if (m_level == TowerLevel::LEVEL2) {
        m_level = TowerLevel::LEVEL3;
    }
}

void ArcherTower::shoot(std::vector<Enemy*>& enemies) {
    if (enemies.empty()) return;

    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive() && isInRange(enemy)) {
            ArcherProjectile* projectile = new ArcherProjectile(x + 16, y + 16, enemy, renderer);
            projectiles.push_back(projectile);
			Sound::PlaySound(shootSound);
            break;
        }
    }
}

ArcherProjectile::ArcherProjectile(int x, int y, Enemy* target, SDL_Renderer* renderer)
    : x(x), y(y), target(target), renderer(renderer), speed(10) {  // Faster projectile
    texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_archer_projectile.png", renderer);
    src = { 0, 0, 32, 32 };
    dest = { x - 8, y - 8, 16, 16 };
}

ArcherProjectile::~ArcherProjectile() {
    \
        if (texture) SDL_DestroyTexture(texture);
}

void ArcherProjectile::Update() {
    if (!target || !target->isAlive()) {
        return;
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

void ArcherProjectile::Render() {
    SDL_RenderCopy(renderer, texture, &src, &dest);
}

bool ArcherProjectile::isOutOfBounds() const {
    return x < 0 || x > 800 || y < 0 || y > 600;
}

bool ArcherProjectile::enemyHit() {
    if (!target || !target->isAlive()) return false;
    int dx = x - (target->getX() + 16);
    int dy = y - (target->getY() + 16);
    float distance = sqrt(dx * dx + dy * dy);
    return distance < 24;
}