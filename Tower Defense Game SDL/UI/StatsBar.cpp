#include "StatsBar.h"
#include "../Managers/TextureManager.h"
#include <iostream>

// Define static member for medieval theme
const SDL_Color StatsBar::BACKGROUND_COLOR = { 101, 67, 33, 255 }; 

StatsBar::StatsBar(SDL_Renderer* renderer, int screenWidth) :
    font(nullptr),
    heartIcon(nullptr),
    moneyIcon(nullptr),
    barBackground(nullptr),
    currentMoney(0),
    currentHealth(0),
    currentWave(0)
{
    if (!TTF_WasInit()) {
        if (TTF_Init() == -1) {
            std::cout << "Text initialize error: " << TTF_GetError() << std::endl;
            return;
        }
    }
    font = TTF_OpenFont("Assets/Icon/consola.ttf", 20);
    if (!font) {
        std::cout << "Font load error: " << TTF_GetError() << std::endl;
        return;
    }
    heartIcon = TextureManager::LoadTexture("Assets/Icon/hp_icon.png", renderer);
    moneyIcon = TextureManager::LoadTexture("Assets/Icon/money_icon.png", renderer);

    createBarBackground(renderer, screenWidth);

    // Set UI positions
    barRect = { 0, 0, screenWidth, BAR_HEIGHT };
    heartIconRect = { 20, 10, 20, 20 };
    moneyIconRect = { screenWidth / 2 - 60, 10, 20, 20 };
    waveTextRect = { screenWidth - 150, 10, 120, 20 };
}

StatsBar::~StatsBar() {
    if (font) TTF_CloseFont(font);
    if (heartIcon) SDL_DestroyTexture(heartIcon);
    if (moneyIcon) SDL_DestroyTexture(moneyIcon);
    if (barBackground) SDL_DestroyTexture(barBackground);
}

bool StatsBar::init() {
    if (!font || !heartIcon || !moneyIcon || !barBackground) {
        std::cout << "Error loading StatsBar resources!" << std::endl;
        return false;
    }
    return true;
}

void StatsBar::update(int money, int health, int wave) {
    currentMoney = money;
    currentHealth = health;
    currentWave = wave;
}

void StatsBar::render(SDL_Renderer* renderer) {
    // Draw background
    SDL_RenderCopy(renderer, barBackground, nullptr, &barRect);

    // Draw icons
    SDL_RenderCopy(renderer, heartIcon, nullptr, &heartIconRect);
    SDL_RenderCopy(renderer, moneyIcon, nullptr, &moneyIconRect);

    // Draw text with gold/white medieval color
    SDL_Color textColor = { 255, 215, 0, 255 }; // Gold color for medieval theme

    // Render health text
    std::string healthText = std::to_string(currentHealth);
    renderText(healthText, heartIconRect.x + heartIconRect.w + 5, heartIconRect.y, renderer, textColor);

    // Render money text
    std::string moneyText = std::to_string(currentMoney);
    renderText(moneyText, moneyIconRect.x + moneyIconRect.w + 5, moneyIconRect.y, renderer, textColor);

    // Render wave text
    std::string waveText = "Wave: " + std::to_string(currentWave);
    renderText(waveText, waveTextRect.x, waveTextRect.y, renderer, textColor);
}

void StatsBar::renderText(const std::string& text, int x, int y, SDL_Renderer* renderer, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
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

void StatsBar::createBarBackground(SDL_Renderer* renderer, int width) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, BAR_HEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b));
    barBackground = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}