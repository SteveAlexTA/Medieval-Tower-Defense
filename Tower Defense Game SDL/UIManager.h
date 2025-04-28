#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Money.h"

enum TowerSelection {
    NONE,
    ARCHER,
    CANNON
};

class UI {
public:
	UI(SDL_Renderer* renderer);
	~UI();
	bool init();
	void update(int money, int wave, int hp);
	void render(SDL_Renderer* renderer);
	void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer);
    bool isArcherTowerClicked(int mouseX, int mouseY) {
        return mouseX >= archerTowerRect.x && mouseX < archerTowerRect.x + archerTowerRect.w &&
            mouseY >= archerTowerRect.y && mouseY < archerTowerRect.y + archerTowerRect.h;
    }
    bool isCannonTowerClicked(int mouseX, int mouseY) {
        return mouseX >= cannonTowerRect.x && mouseX < cannonTowerRect.x + cannonTowerRect.w &&
            mouseY >= cannonTowerRect.y && mouseY < cannonTowerRect.y + cannonTowerRect.h;
    }
    TowerSelection getSelectedTower() const { return selectedTower; }
    void setSelectedTower(TowerSelection tower) { selectedTower = tower; }

    bool archerTowerHovered = false;
    bool cannonTowerHovered = false;
private:
    SDL_Texture* moneyIcon;
    SDL_Texture* moneyText;
    SDL_Texture* waveIcon;
    SDL_Texture* waveText;
    SDL_Texture* healthIcon;
    SDL_Texture* healthText;
    SDL_Texture* archerTowerIcon;
    SDL_Texture* cannonTowerIcon;
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
    SDL_Rect buildPanelRect = { 0, 558, 200, 42 };
    SDL_Rect archerTowerRect = { 10, 560, 32, 32 };
    SDL_Rect cannonTowerRect = { 70, 560, 32, 32 };
    // Current values
    int currentMoney;
    int currentWave;
    int currentHealth;
    TowerSelection selectedTower = TowerSelection::NONE;
    // Helper methods
    SDL_Texture* createTextTexture(const std::string& text, SDL_Renderer* renderer);
    void updateMoneyText(SDL_Renderer* renderer);
    void updateWaveText(SDL_Renderer* renderer);
    void updateHealthText(SDL_Renderer* renderer);
    void renderTowerSelectionPanel(SDL_Renderer* renderer);
};