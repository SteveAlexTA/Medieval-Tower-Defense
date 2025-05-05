#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "../Map/Map.h"
#include "../Tower/Tower.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Wave.h"
#include "Money.h"
#include "Menu.h"
#include "../UI/UIManager.h"
#include "../Sound/SoundManager.h"

class Map;
class Tower;
class Enemy;
class Menu;
class UI;
class Money;
class WaveSystem;
enum class TowerType;
enum class MenuState;

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
    bool isGameOver() const { return gameOver; }
    int getBaseLives() const { return lives; }

private:
    void preloadResources();
    void createEnemyPool(int poolSize = 100);
	void initBackgroundMusic();
    void startGame();
    void loadAudioAssets();

    int cnt;
    bool isRunning;
    SDL_Window* window;
    Map* map;
    std::vector<Tower*> towers;
    std::vector<Enemy*> activeEnemies;
    std::vector<Enemy*> enemyPool;
    float deltaTime;

    int lives = 3;
	bool gameOver = false;

	WaveSystem* waveSystem;
    Money* moneySystem;
    UI* UISystem;
    Menu* menuSystem;

    SDL_Texture* m_goblinTexture = nullptr;
	SDL_Texture* m_skeletonTexture = nullptr;
    SDL_Texture* m_demonTexture = nullptr;
	SDL_Texture* m_dragonTexture = nullptr;

    bool m_resourcesPreloaded = false;
	bool buildTowerMode = false;
    bool inMenu = true;

    Tower* selectedTower = nullptr;
    void selectTowerAt(int x, int y);
    void upgradeTower(Tower* tower);
    void deleteTower(Tower* tower);
    bool isClickInUpgradeUI(int mouseX, int mouseY, Tower* tower);
    bool isClickInDeleteUI(int mouseX, int mouseY, Tower* tower);

    static const int MAX_TOWERS = 8;
    bool showMaxTowersMessage = false;
    Uint32 messageStartTime = 0;
    const Uint32 MESSAGE_DURATION = 2000;

    Tower* createTower(TowerSelection type, int x, int y);
    int getTowerCost(TowerSelection type) const;
    TowerType getTowerType(Tower* tower) const;
    int getUpgradeCost(TowerType type, int currentLevel) const;
    int getRefundAmount(TowerType type, int level) const;

    void rewardEnemyKilled(Enemy* enemy);

    Mix_Music* backgroundMusic;
};

#endif //GAME_H

