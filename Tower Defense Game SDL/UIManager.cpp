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
	moneyIcon = TextureManager::LoadTexture("Assets/UI/emerald_icon_clean_32x32.png", renderer);
	healthIcon = TextureManager::LoadTexture("Assets/UI/hp_icon.png", renderer);
	moneyIconRect = { 10, 10, 32, 32 };
	moneyTextRect = { 50, 10, 100, 32 };
	healthIconRect = { 390, 10, 32, 32 };
	healthTextRect = { 430, 10, 100, 32 };
}
UI::~UI() {
	if (moneyText) SDL_DestroyTexture(moneyText);
	if (waveText) SDL_DestroyTexture(waveText);
	if (healthText) SDL_DestroyTexture(healthText);
	if (font) TTF_CloseFont(font);
	TTF_Quit();
}
bool UI::init() {
	if (!moneyIcon || !healthIcon) {
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
	SDL_RenderCopy(renderer, moneyIcon, nullptr, &moneyIconRect);
	SDL_RenderCopy(renderer, healthIcon, nullptr, &healthIconRect);
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
	waveText = createTextTexture(std::to_string(currentWave), renderer);
}
void UI::updateHealthText(SDL_Renderer* renderer) {
	if (healthText) SDL_DestroyTexture(healthText);
	healthText = createTextTexture(std::to_string(currentHealth), renderer);
}