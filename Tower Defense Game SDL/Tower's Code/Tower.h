#pragma once
#include<SDL.h>
#include "../TextureManager.h"
#include "../Enemy's Code/Enemy.h"
#include <vector>
enum class TowerType {
	NONE,
	ARCHER,
	CANNON
};

enum class TowerLevel {
	LEVEL1,
	LEVEL2,
	LEVEL3
};

class Tower {
public:
	Tower(int x, int y, SDL_Renderer* renderer, int damage);
	virtual ~Tower();
	virtual void Update(std::vector<Enemy*>& enemies) = 0;
	virtual void Render();
	virtual void shoot(std::vector<Enemy*>& enemies) = 0;

	int getX() const { return x; }
	int getY() const { return y; }
	int getDamage() const { return damage; }

	bool canUpgrade() const { return m_level != TowerLevel::LEVEL3; }
	virtual void upgrade() = 0;
	bool isSelected() const { return m_isSelected; }
	void setSelected(bool selected) { m_isSelected = selected; }
	void RenderUpgradeUI();
	TowerLevel getLevel() const { return m_level; }

	float getRange() const { return range; }
	bool isInRange(Enemy* enemy) const;

protected:
	int x, y; //Position
	SDL_Texture* texture;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_Renderer* renderer;
	int fireRate = 0;
	float range; 
	int damage;

	TowerLevel m_level = TowerLevel::LEVEL1;
	bool m_isSelected = false;
	static SDL_Texture* s_upgradeTexture;
	static SDL_Texture* s_deleteTexture;
	static bool s_textureLoaded;
};

