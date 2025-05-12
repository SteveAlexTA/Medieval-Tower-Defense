#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "../Core/Money.h"

enum TowerSelection {
    NONE,
    ARCHER,
    CANNON,
    LIGHTNING,
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
    bool isLightningTowerClicked(int mouseX, int mouseY) {
        return mouseX >= lightningTowerRect.x && mouseX < lightningTowerRect.x + lightningTowerRect.w &&
               mouseY >= lightningTowerRect.y && mouseY < lightningTowerRect.y + lightningTowerRect.h;
    }
    TowerSelection getSelectedTower() const { return selectedTower; }
    void setSelectedTower(TowerSelection tower) { selectedTower = tower; }
    void resetSelectedTower() { selectedTower = TowerSelection::NONE; }

    bool archerTowerHovered = false;
    bool cannonTowerHovered = false;
    bool lightningTowerHovered = false;
private:
    SDL_Texture* moneyText;
    SDL_Texture* waveText;
    SDL_Texture* healthText;
    SDL_Texture* archerTowerIcon;
    SDL_Texture* cannonTowerIcon;
    SDL_Texture* lightningTowerIcon;
    // Text font
    TTF_Font* font;
    TTF_Font* getFont() const { return font; }
    // UI positions
    SDL_Rect moneyTextRect;
    SDL_Rect waveTextRect;
    SDL_Rect healthTextRect;
    SDL_Rect archerTowerRect;
    SDL_Rect cannonTowerRect;
    SDL_Rect lightningTowerRect;
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