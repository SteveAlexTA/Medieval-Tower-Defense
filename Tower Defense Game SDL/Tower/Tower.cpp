#include "Tower.h"
#include "../Core/TextureManager.h"
#include "../Core/Money.h"

SDL_Texture* Tower::upgradeTexture = nullptr;
SDL_Texture* Tower::deleteTexture = nullptr;
TTF_Font* Tower::sharedFont = nullptr;
bool Tower::textureLoaded = false;

Tower::Tower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
	: renderer(renderer), enemies(enemies), towers(towers), towerX(x), towerY(y),
	baseDamage(0), baseRange(150.0f), baseFireRate(60),
	damage(0), range(150.0f), fireRate(60),
	fireCounter(0), level(TowerLevel::LEVEL1), type(TowerType::NONE),
	selected(false), texture(nullptr)
{
	srcRect = { 0, 0, 32, 32 }; 
	destRect = { towerX, towerY, 32, 32 }; 
	InitializeSharedTextures();
	UpgradeStats();
}

Tower::~Tower() { //Tower destructor
	for (auto& projectile : projectiles) {
		delete projectile;
	}
    if (texture) SDL_DestroyTexture(texture);
}

void Tower::InitializeSharedTextures() {
	if (!textureLoaded) {
		upgradeTexture = TextureManager::LoadTexture("Assets/Icon/upgrade_icon.png", renderer);
		deleteTexture = TextureManager::LoadTexture("Assets/Icon/X_icon.png", renderer);
		textureLoaded = true;
	}
}

void Tower::CleanupSharedTextures() {
	if (textureLoaded) {
		SDL_DestroyTexture(upgradeTexture);
		SDL_DestroyTexture(deleteTexture);
		textureLoaded = false;
	}
}

void Tower::LoadTexture(const char* towerPath) {
	if (texture) {
		SDL_DestroyTexture(texture); 
	}
	texture = TextureManager::LoadTexture(towerPath, renderer);
}

void Tower::Update() {
	if (!enemies->empty()) {
		fireCounter++;
		if (fireCounter >= fireRate) {
			Shoot();
			fireCounter = 0;
		}
	}
	UpdateProjectiles();
}

void Tower::UpdateProjectiles() {
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		(*it)->Update();
		Enemy* target = (*it)->GetTarget();
		bool isTargetValid = target && target->isAlive();
		if (isTargetValid && (*it)->HasHitTarget()) {
			target->takeDamage(damage);
			delete* it;
			it = projectiles.erase(it);
		}
		else if ((*it)->IsOutOfBounds() || !isTargetValid) {
			delete* it;
			it = projectiles.erase(it);
		}
		else {
			++it;
		}
	}
}

void Tower::Render() {
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
	for (auto& projectile : projectiles) {
		projectile->Render();
	}
	if (selected) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect highlightRect = { destRect.x - 2, destRect.y - 2, destRect.w + 4, destRect.h + 4 };
		SDL_RenderDrawRect(renderer, &highlightRect);
		RenderUpgradeUI();
	}
}

int Tower::calculateRefundAmount() const {
	int totalCost = 0;
	switch (type) {
	case TowerType::ARCHER:
		totalCost = Money::ARCHER_TOWER_COST;
		if (level >= TowerLevel::LEVEL2) {
			totalCost += Money::ARCHER_UPGRADE_LVL2_COST;
		}
		if (level == TowerLevel::LEVEL3) {
			totalCost += Money::ARCHER_UPGRADE_LVL3_COST;
		}
		break;
	case TowerType::CANNON:
		totalCost = Money::CANNON_TOWER_COST;
		if (level >= TowerLevel::LEVEL2) {
			totalCost += Money::CANNON_UPGRADE_LVL2_COST;
		}
		if (level == TowerLevel::LEVEL3) {
			totalCost += Money::CANNON_UPGRADE_LVL3_COST;
		}
		break;
	case TowerType::LIGHTNING:
		totalCost = Money::LIGHTNING_TOWER_COST;
		if (level >= TowerLevel::LEVEL2) {
			totalCost += Money::LIGHTNING_UPGRADE_LVL2_COST;
		}
		if (level == TowerLevel::LEVEL3) {
			totalCost += Money::LIGHTNING_UPGRADE_LVL3_COST;
		}
		break;
	default:
		break;
	}
	return totalCost / 2;
}

void Tower::RenderUpgradeUI() {
	SDL_Color textColor = { 255, 255, 255, 255 };
	TTF_Font* font = TTF_OpenFont("Assets/Icon/consola.ttf", 14);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		return;
	}
	if (canUpgrade()) {
		SDL_Rect upgradeIconRect = { towerX - 16, towerY - 40, 32, 32 };
		SDL_RenderCopy(renderer, upgradeTexture, nullptr, &upgradeIconRect);
		int upgradeCost = getUpgradePrice();
		std::string costText = std::to_string(upgradeCost);
		SDL_Surface* costSurface = TTF_RenderText_Solid(font, costText.c_str(), textColor);
		if (costSurface) {
			SDL_Texture* costTexture = SDL_CreateTextureFromSurface(renderer, costSurface);
			SDL_Rect costRect = { towerX - 16 + (32 - costSurface->w) / 2,  towerY - 60, costSurface->w, costSurface->h };
			SDL_RenderCopy(renderer, costTexture, nullptr, &costRect);
			SDL_FreeSurface(costSurface);
			SDL_DestroyTexture(costTexture);
		}
	}
	SDL_Rect deleteIconRect = { towerX + 16, towerY - 40, 32, 32 };
	SDL_RenderCopy(renderer, deleteTexture, nullptr, &deleteIconRect);
	int refundAmount = calculateRefundAmount();
	std::string refundText = std::to_string(refundAmount);
	SDL_Surface* refundSurface = TTF_RenderText_Solid(font, refundText.c_str(), textColor);
	if (refundSurface) {
		SDL_Texture* refundTexture = SDL_CreateTextureFromSurface(renderer, refundSurface);
		SDL_Rect refundRect = { towerX + 16 + (32 - refundSurface->w) / 2, towerY - 60, refundSurface->w,refundSurface->h };
		SDL_RenderCopy(renderer, refundTexture, nullptr, &refundRect);
		SDL_FreeSurface(refundSurface);
		SDL_DestroyTexture(refundTexture);
	}
}

bool Tower::IsEnemyInRange(Enemy* enemy) const { //Check if enemy is within the tower's range using Euclidean distance
	if (!enemy || !enemy->isAlive()) return false;
	// Calculate distance from tower center to enemy center
	float towerCenterX = towerX + 16;
	float towerCenterY = towerY + 16;
	float enemyCenterX = enemy->getX() + 16;
	float enemyCenterY = enemy->getY() + 16;
	float dx = towerCenterX - enemyCenterX;
	float dy = towerCenterY - enemyCenterY;
	float distanceSquared = dx * dx + dy * dy; //Pythagorean theorem
	return distanceSquared <= (range * range);
}

Enemy* Tower::FindClosestEnemy() {
	if (enemies->empty()) return nullptr;
	// Find 1st enemy in range
	for (auto& enemy : *enemies) {
		if (enemy && enemy->isAlive() && IsEnemyInRange(enemy)) {
			return enemy;
		}
	}
	return nullptr;
}

void Tower::Shoot() {
	Enemy* target = FindClosestEnemy();
	if (target) {
		const char* projectileTexturePath = "";
		float projectileSpeed = 10.0f;
		switch (type) {
		case TowerType::ARCHER:
			projectileTexturePath = "Assets/Tower/spr_tower_archer_projectile.png";
			Sound::Instance().PlaySound("arrow");
			break;
		case TowerType::CANNON:
			projectileTexturePath = "Assets/Tower/spr_tower_cannon_projectile.png";
			Sound::Instance().PlaySound("cannon");
			projectileSpeed = 8.0f;
			break;
		case TowerType::LIGHTNING:
			projectileTexturePath = "Assets/Tower/spr_tower_lightning_projectile.png";
			Sound::Instance().PlaySound("lightning");
			projectileSpeed = 15.0f;
			break;
		default:
			return;
		}
		Projectile* projectile = new Projectile(renderer, towerX + 16, towerY + 16, target, projectileTexturePath, damage, projectileSpeed);
		projectiles.push_back(projectile);
	}
}

void Tower::Upgrade()
{
	if (!canUpgrade()) return;
	if (level == TowerLevel::LEVEL1) {
		level = TowerLevel::LEVEL2;
	}
	else if (level == TowerLevel::LEVEL2) {
		level = TowerLevel::LEVEL3;
	}
	UpgradeStats();
}

void Tower::UpgradeStats() {
	float damageMultiplier = 1.0f;
	float fireRateMultiplier = 1.0f;
	float rangeMultiplier = 1.0f;
	switch (level) {
	case TowerLevel::LEVEL1:
		damageMultiplier = 1.0f;
		fireRateMultiplier = 1.0f;
		rangeMultiplier = 1.0f;
		break;
	case TowerLevel::LEVEL2:
		damageMultiplier = 1.2f; // +20% damage 
		fireRateMultiplier = 0.8f;  // +20% fireRate
		rangeMultiplier = 1.2f; // +20% range
		break;
	case TowerLevel::LEVEL3:
		damageMultiplier = 1.5f;  // +50% damage
		fireRateMultiplier = 0.5f; // +50% fireRate
		rangeMultiplier = 1.5f;  // +50% range
		break;
	}
	damage = static_cast<int>(baseDamage * damageMultiplier);
	fireRate = static_cast<int>(baseFireRate * fireRateMultiplier);
	range = baseRange * rangeMultiplier;
}

int Tower::getUpgradePrice() const {
	if (!canUpgrade()) return 0;
	switch (type) {
	case TowerType::ARCHER:
		if (level == TowerLevel::LEVEL1)
			return Money::ARCHER_UPGRADE_LVL2_COST;
		else if (level == TowerLevel::LEVEL2)
			return Money::ARCHER_UPGRADE_LVL3_COST;
		break;
	case TowerType::CANNON:
		if (level == TowerLevel::LEVEL1)
			return Money::CANNON_UPGRADE_LVL2_COST;
		else if (level == TowerLevel::LEVEL2)
			return Money::CANNON_UPGRADE_LVL3_COST;
		break;
	case TowerType::LIGHTNING:
		if (level == TowerLevel::LEVEL1)
			return Money::LIGHTNING_UPGRADE_LVL2_COST;
		else if (level == TowerLevel::LEVEL2)
			return Money::LIGHTNING_UPGRADE_LVL3_COST;
		break;
	default:
		break;
	}
	return 0;
}

bool Tower::HandleEvents(SDL_Event* e) {
	if (e->type == SDL_MOUSEBUTTONDOWN) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		// Check if tower is clicked
		if (IsPointInside(mouseX, mouseY)) {
			selected = true;
			return true;
		}
		// Check if upgrade button is clicked
		else if (selected && canUpgrade() && mouseX >= towerX - 16 && mouseX <= towerX + 16 && mouseY >= towerY - 40 && mouseY <= towerY - 8) {
			Upgrade();
			return true;
		}
		// Check if delete button is clicked
		else if (selected && mouseX >= towerX + 16 && mouseX <= towerX + 48 && mouseY >= towerY - 40 && mouseY <= towerY - 8) {
			return true;
		}
		else {
			selected = false;
		}
	}
	return false;
}

bool Tower::IsPointInside(int x, int y) const
{
	return (x >= towerX && x <= towerX + destRect.w && y >= towerY && y <= towerY + destRect.h);
}

Projectile::Projectile(SDL_Renderer* renderer, int x, int y, Enemy* target,
	const char* texturePath, int damage, float speed)
	: renderer(renderer), x(x), y(y), target(target), damage(damage), speed(speed)
{
	texture = TextureManager::LoadTexture(texturePath, renderer);
	srcRect = { 0, 0, 32, 32 };
	destRect = { x - 8, y - 8, 16, 16 }; // Center the projectile
}

Projectile::~Projectile() { if (texture) SDL_DestroyTexture(texture); }

void Projectile::Update()
{
	if (!target || !target->isAlive()) {
		return;
	}
	// Calculate direction to target
	float targetX = target->getX() + 16; // Target center X
	float targetY = target->getY() + 16; // Target center Y
	float deltaX = targetX - x;
	float deltaY = targetY - y;
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (distance > 0) {
		// Move projectile towards target
		float moveAmount = std::min(distance, speed);
		float dirX = deltaX / distance;
		float dirY = deltaY / distance;
		x += static_cast<int>(dirX * moveAmount);
		y += static_cast<int>(dirY * moveAmount);
		// Update dest rect
		destRect.x = x - 8;
		destRect.y = y - 8;
	}
}

void Projectile::Render()
{
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

bool Projectile::IsOutOfBounds() const
{
	return x < 0 || x > 800 || y < 0 || y > 600;
}

bool Projectile::HasHitTarget() const
{
	if (!target || !target->isAlive()) return false;
	// Calculate distance to target center
	int targetCenterX = target->getX() + 16;
	int targetCenterY = target->getY() + 16;
	int dx = x - targetCenterX;
	int dy = y - targetCenterY;
	float distance = sqrt(dx * dx + dy * dy);
	return distance < 24;
}