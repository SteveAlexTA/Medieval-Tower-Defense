#pragma once
#include<SDL.h>
#include<SDL_ttf.h>
#include "../Managers/TextureManager.h"
#include "../Enemies/Enemy.h"
#include "../Managers/SoundManager.h"
#include <vector>

enum class TowerType {
	NONE,
	ARCHER,
	CANNON,
	LIGHTNING,
};

enum class TowerLevel {
	LEVEL1,
	LEVEL2,
	LEVEL3
};

class Enemy;
class Projectile;
// Forward declaring the Enemy & Projectile class to avoid circular dependency
class Tower {
public:
	Tower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x = 0, int y = 0);
	virtual ~Tower();
	void Update();
	void Render();
	void Shoot();
	// Getters
	int getX() const { return towerX; }
	int getY() const { return towerY; }
	int getDamage() const { return damage; }
	float getRange() const { return range; }
	TowerLevel getLevel() const { return level; }

	bool canUpgrade() const { return level != TowerLevel::LEVEL3; }
	bool isSelected() const { return selected; }
	int getUpgradePrice() const;
	int calculateRefundAmount() const;
	// Setters
	void setX(int x) { towerX = x; }
	void setY(int y) { towerY = y; }
	void setSelected(bool select) { selected = select; }

	virtual void Upgrade();
	bool HandleEvents(SDL_Event* event);
	bool IsPointInside(int x, int y) const;

	void RenderRangeCircle() const;
	void setSrcRect(SDL_Rect rect) { srcRect = rect; }
	void setDestRect(SDL_Rect rect) { destRect = rect; }
	SDL_Rect getSrcRect() const { return srcRect; }
	SDL_Rect getDestRect() const { return destRect; }
protected:
	int towerX, towerY; // Position
	int baseDamage;
	float baseRange;
	int baseFireRate;

	int damage;
	float range;
	int fireRate;

	int fireCounter;
	TowerLevel level;
	TowerType type;
	bool selected;

	SDL_Renderer* renderer;
	std::vector<Enemy*>* enemies;
	std::vector<Tower*>* towers;
	SDL_Texture* texture;
	SDL_Rect destRect, srcRect;

	std::vector<Projectile*> projectiles;
	bool IsEnemyInRange(Enemy* enemy) const;
	void RenderUpgradeUI();
	Enemy* FindClosestEnemy();
	void UpdateProjectiles();
	void LoadTexture(const char* towerPath);
	void UpgradeStats();
private:
	static SDL_Texture* upgradeTexture;
	static SDL_Texture* deleteTexture;
	static TTF_Font* sharedFont;
	static bool textureLoaded;
	void InitializeSharedTextures();
	void CleanupSharedTextures();
};

class Projectile {
public:
	Projectile(SDL_Renderer* renderer, int x, int y, Enemy* target, const char* texturePath, int damage, float speed);
	virtual ~Projectile();
	void Update();
	void Render();
	bool IsOutOfBounds() const;
	bool HasHitTarget() const;
	Enemy* GetTarget() const { return target; }
protected:
	int x, y;
	float speed;
	int damage;
	Enemy* target;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect destRect, srcRect;
};