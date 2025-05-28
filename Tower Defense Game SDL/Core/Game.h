#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "../Map/Map.h"
#include "../Towers/Tower.h"
#include "../Enemies/Enemy.h"
#include "../Enemies/Wave.h"
#include "../Systems/Money.h"
#include "../States/Menu.h"
#include "../States/LoseScreen.h"
#include "../States/WinScreen.h"
#include "../Managers/UIManager.h"
#include "../Managers/SoundManager.h"
#include "../Systems/InputSystem.h"

class Map;
class Tower;
class Enemy;
class Menu;
class WinScreen;
class LoseScreen;
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
    SDL_Window* getWindow() const { return window; }
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }
    void placeTower(int x, int y);
    bool canPlaceTower(int x, int y);
    void spawnEnemy();
    static SDL_Renderer* renderer;
    int getBaseLives() const { return lives; }

	void setRunning(bool running) { isRunning = running; }
	bool isInMenu() const { return inMenu; }
	bool isInWinScreen() const { return inWinScreen; }
	bool isInLoseScreen() const { return inLoseScreen; }
	bool isBuildTowerMode() const { return buildTowerMode; }
	void setBuildTowerMode(bool mode) { buildTowerMode = mode; }

	Tower* getSelectedTower() const { return selectedTower; }
	void setSelectedTower(Tower* tower) { selectedTower = tower; }

	Menu* getMenuSystem() const { return menuSystem; }

	UI* getUISystem() const { return UISystem; }

    void startGame();
	void selectTowerAt(int x, int y);
	void upgradeTower(Tower* tower);
	void deleteTower(Tower* tower);

private:
    void preloadResources();
    void createEnemyPool(int poolSize = 100);
    void loadAudioAssets();
	void resetGame();

    int cnt;
    bool isRunning; 
    SDL_Window* window;
    Map* map;
    Mix_Music* backgroundMusic;
    std::vector<Tower*> towers;
    std::vector<Enemy*> activeEnemies;
    std::vector<Enemy*> enemyPool;
    float deltaTime;
    int lives;

    WaveSystem* waveSystem;
    Money* moneySystem;
    WinScreen* winScreen;
    LoseScreen* loseScreen;
    UI* UISystem;
    Menu* menuSystem;
	InputSystem* inputSystem;

    SDL_Texture* m_goblinTexture;
    SDL_Texture* m_skeletonTexture;
    SDL_Texture* m_demonTexture;
    SDL_Texture* m_dragonTexture;

    bool m_resourcesPreloaded;
    bool buildTowerMode;
    bool inMenu;
    bool inLoseScreen;           
    bool inWinScreen;

    Tower* selectedTower;
    static const int MAX_TOWERS = 12;
    bool showMaxTowersMessage = false;
    Uint32 messageStartTime = 0;
    const Uint32 MESSAGE_DURATION = 2000;

    Tower* createTower(TowerSelection type, int x, int y);
    int getTowerCost(TowerSelection type) const;
    TowerType getTowerType(Tower* tower) const;
    int getUpgradeCost(TowerType type, int currentLevel) const;
    void rewardEnemyKilled(Enemy* enemy);

};

#endif //GAME_H
