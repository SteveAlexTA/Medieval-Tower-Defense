#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Enemy's Code/Enemy.h"
#include "Enemy's Code/Wave.h"

SDL_Renderer* Game::renderer = nullptr;

Game::Game()
    : cnt(0)
    , isRunning(false)
    , window(nullptr)
    , map(nullptr)
    , waveSystem(nullptr)
	, moneySystem(nullptr)
	, UISystem(nullptr)
    , m_enemyTexture(nullptr)
    , m_skeletonTexture(nullptr)
    , m_resourcesPreloaded(false)
    , selectedTower(nullptr)
    , deltaTime(0.0f)
{}

Game::~Game() {
	for (auto enemy : enemyPool) {
		delete enemy;
	}
    enemyPool.clear();
	for (auto tower : towers) {
		delete tower;
	}
	towers.clear();
	delete waveSystem;
    delete map;
    delete moneySystem;
	delete UISystem;
	if (m_enemyTexture) SDL_DestroyTexture(m_enemyTexture);
	if (m_skeletonTexture) SDL_DestroyTexture(m_skeletonTexture);
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystem Initialised!..." << std::endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window created successfully!" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created successfully!" << std::endl;
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_Image failed to load! " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
	preloadResources();
    map = new Map();
    waveSystem = new WaveSystem();
	createEnemyPool(50);
	waveSystem->startNextWave();
	moneySystem = new Money(200);
	UISystem = new UI(renderer);
	if (!UISystem->init()) {
		std::cout << "Failed to initialize UI!" << std::endl;
		isRunning = false;
        return;
	}
}
void Game::preloadResources() {
    if (!m_resourcesPreloaded) {
        m_enemyTexture = TextureManager::LoadTexture("Assets/Enemy/spr_goblin.png", renderer);
        if (!m_enemyTexture) {
            std::cout << "Failed to load enemy texture!" << std::endl;
            isRunning = false;
        }
        m_skeletonTexture = TextureManager::LoadTexture("Assets/Enemy/spr_skeleton.png", renderer);
        if (!m_skeletonTexture) {
            std::cout << "Failed to load skeleton texture!" << std::endl;
            isRunning = false;
        }
        m_resourcesPreloaded = true;
    }
}
void Game::createEnemyPool(int poolSize) {
    int halfPoolSize = poolSize / 2;
    // Pre-create enemies for object pooling
    for (int i = 0; i < halfPoolSize; ++i) {
        enemyPool.push_back(new Goblin(0, 0, renderer, map->map, m_enemyTexture));
        enemyPool.push_back(new Skeleton(0, 0, renderer, map->map, m_skeletonTexture));
    }
    // Deactivate all enemies initially
    for (auto& enemy : enemyPool) {
        enemy->deactivate();
    }
}
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (selectedTower) {
                    if (isClickInUpgradeUI(mouseX, mouseY, selectedTower)) {
                        upgradeTower(selectedTower);
                        return;
                    }
                    else if (isClickInDeleteUI(mouseX, mouseY, selectedTower)) {
                        deleteTower(selectedTower);
                        return;
                    }
                }
                selectTowerAt(mouseX, mouseY);
                if (!selectedTower) {
                    placeTower(mouseX, mouseY);
                }
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (selectedTower) {
                    selectedTower->setSelected(false);
                    selectedTower = nullptr;
                }
            }
            break;
        default:
            break;
        }
    }
}
bool Game::canPlaceTower(int x, int y) {
    int gridX = x / 32;
    int gridY = y / 32;
    if (gridX < 0 || gridX >= 25 || gridY < 0 || gridY >= 20) {
        return false;
    }
    return !map->IsEnemyPath(gridY, gridX);
}
void Game::placeTower(int x, int y) {
    if (canPlaceTower(x, y) && moneySystem->spendMoney(Money::TOWER_BASE_COST)) {
        int gridX = (x / 32) * 32;
        int gridY = (y / 32) * 32;
        Tower* newTower = new BaseTowerOne(gridX, gridY, renderer, 25);
        towers.push_back(newTower);
	}
    else {
        std::cout << "Cannot place tower here!" << std::endl;
    }
}
void Game::selectTowerAt(int x, int y) {
    if (selectedTower) {
        selectedTower->setSelected(false);
        selectedTower = nullptr;
    }
    for (auto& tower : towers) {
        int towerX = tower->getX();
        int towerY = tower->getY();
        if (x >= towerX && x < towerX + 32 && y >= towerY && y < towerY + 32) {
            selectedTower = tower;
            selectedTower->setSelected(true);
            return;
        }
    }
}
void Game::upgradeTower(Tower* tower) {
    if (!tower || !tower->canUpgrade()) return;
    int upgradeCost = 0;
    if (tower->getLevel() == TowerLevel::LEVEL2) {
        upgradeCost = Money::TOWER_UPGRADE_ARCHER_COST;
    }
    else if (tower->getLevel() == TowerLevel::LEVEL3) {
        upgradeCost = Money::TOWER_UPGRADE_CANNON_COST;
    }
    if (moneySystem->spendMoney(upgradeCost)) {
        tower->upgrade();
    }
}
void Game::deleteTower(Tower* tower) {
    if (!tower) return;
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        if (*it == tower) {
			int refund = 0;
            switch (tower->getLevel()) {
            case TowerLevel::LEVEL1:
                refund = Money::TOWER_BASE_COST / 2; //1/2 the original cost
                break;
            case TowerLevel::LEVEL2:
                refund = (Money::TOWER_BASE_COST + Money::TOWER_UPGRADE_ARCHER_COST) / 2;
                break;
            case TowerLevel::LEVEL3:
                refund = (Money::TOWER_BASE_COST + Money::TOWER_UPGRADE_ARCHER_COST + Money::TOWER_UPGRADE_CANNON_COST) / 2;
                break;
            }
			moneySystem->addMoney(refund);
            delete* it;
            towers.erase(it);
            selectedTower = nullptr;
            return;
        }
    }
}
void Game::rewardEnemyKilled(Enemy* enemy) {
    if (dynamic_cast<Goblin*>(enemy)) {
        moneySystem->addMoney(Money::GOBLIN_REWARD);
    }
    else if (dynamic_cast<Skeleton*>(enemy)) {
        moneySystem->addMoney(Money::SKELETON_REWARD);
    }
}
bool Game::isClickInUpgradeUI(int mouseX, int mouseY, Tower* tower) {
    if (!tower || !tower->canUpgrade()) return false;
    int towerX = tower->getX();
    int towerY = tower->getY();
    SDL_Rect upgradeRect = { towerX - 16, towerY - 40, 32, 32 };
    return (mouseX >= upgradeRect.x && mouseX < upgradeRect.x + upgradeRect.w && mouseY >= upgradeRect.y && mouseY < upgradeRect.y + upgradeRect.h);
}

bool Game::isClickInDeleteUI(int mouseX, int mouseY, Tower* tower) {
    if (!tower) return false;
    int towerX = tower->getX();
    int towerY = tower->getY();
    SDL_Rect deleteRect = { towerX + 16, towerY - 40, 32, 32 };
    return (mouseX >= deleteRect.x && mouseX < deleteRect.x + deleteRect.w && mouseY >= deleteRect.y && mouseY < deleteRect.y + deleteRect.h);
}
void Game::update() {
    static Uint32 lastFrameTime = SDL_GetTicks();
	Uint32 currentFrameTime = SDL_GetTicks();
	deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Calculate delta time
	if (deltaTime > 0.1f) { // Cap deltaTime to avoid large jumps
		deltaTime = 0.1f;
	}
	lastFrameTime = currentFrameTime;

    cnt++;
    waveSystem->update(deltaTime);

    if (waveSystem->aboutToSpawnEnemy()) {
        spawnEnemy();
    }
    if (waveSystem->isWaveCompleted() && activeEnemies.empty()) {
        waveSystem->startNextWave();
    }
    for (auto tower : towers) {
        tower->Update(activeEnemies);
    }
    for (auto it = activeEnemies.begin(); it != activeEnemies.end();) {
        (*it)->move(deltaTime);
        if ((*it)->isDead()) {
			rewardEnemyKilled(*it);
            (*it)->deactivate();
            it = activeEnemies.erase(it);
        }
        else {
            ++it;
        }
    }
	UISystem->update(moneySystem->getMoney(), waveSystem->getCurrentWave(), 100); 
}

void Game::spawnEnemy() {
    int wave = waveSystem->getCurrentWave();
    EnemyType waveEnemyType = waveSystem->getCurrentEnemyType();
    bool spawnSkeleton = false; 

    switch (waveEnemyType) {
	case EnemyType::GOBLIN:
		spawnSkeleton = false;
		break;
	case EnemyType::SKELETON:
		spawnSkeleton = true;
		break;
    default:
        break;
    }
	Enemy* spawnedEnemy = nullptr;
    if (spawnSkeleton) {
        for (Enemy* enemy : enemyPool) {
            if (Skeleton* skeleton = dynamic_cast<Skeleton*>(enemy)) {
                if (!skeleton->isAlive()) {
                    spawnedEnemy = skeleton;
                    break;
                }
            }
        }
    }
    else {
        for (Enemy* enemy : enemyPool) {
            if (Goblin* goblin = dynamic_cast<Goblin*>(enemy)) {
                if (!goblin->isAlive()) {
                    spawnedEnemy = goblin;
                    break;
                }
            }
        }
    }
    // If no preferred type is available, use any available enemy
    if (!spawnedEnemy) {
        for (Enemy* enemy : enemyPool) {
            if (!enemy->isAlive()) {
                spawnedEnemy = enemy;
                break;
            }
        }
    }
    // If we found an enemy to spawn, reset it and add to active enemies
    if (spawnedEnemy) {
        spawnedEnemy->reset(map->map);
        activeEnemies.push_back(spawnedEnemy);
    }
    else {
        std::cout << "WARNING: Unable to spawn enemy - pool exhausted" << std::endl;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    map->DrawMap();
    for (auto tower : towers) {
        tower->Render();
    }
    for (auto& enemy : activeEnemies) {
        if (enemy && !enemy->isDead()) {
            enemy->display(renderer);
        }
    }
    UISystem->render(renderer);
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}
