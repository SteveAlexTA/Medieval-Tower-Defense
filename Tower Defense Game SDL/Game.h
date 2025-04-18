#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_image.h>
#pragma once
#include <iostream>
#include <vector>
#include "Map.h"
#include "Tower's Code/Tower.h"
#include "Enemy's Code/Enemy.h"
#include "Enemy's Code/Wave.h"
#include "Money.h"
#include "UIManager.h"
class Map;
class Tower;
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
    void spawnEnemy();
	static SDL_Renderer* renderer;
private:
    void preloadResources();
    void createEnemyPool(int poolSize = 100);

    int cnt;
    bool isRunning;
    SDL_Window* window;
    Map* map;
    std::vector<Tower*> towers;
    std::vector<Enemy*> activeEnemies;
    std::vector<Enemy*> enemyPool;
    float deltaTime;

	WaveSystem* waveSystem;
    Money* moneySystem;
    UI* UISystem;

    SDL_Texture* m_enemyTexture = nullptr;
	SDL_Texture* m_skeletonTexture = nullptr;
    bool m_resourcesPreloaded = false;

    Tower* selectedTower = nullptr;
    void selectTowerAt(int x, int y);
    void upgradeTower(Tower* tower);
    void deleteTower(Tower* tower);
    bool isClickInUpgradeUI(int mouseX, int mouseY, Tower* tower);
    bool isClickInDeleteUI(int mouseX, int mouseY, Tower* tower);

    void rewardEnemyKilled(Enemy* enemy);
};

#endif //GAME_H

