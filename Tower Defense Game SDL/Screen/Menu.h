#ifndef MENU_H
#define MENU_H
#include <SDL.h>  
#include <SDL_ttf.h>  
#include <SDL_image.h>  
#include <SDL_mixer.h>
#include "../UI/UIManager.h"  
#include "../Core/TextureManager.h"
#include <string>

class TextureManager;
class Sound;

enum class MenuState {
    MAIN_MENU,
    PLAY,
    EXIT
};

class Button {
public:
    Button(int x, int y, int w, int h, const char* texturePath, SDL_Renderer* renderer);
    ~Button();

    void render(SDL_Renderer* renderer);
    bool isClicked(int mouseX, int mouseY);
    void setHovered(bool isHovered) { hovered = isHovered; }

private:
    SDL_Rect rect;
    SDL_Texture* texture;
    bool hovered;
};

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    bool init();
    void handleEvents(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer);

    MenuState getMenuState() const { return menuState; }

private:
    MenuState menuState;
    SDL_Texture* backgroundTexture;
    SDL_Texture* titleTexture;
    Button* playButton;
    Button* exitButton;
    TTF_Font* titleFont;

    void createTitle(SDL_Renderer* renderer);
};
#endif