#include "UIManager.h"
#include "../Core/TextureManager.h"
#include <string>

UI::UI(SDL_Renderer* renderer) :
	 moneyText(nullptr)
	, waveText(nullptr)
	, healthText(nullptr)
	, font(nullptr)
	, archerTowerIcon(nullptr)
	, cannonTowerIcon(nullptr)
	, currentMoney(0)
	, currentWave(0)
	, currentHealth(0)
{
	if (!TTF_WasInit()) {
		if (TTF_Init() == -1) {
			std::cout << "Text initialize error: " << TTF_GetError() << std::endl;
			return;
		}
	}
	font = TTF_OpenFont("Assets/UI/consola.ttf", 24);
	if (!font) {
		std::cout << "Font load error: " << TTF_GetError() << std::endl;
		return;
	}
	archerTowerIcon = TextureManager::LoadTexture("Assets/Tower/spr_tower_archer.png", renderer);
	cannonTowerIcon = TextureManager::LoadTexture("Assets/Tower/spr_tower_cannon.png", renderer);
 
	moneyTextRect = { 10, 480, 150, 20 };
	healthTextRect = { 10, 500, 150, 20 };
	waveTextRect = { 10, 520, 150, 20 };
	 
	archerTowerRect = { 740, 520, 32, 32 };
	cannonTowerRect = { 740, 570, 32, 32 };
}
UI::~UI() {
	if (moneyText) SDL_DestroyTexture(moneyText);
	if (waveText) SDL_DestroyTexture(waveText);
	if (healthText) SDL_DestroyTexture(healthText);
	if (archerTowerIcon) SDL_DestroyTexture(archerTowerIcon);
	if (cannonTowerIcon) SDL_DestroyTexture(cannonTowerIcon);
	if (font) TTF_CloseFont(font);
	TTF_Quit();
}
bool UI::init() {
	if ( !archerTowerIcon || !cannonTowerIcon) {
		std::cout << "Error loading UI textures!" << std::endl;
		return false;
	}
	return true;
}
void UI::update(int money, int wave, int health) {
	if (money != currentMoney) {
		currentMoney = money;
		if (moneyText) SDL_DestroyTexture(moneyText);
		moneyText = nullptr;
	}
	if (wave != currentWave) {
		currentWave = wave;
		if (waveText) SDL_DestroyTexture(waveText);
		waveText = nullptr;
	}
	if (health != currentHealth) {
		currentHealth = health;
		if (healthText) SDL_DestroyTexture(healthText);
		healthText = nullptr;
	}
}
void UI::render(SDL_Renderer* renderer) {
	if (!moneyText) updateMoneyText(renderer);
	if (!waveText) updateWaveText(renderer);
	if (!healthText) updateHealthText(renderer);

	// Render text 
	renderText("Emerald: " + std::to_string(currentMoney), 10, 478, renderer);
	renderText("Lives: " + std::to_string(currentHealth), 10, 508, renderer);
	renderText("Wave: " + std::to_string(currentWave), 10, 538, renderer);

	renderTowerSelectionPanel(renderer);
}

void UI::renderTowerSelectionPanel(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 135, 206, 235, 200);
	SDL_Rect towerPanelRect = { 720, 500, 70, 140 };
	SDL_RenderFillRect(renderer, &towerPanelRect);

	// Draw archer tower icon
	SDL_RenderCopy(renderer, archerTowerIcon, nullptr, &archerTowerRect);

	// Draw cannon tower icon
	SDL_RenderCopy(renderer, cannonTowerIcon, nullptr, &cannonTowerRect);

	// Draw cost labels
	SDL_Color textColor = { 255, 255, 255, 255 };

	// Archer tower cost text (next to the icon)
	std::string archerCost = std::to_string(Money::ARCHER_TOWER_COST);
	SDL_Surface* archerCostSurface = TTF_RenderText_Solid(font, archerCost.c_str(), textColor);
	SDL_Texture* archerCostText = SDL_CreateTextureFromSurface(renderer, archerCostSurface);
	SDL_Rect archerCostRect = { archerTowerRect.x - 50, archerTowerRect.y + 4,
							   archerCostSurface->w, archerCostSurface->h };
	SDL_RenderCopy(renderer, archerCostText, nullptr, &archerCostRect);
	SDL_FreeSurface(archerCostSurface);
	SDL_DestroyTexture(archerCostText);

	// Cannon tower cost text (next to the icon)
	std::string cannonCost = std::to_string(Money::CANNON_TOWER_COST);
	SDL_Surface* cannonCostSurface = TTF_RenderText_Solid(font, cannonCost.c_str(), textColor);
	SDL_Texture* cannonCostText = SDL_CreateTextureFromSurface(renderer, cannonCostSurface);
	SDL_Rect cannonCostRect = { cannonTowerRect.x - 50, cannonTowerRect.y + 4,
							  cannonCostSurface->w, cannonCostSurface->h };
	SDL_RenderCopy(renderer, cannonCostText, nullptr, &cannonCostRect);
	SDL_FreeSurface(cannonCostSurface);
	SDL_DestroyTexture(cannonCostText);

	// Highlight selected tower type
	if (selectedTower == TowerSelection::ARCHER || archerTowerHovered) {
		SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);  // Gold color
		SDL_Rect highlightRect = { archerTowerRect.x - 2, archerTowerRect.y - 2,
								 archerTowerRect.w + 4, archerTowerRect.h + 4 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}

	if (selectedTower == TowerSelection::CANNON || cannonTowerHovered) {
		SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);  // Gold color
		SDL_Rect highlightRect = { cannonTowerRect.x - 2, cannonTowerRect.y - 2,
								 cannonTowerRect.w + 4, cannonTowerRect.h + 4 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
}

void UI::renderText(const std::string& text, int x, int y, SDL_Renderer* renderer) {
	SDL_Color textColor = { 255, 255, 255, 255 }; 
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	if (!surface) {
		std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect destRect = { x, y, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, nullptr, &destRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

SDL_Texture* UI::createTextTexture(const std::string& text, SDL_Renderer* renderer) {
	SDL_Color textColor = { 255, 255, 255, 255 }; 
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (!surface) {
		std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void UI::updateMoneyText(SDL_Renderer* renderer) {
	if (moneyText) SDL_DestroyTexture(moneyText);
	moneyText = createTextTexture(std::to_string(currentMoney), renderer);
}

void UI::updateWaveText(SDL_Renderer* renderer) {
	if (waveText) SDL_DestroyTexture(waveText);
	std::string waveStr = "Wave: " + std::to_string(currentWave);
	waveText = createTextTexture(waveStr, renderer);
}

void UI::updateHealthText(SDL_Renderer* renderer) {
	if (healthText) SDL_DestroyTexture(healthText);
	healthText = createTextTexture(std::to_string(currentHealth), renderer);
}