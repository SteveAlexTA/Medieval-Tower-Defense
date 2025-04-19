#include "UIManager.h"
#include "TextureManager.h"
#include <string>

UI::UI(SDL_Renderer* renderer) :
	moneyIcon(nullptr)
	, moneyText(nullptr)
	, waveIcon(nullptr)
	, waveText(nullptr)
	, healthIcon(nullptr)
	, healthText(nullptr)
	, font(nullptr)
	, buildTowerIcon(nullptr)
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
	font = TTF_OpenFont("Assets/UI/times.ttf", 24);
	if (!font) {
		std::cout << "Font load error: " << TTF_GetError() << std::endl;
		return;
	}
	moneyIcon = TextureManager::LoadTexture("Assets/UI/emerald_icon.png", renderer);
	healthIcon = TextureManager::LoadTexture("Assets/UI/hp_icon.png", renderer);
	buildTowerIcon = TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow.png", renderer);
	moneyIconRect = { 10, 8, 24, 24 };   
	moneyTextRect = { 40, 8, 70, 24 };   
	healthIconRect = { 680, 8, 24, 24 };  
	healthTextRect = { 710, 8, 70, 24 };  
	waveTextRect = { 350, 8, 100, 24 };   
	buildTowerRect = { 10, 540, 40, 40 };
}
UI::~UI() {
	if (moneyText) SDL_DestroyTexture(moneyText);
	if (waveText) SDL_DestroyTexture(waveText);
	if (healthText) SDL_DestroyTexture(healthText);
	if (buildTowerIcon) SDL_DestroyTexture(buildTowerIcon);
	if (font) TTF_CloseFont(font);
	TTF_Quit();
}
bool UI::init() {
	if (!moneyIcon || !healthIcon || !buildTowerIcon) {
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
	// Render top bar
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_Rect topBar = { 0, 0, 800, 32 };
	SDL_RenderFillRect(renderer, &topBar);
	// Render tower build UI at bottom
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_Rect buildPanel = { 0, 538, 150, 62 };
	SDL_RenderFillRect(renderer, &buildPanel);

	SDL_RenderCopy(renderer, moneyIcon, nullptr, &moneyIconRect);
	SDL_RenderCopy(renderer, healthIcon, nullptr, &healthIconRect);
	SDL_RenderCopy(renderer, buildTowerIcon, nullptr, &buildTowerRect);
	if (!moneyText) {
		updateMoneyText(renderer);
	}
	if (!waveText) {
		updateWaveText(renderer);
	}
	if (!healthText) {
		updateHealthText(renderer);
	}
	SDL_RenderCopy(renderer, moneyText, nullptr, &moneyTextRect);
	SDL_RenderCopy(renderer, waveText, nullptr, &waveTextRect);
	SDL_RenderCopy(renderer, healthText, nullptr, &healthTextRect);

	SDL_Color textColor = { 0, 0, 0, 255 };
	SDL_Surface* costSurface = TTF_RenderText_Solid(font, "100 Emeralds", textColor);
	SDL_Texture* costText = SDL_CreateTextureFromSurface(renderer, costSurface);
	SDL_Rect costRect = { buildTowerRect.x, buildTowerRect.y + buildTowerRect.h + 5, costSurface->w, costSurface->h };
	SDL_RenderCopy(renderer, costText, nullptr, &costRect);
	SDL_FreeSurface(costSurface);
	SDL_DestroyTexture(costText);
	// Highlight tower if hovered
	if (isBuildTowerHovered) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_Rect highlightRect = { buildTowerRect.x - 2, buildTowerRect.y - 2, buildTowerRect.w + 4, buildTowerRect.h + 4 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
}
void UI::renderText(const std::string& text, int x, int y, SDL_Renderer* renderer) {
	SDL_Color textColor = { 255, 0, 0, 255 }; // Red text
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
	SDL_Color textColor = { 0, 0, 0, 255 }; //Black text
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