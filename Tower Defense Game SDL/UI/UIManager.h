#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "StatsBar.h"
#include "TowerBar.h"

class UI {
public:
    UI(SDL_Renderer* renderer);
    ~UI();
    bool init();
    void update(int money, int wave, int hp);
    void render(SDL_Renderer* renderer);
    void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer);
    bool isArcherTowerClicked(int mouseX, int mouseY) {
        return towerBar->isArcherTowerClicked(mouseX, mouseY);
    }
    bool isCannonTowerClicked(int mouseX, int mouseY) {
        return towerBar->isCannonTowerClicked(mouseX, mouseY);
    }
    bool isLightningTowerClicked(int mouseX, int mouseY) {
        return towerBar->isLightningTowerClicked(mouseX, mouseY);
    }
    TowerSelection getSelectedTower() const { return towerBar->getSelectedTower(); }
    void setSelectedTower(TowerSelection tower) { towerBar->setSelectedTower(tower); }
    void resetSelectedTower() { towerBar->resetSelectedTower(); }
    bool archerTowerHovered = false;
    bool cannonTowerHovered = false;
    bool lightningTowerHovered = false;
private:
    StatsBar* statsBar;
    TowerBar* towerBar;
    int currentMoney;
    int currentWave;
    int currentHealth;
};