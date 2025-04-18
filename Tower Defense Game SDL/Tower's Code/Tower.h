#pragma once
#include<SDL.h>
#include "../TextureManager.h"
#include "../Enemy's Code/Enemy.h"
#include <vector>
class Projectile; 
enum class TowerLevel {
	LEVEL1,
	LEVEL2,
	LEVEL3
};
class Tower
{
public:
	Tower(int x, int y, SDL_Renderer* renderer, int damage);
	virtual ~Tower();
	virtual void Update(std::vector<Enemy*>& enemies);
	virtual void Render();
	virtual void shoot(std::vector<Enemy*>& enemies) = 0;

	int getX() const { return x; }
	int getY() const { return y; }
	int getDamage() const { return damage; }

	bool canUpgrade() const { return m_level == TowerLevel::LEVEL1 || m_level == TowerLevel::LEVEL2; }
	void upgrade();
	bool isSelected() const { return m_isSelected; }
	void setSelected(bool selected) { m_isSelected = selected; }
	void RenderUpgradeUI();
	TowerLevel getLevel() const { return m_level; }
protected:
	int x, y; //Position
	SDL_Texture* texture;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_Renderer* renderer;
	int fireRate = 0;
	int damage;
	std::vector<Projectile*> projectiles;

	TowerLevel m_level = TowerLevel::LEVEL1;
	bool m_isSelected = false;
	static SDL_Texture* s_archerTexture;
	static SDL_Texture* s_canonTexture;
	static SDL_Texture* s_deleteTexture;
	static bool s_textureLoaded;
};

class BaseTowerOne : public Tower {
public:
	BaseTowerOne(int x, int y, SDL_Renderer* renderer, int damage);
	virtual void shoot(std::vector<Enemy*>& enemies) override;
};