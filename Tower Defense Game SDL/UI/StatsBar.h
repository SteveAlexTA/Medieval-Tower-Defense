#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class StatsBar {
public:
    StatsBar(SDL_Renderer* renderer, int screenWidth);
    ~StatsBar();

    bool init();
    void update(int money, int health, int wave);
    void render(SDL_Renderer* renderer);

    // Constants
    static const int BAR_HEIGHT = 30;
    static const SDL_Color BACKGROUND_COLOR;  
private:
    // Resources
    TTF_Font* font;
    SDL_Texture* heartIcon;
    SDL_Texture* moneyIcon;
    SDL_Texture* barBackground;

    // UI positions and dimensions
    SDL_Rect barRect;
    SDL_Rect heartIconRect;
    SDL_Rect moneyIconRect;
    SDL_Rect waveTextRect;

    // Game state values
    int currentMoney;
    int currentHealth;
    int currentWave;

    // Helper methods
    void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer, SDL_Color color);
    void createBarBackground(SDL_Renderer* renderer, int width);
};