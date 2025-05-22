#include "TowerBar.h"
#include "../Core/TextureManager.h"
#include <iostream>

TowerBar::TowerBar(SDL_Renderer* renderer) :
    archerTowerIcon(nullptr),
    cannonTowerIcon(nullptr),
    lightningTowerIcon(nullptr),
    font(nullptr),
    smallFont(nullptr)
{
    if (!TTF_WasInit()) {
        if (TTF_Init() == -1) {
            std::cout << "Text initialize error: " << TTF_GetError() << std::endl;
            return;
        }
    }

    // Load fonts
    font = TTF_OpenFont("Assets/Icon/consola.ttf", 20);
    smallFont = TTF_OpenFont("Assets/Icon/consola.ttf", 14); // Smaller font for cost labels
    if (!font || !smallFont) {
        std::cout << "Font load error: " << TTF_GetError() << std::endl;
        return;
    }
    // Load tower icons
    archerTowerIcon = TextureManager::LoadTexture("Assets/Tower/spr_tower_archer.png", renderer);
    cannonTowerIcon = TextureManager::LoadTexture("Assets/Tower/spr_tower_cannon.png", renderer);
    lightningTowerIcon = TextureManager::LoadTexture("Assets/Tower/spr_tower_lightning.png", renderer);
    // Set UI positions - right side of screen
    barRect = { 570, 500, 250, 100 }; 
    archerTowerRect = { 580, 500, 40, 40 };
    cannonTowerRect = { 630, 500, 40, 40 };
    lightningTowerRect = { 680, 500, 40, 40 };
}

TowerBar::~TowerBar() {
    if (archerTowerIcon) SDL_DestroyTexture(archerTowerIcon);
    if (cannonTowerIcon) SDL_DestroyTexture(cannonTowerIcon);
    if (lightningTowerIcon) SDL_DestroyTexture(lightningTowerIcon);
    if (font) TTF_CloseFont(font);
    if (smallFont) TTF_CloseFont(smallFont);
}

bool TowerBar::init() {
    if (!archerTowerIcon || !cannonTowerIcon || !lightningTowerIcon) {
        std::cout << "Error loading TowerBar textures!" << std::endl;
        return false;
    }
    return true;
}

void TowerBar::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 180); 
    SDL_RenderFillRect(renderer, &barRect);
    // Draw tower icons with cost labels below
    SDL_RenderCopy(renderer, archerTowerIcon, nullptr, &archerTowerRect);
    SDL_RenderCopy(renderer, cannonTowerIcon, nullptr, &cannonTowerRect);
    SDL_RenderCopy(renderer, lightningTowerIcon, nullptr, &lightningTowerRect);

    // Text color
    SDL_Color textColor = { 255, 255, 255, 255 };

    // Draw cost labels below tower icons
    std::string archerCost = std::to_string(Money::ARCHER_TOWER_COST);
    renderText(archerCost, archerTowerRect.x + archerTowerRect.w / 2 - 10, archerTowerRect.y + archerTowerRect.h + 5, renderer, textColor, smallFont);

    std::string cannonCost = std::to_string(Money::CANNON_TOWER_COST);
    renderText(cannonCost, cannonTowerRect.x + cannonTowerRect.w / 2 - 10, cannonTowerRect.y + cannonTowerRect.h + 5, renderer, textColor, smallFont);

    std::string lightningCost = std::to_string(Money::LIGHTNING_TOWER_COST);
    renderText(lightningCost, lightningTowerRect.x + lightningTowerRect.w / 2 - 10, lightningTowerRect.y + lightningTowerRect.h + 5, renderer, textColor, smallFont);

    // Highlight selected or hovered tower
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); 

    if (selectedTower == TowerSelection::ARCHER || archerTowerHovered) {
        SDL_Rect highlightRect = { archerTowerRect.x - 2, archerTowerRect.y - 2, archerTowerRect.w + 4, archerTowerRect.h + 4 };
        SDL_RenderDrawRect(renderer, &highlightRect);
    }

    if (selectedTower == TowerSelection::CANNON || cannonTowerHovered) {
        SDL_Rect highlightRect = { cannonTowerRect.x - 2, cannonTowerRect.y - 2, cannonTowerRect.w + 4, cannonTowerRect.h + 4 };
        SDL_RenderDrawRect(renderer, &highlightRect);
    }

    if (selectedTower == TowerSelection::LIGHTNING || lightningTowerHovered) {
        SDL_Rect highlightRect = { lightningTowerRect.x - 2, lightningTowerRect.y - 2, lightningTowerRect.w + 4, lightningTowerRect.h + 4 };
        SDL_RenderDrawRect(renderer, &highlightRect);
    }
}

void TowerBar::renderText(const std::string& text, int x, int y, SDL_Renderer* renderer, SDL_Color color, TTF_Font* textFont) {
    SDL_Surface* surface = TTF_RenderText_Blended(textFont, text.c_str(), color);
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

bool TowerBar::isArcherTowerClicked(int mouseX, int mouseY) const {
    return mouseX >= archerTowerRect.x && mouseX < archerTowerRect.x + archerTowerRect.w && mouseY >= archerTowerRect.y && mouseY < archerTowerRect.y + archerTowerRect.h;
}

bool TowerBar::isCannonTowerClicked(int mouseX, int mouseY) const {
    return mouseX >= cannonTowerRect.x && mouseX < cannonTowerRect.x + cannonTowerRect.w && mouseY >= cannonTowerRect.y && mouseY < cannonTowerRect.y + cannonTowerRect.h;
}

bool TowerBar::isLightningTowerClicked(int mouseX, int mouseY) const {
    return mouseX >= lightningTowerRect.x && mouseX < lightningTowerRect.x + lightningTowerRect.w && mouseY >= lightningTowerRect.y && mouseY < lightningTowerRect.y + lightningTowerRect.h;
}