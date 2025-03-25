#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#pragma once
#include <iostream>
#include <vector>
#include "Map.h"
#include "Towers Code/CrossbowTower.h"
class Map;
class CrossbowTower;
class Game {
public:
    Game();
    virtual ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() { return isRunning; }
    void placeTower(int x, int y);
    bool canPlaceTower(int x, int y);
	static SDL_Renderer* renderer;

private:
    int cnt;
    bool isRunning;
    SDL_Window* window;
    Map* map;
    std::vector<CrossbowTower*> towers;
};

#endif //GAME_H

