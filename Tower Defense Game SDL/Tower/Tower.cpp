#include "Tower.h"
#include "../Core/TextureManager.h"
SDL_Texture* Tower::s_upgradeTexture = nullptr;
SDL_Texture* Tower::s_deleteTexture = nullptr;
bool Tower::s_textureLoaded = false;

Tower::Tower(int x, int y, SDL_Renderer* renderer, int damage) : x(x), y(y), renderer(renderer), damage(damage), range(100.0f) { //Tower constructor
	if (!s_textureLoaded) {
		s_upgradeTexture = TextureManager::LoadTexture("Assets/Icon/upgrade_icon.png", renderer);
		s_deleteTexture = TextureManager::LoadTexture("Assets/Icon/X_icon.png", renderer);
		s_textureLoaded = true;
	}
	src = {0,0,32,32}; //Source rectangle = full texture
    dest = { x,y,32,32 }; //Destination rectangle = pos and size
}

Tower::~Tower() { //Tower destructor
    if (texture) SDL_DestroyTexture(texture);
}

void Tower::Render() {
	SDL_RenderCopy(renderer, texture, nullptr, &dest);
	if (m_isSelected) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect highlightRect = { dest.x - 2, dest.y - 2, dest.w + 4, dest.h + 4 };
		SDL_RenderDrawRect(renderer, &highlightRect);
		RenderUpgradeUI();
	}
}

void Tower::RenderUpgradeUI() {
	if (canUpgrade()) {
		SDL_Rect upgradeIconRect = { x - 16, y - 40, 32, 32 };
		SDL_RenderCopy(renderer, s_upgradeTexture, nullptr, &upgradeIconRect);
	}
	SDL_Rect deleteIconRect = { x + 16, y - 40, 32, 32 };
	SDL_RenderCopy(renderer, s_deleteTexture, nullptr, &deleteIconRect);
}

bool Tower::isInRange(Enemy* enemy) const {
	if (!enemy || !enemy->isAlive()) return false;
	// Calculate distance from tower center -> enemy center
	float towerCenterX = x + 16; // Tower center 
	float towerCenterY = y + 16;
	float enemyCenterX = enemy->getX() + 16; // Enemy center 
	float enemyCenterY = enemy->getY() + 16;
	float dx = towerCenterX - enemyCenterX;
	float dy = towerCenterY - enemyCenterY;
	float distanceSquared = dx * dx + dy * dy;
	return distanceSquared <= (range * range);
}