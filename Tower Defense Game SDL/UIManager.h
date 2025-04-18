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
    //void renderText(const std::string& text, int x, int y, SDL_Renderer* renderer);
private:
    SDL_Texture* moneyIcon;
    SDL_Texture* moneyText;
    SDL_Texture* waveIcon;
    SDL_Texture* waveText;
    SDL_Texture* healthIcon;
    SDL_Texture* healthText;
    // Text font
    TTF_Font* font;
    // UI positions
    SDL_Rect moneyIconRect;
    SDL_Rect moneyTextRect;
    SDL_Rect waveIconRect;
    SDL_Rect waveTextRect;
    SDL_Rect healthIconRect;
    SDL_Rect healthTextRect;
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