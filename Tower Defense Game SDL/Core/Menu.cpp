#include "Menu.h"
#include "../Sound/SoundManager.h"
#include <iostream>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

Button::Button(int x, int y, int w, int h, const char* texturePath, SDL_Renderer* renderer)
    : hovered(false)
{
    rect = { x, y, w, h };
    texture = TextureManager::LoadTexture(texturePath, renderer);
    if (!texture) {
        std::cout << "Failed to load button texture: " << texturePath << std::endl;
    }
}

Button::~Button() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Button::render(SDL_Renderer* renderer) {
    if (!texture) return;

    // Draw button with highlight if hovered
    if (hovered) {
        SDL_SetTextureColorMod(texture, 255, 255, 150); 
    }
    else {
        SDL_SetTextureColorMod(texture, 255, 255, 255); 
    }
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

bool Button::isClicked(int mouseX, int mouseY) {
    return (mouseX >= rect.x && mouseX < rect.x + rect.w && mouseY >= rect.y && mouseY < rect.y + rect.h);
}

Menu::Menu(SDL_Renderer* renderer)
    : menuState(MenuState::MAIN_MENU),
    backgroundTexture(nullptr),
    titleTexture(nullptr),
    playButton(nullptr),
    exitButton(nullptr),
    titleFont(nullptr)
{
    if (!TTF_WasInit()) {
        if (TTF_Init() == -1) {
            std::cout << "TTF initialization failed: " << TTF_GetError() << std::endl;
            return;
        }
    }

	if (!Sound::Instance().Init()) {
        std::cout << "Sound initialization failed!" << std::endl;
	}
    // Load assets
    backgroundTexture = TextureManager::LoadTexture("Assets/Icon/menu_background.png", renderer);
    if (!backgroundTexture) {
        std::cout << "Failed to load menu background!" << std::endl;
    }

    // Create buttons
    playButton = new Button(300, 300, 200, 64, "Assets/Icon/play_button.png", renderer);
    exitButton = new Button(300, 400, 200, 64, "Assets/Icon/exit_button.png", renderer);

    // Load title font
    titleFont = TTF_OpenFont("Assets/Icon/consola.ttf", 72);
    if (!titleFont) {
        std::cout << "Failed to load title font: " << TTF_GetError() << std::endl;
    }
    createTitle(renderer);
}

Menu::~Menu() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (titleTexture) SDL_DestroyTexture(titleTexture);
    if (titleFont) TTF_CloseFont(titleFont);
    delete playButton;
    delete exitButton;
}

void Menu::createTitle(SDL_Renderer* renderer) {
    if (!titleFont) return;

    SDL_Color titleColor = { 139, 69, 19, 255 }; 
    SDL_Surface* titleSurface = TTF_RenderText_Blended(titleFont, "MEDIEVAL TOWER DEFENSE", titleColor);
    if (!titleSurface) {
        std::cout << "Failed to create title surface: " << TTF_GetError() << std::endl;
        return;
    }

    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);

    if (!titleTexture) {
        std::cout << "Failed to create title texture: " << SDL_GetError() << std::endl;
    }
}

bool Menu::init() {
    return backgroundTexture && playButton && exitButton;
}

void Menu::handleEvents(SDL_Event& event) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Update button hover states
    playButton->setHovered(playButton->isClicked(mouseX, mouseY));
    exitButton->setHovered(exitButton->isClicked(mouseX, mouseY));

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (playButton->isClicked(mouseX, mouseY)) {
                menuState = MenuState::PLAY;
			}
            else if (exitButton->isClicked(mouseX, mouseY)) {
                menuState = MenuState::EXIT;
            }
        }
    }
}

void Menu::update() {}

void Menu::render(SDL_Renderer* renderer) {
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw background
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    }

    // Draw title
    if (titleTexture) {
        SDL_Rect titleRect = { 100, 100, 600, 100 };
        SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    }

    // Draw buttons
    playButton->render(renderer);
    exitButton->render(renderer);

    // Present the renderer
    SDL_RenderPresent(renderer);
}