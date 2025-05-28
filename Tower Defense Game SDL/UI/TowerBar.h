#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "../Systems/Money.h"

enum TowerSelection {
    NONE,
    ARCHER,
    CANNON,
    LIGHTNING,
};

class TowerBar {
public:
    TowerBar(SDL_Renderer* renderer);
    ~TowerBar();

    bool init();
    void render(SDL_Renderer* renderer);

    // Tower selection interaction
    bool isArcherTowerClicked(int mouseX, int mouseY) const;
    bool isCannonTowerClicked(int mouseX, int mouseY) const;
    bool isLightningTowerClicked(int mouseX, int mouseY) const;

    TowerSelection getSelectedTower() const { return selectedTower; }
    void setSelectedTower(TowerSelection tower) { selectedTower = tower; }
    void resetSelectedTower() { selectedTower = TowerSelection::NONE; }

    // Tower hover states
    bool archerTowerHovered = false;
    bool cannonTowerHovered = false;
    bool lightningTowerHovered = false;

private:
    // Tower icons
    SDL_Texture* archerTowerIcon;
    SDL_Texture* cannonTowerIcon;
    SDL_Texture* lightningTowerIcon;
    // Text font
    TTF_Font* font;
    TTF_Font* smallFont; // For cost labels
    // UI positions
    SDL_Rect barRect;
    SDL_Rect archerTowerRect;
    SDL_Rect cannonTowerRect;
    SDL_Rect lightningTowerRect;
    // Selection state
    TowerSelection selectedTower = TowerSelection::NONE;
    // Helper methods
    void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer, SDL_Color color, TTF_Font* textFont);
};