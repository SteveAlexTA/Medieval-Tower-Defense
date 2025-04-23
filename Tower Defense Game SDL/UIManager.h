#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Money.h"
class UI {
public:
	UI(SDL_Renderer* renderer);
	~UI();
	bool init();
	void update(int money, int wave, int hp);
	void render(SDL_Renderer* renderer);
	void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer);
    bool isBuildTowerClicked(int mouseX, int mouseY) {
        return mouseX >= buildTowerRect.x && mouseX < buildTowerRect.x + buildTowerRect.w && mouseY >= buildTowerRect.y && mouseY < buildTowerRect.y + buildTowerRect.h;
    }
    bool isBuildTowerHovered = false;
private:
    SDL_Texture* moneyIcon;
    SDL_Texture* moneyText;
    SDL_Texture* waveIcon;
    SDL_Texture* waveText;
    SDL_Texture* healthIcon;
    SDL_Texture* healthText;
    SDL_Texture* buildTowerIcon;
    // Text font
    TTF_Font* font;
	TTF_Font* getFont() const { return font; }
    // UI positions
    SDL_Rect moneyIconRect = { 0, 0, 0, 0 };
    SDL_Rect moneyTextRect = { 0, 0, 0, 0 };
    SDL_Rect waveIconRect = { 0, 0, 0, 0 };
    SDL_Rect waveTextRect = { 0, 0, 0, 0 };
    SDL_Rect healthIconRect = { 0, 0, 0, 0 };
    SDL_Rect healthTextRect = { 0, 0, 0, 0 };
    SDL_Rect buildTowerRect = { 10, 500, 48, 48 };
    SDL_Rect buildTowerTextRect = { 10, 550, 100, 32 };
    // Current values
    int currentMoney;
    int currentWave;
    int currentHealth;
    // Helper methods
    SDL_Texture* createTextTexture(const std::string& text, SDL_Renderer* renderer);
    void updateMoneyText(SDL_Renderer* renderer);
    void updateWaveText(SDL_Renderer* renderer);
    void updateHealthText(SDL_Renderer* renderer);
};