#include "LoseScreen.h"
#include "../Core/TextureManager.h"
#include <SDL_ttf.h>

LoseScreen::LoseScreen(SDL_Renderer* renderer)
    : renderer(renderer)
    , titleText(nullptr)
    , titleRect{ 0, 0, 0, 0 }
    , backgroundTexture(nullptr)
    , font(nullptr)
    , displayStartTime(0)
{
}

LoseScreen::~LoseScreen() {
    if (titleText) SDL_DestroyTexture(titleText);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (font) TTF_CloseFont(font);
}

bool LoseScreen::init() {
    backgroundTexture = TextureManager::LoadTexture("Assets/Icon/lose_background.png", renderer);
    if (!backgroundTexture) {
        return false;
    }

    font = TTF_OpenFont("Assets/Icon/consola.ttf", 72);
    if (!font) {
        return false;
    }

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, "YOU LOSE", textColor);
    titleText = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int textWidth, textHeight;
    SDL_QueryTexture(titleText, NULL, NULL, &textWidth, &textHeight);

    titleRect = { (800 - textWidth) / 2, (600 - textHeight) / 2, textWidth, textHeight };
    resetDisplayTime();

    return true;
}

void LoseScreen::resetDisplayTime() {
    displayStartTime = SDL_GetTicks();
}

bool LoseScreen::isDisplayTimeElapsed() const {
    return SDL_GetTicks() - displayStartTime >= DISPLAY_DURATION;
}

void LoseScreen::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    }

    SDL_RenderCopy(renderer, titleText, nullptr, &titleRect);

    SDL_RenderPresent(renderer);
}