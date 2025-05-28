#include "UIManager.h"
#include "../Managers/TextureManager.h"
#include <SDL_ttf.h>
#include <string>

UI::UI(SDL_Renderer* renderer) :
	currentMoney(0),
	currentWave(0),
	currentHealth(0),
	archerTowerHovered(false),
	cannonTowerHovered(false),
	lightningTowerHovered(false)
{
	if (!TTF_WasInit()) {
		if (TTF_Init() == -1) {
			std::cout << "Failed to initialize text: " << TTF_GetError() << std::endl;
			return;
		}
	}
	statsBar = new StatsBar(renderer, 800);
	towerBar = new TowerBar(renderer);
}

UI::~UI() {
	if (statsBar) delete statsBar;
	if (towerBar) delete towerBar;
}

bool UI::init() {
	if (!statsBar->init() || !towerBar->init()) {
		std::cout << "Failed to initialize UI!" << std::endl;
		return false;
	}
	return true;
}

void UI::update(int money, int wave, int health) {
	currentMoney = money;
	currentWave = wave;
	currentHealth = health;

	statsBar->update(money, health, wave);

	towerBar->archerTowerHovered = archerTowerHovered;
	towerBar->cannonTowerHovered = cannonTowerHovered;
	towerBar->lightningTowerHovered = lightningTowerHovered;
}

void UI::renderText(const std::string& text, int x, int y, SDL_Renderer* renderer) {
	SDL_Color textColor = { 255, 255, 255, 255 }; // White text
	// Create a font for rendering text
	TTF_Font* font = TTF_OpenFont("Assets/Icon/consola.ttf", 18);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		return;
	}
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	if (!surface) {
		std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
		TTF_CloseFont(font);
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect destRect = { x, y, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, nullptr, &destRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

void UI::render(SDL_Renderer* renderer) {
	statsBar->render(renderer);
	towerBar->render(renderer);
}
