#include "WinScreen.h"
#include "../Core/TextureManager.h"
#include <SDL_ttf.h>

WinScreen::WinScreen(SDL_Renderer* renderer)
    : renderer(renderer)
    , titleText(nullptr)
    , titleRect{ 0, 0, 0, 0 }
    , backgroundTexture(nullptr)
    , font(nullptr)
    , displayStartTime(0)
{
}

WinScreen::~WinScreen() {
    if (titleText) SDL_DestroyTexture(titleText);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (font) TTF_CloseFont(font);
}

bool WinScreen::init() {
    backgroundTexture = TextureManager::LoadTexture("Assets/Icon/win_background.png", renderer);
    if (!backgroundTexture) {
        return false;
    }

    font = TTF_OpenFont("Assets/Icon/consola.ttf", 72);
    if (!font) {
        return false;
    }

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, "YOU WIN", textColor);
    titleText = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int textWidth, textHeight;
    SDL_QueryTexture(titleText, NULL, NULL, &textWidth, &textHeight);

    titleRect = { (800 - textWidth) / 2, (600 - textHeight) / 2, textWidth, textHeight };
    resetDisplayTime();
    return true;
}

void WinScreen::resetDisplayTime() {
    displayStartTime = SDL_GetTicks();
}

bool WinScreen::isDisplayTimeElapsed() const {
    return SDL_GetTicks() - displayStartTime >= DISPLAY_DURATION;
}

void WinScreen::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    }

    SDL_RenderCopy(renderer, titleText, nullptr, &titleRect);

    SDL_RenderPresent(renderer);
}