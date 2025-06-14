#include "Game.h"
#include "../Managers/TextureManager.h"
#include "../States/Menu.h"
#include "../Map/Map.h"
#include "../Enemies/Enemy.h"
#include "../Enemies/Wave.h"
#include "../Enemies/Goblin.h"
#include "../Enemies/Skeleton.h"
#include "../Enemies/Demon.h"
#include "../Enemies/Dragon.h"
#include "../Towers/Tower.h"
#include "../Towers/ArcherTower.h"
#include "../Towers/CannonTower.h"
#include "../Towers/LightningTower.h"


SDL_Renderer* Game::renderer = nullptr;

Game::Game()
    : cnt(0)
    , isRunning(false)
    , window(nullptr)
    , map(nullptr)
    , waveSystem(nullptr)
    , moneySystem(nullptr)
    , UISystem(nullptr)
    , menuSystem(nullptr)
	, inputSystem(nullptr)
	, winScreen(nullptr)
	, loseScreen(nullptr)
    , m_goblinTexture(nullptr)
    , m_skeletonTexture(nullptr)
    , m_demonTexture(nullptr)
    , m_dragonTexture(nullptr)
    , m_resourcesPreloaded(false)
    , selectedTower(nullptr)
    , inMenu(true)
	, inLoseScreen(false)
	, inWinScreen(false)
	, buildTowerMode(false)
    , backgroundMusic(nullptr)
	, showMaxTowersMessage(false)
    , lives(0)
    , deltaTime(0.0f)
    , m_isFullScreen(false)
{
}

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
    delete menuSystem;
	delete inputSystem;
	delete winScreen;
	delete loseScreen;
    Sound::Instance().StopMusic();
    if (m_goblinTexture) SDL_DestroyTexture(m_goblinTexture);
    if (m_skeletonTexture) SDL_DestroyTexture(m_skeletonTexture);
    if (m_demonTexture) SDL_DestroyTexture(m_demonTexture);
    if (m_dragonTexture) SDL_DestroyTexture(m_dragonTexture);
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
        m_isFullScreen = true;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystem initialised!..." << std::endl;
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
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cout << "SDL_Image failed to load! " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
    if (!SoundManager::Instance().Init()) {
        std::cout << "Failed to initialize sound system!" << std::endl;
        isRunning = false;
        return;
    }
    loadAudioAssets();
    menuSystem = new Menu(renderer);
    if (!menuSystem->init()) {
        std::cout << "Failed to initialize menu!" << std::endl;
        isRunning = false;
        return;
    }
    winScreen = new WinScreen(renderer);
    if (!winScreen->init()) {
        std::cout << "Failed to initialize win screen!" << std::endl;
        isRunning = false;
        return;
    }
    loseScreen = new LoseScreen(renderer);
    if (!loseScreen->init()) {
        std::cout << "Failed to initialize lose screen!" << std::endl;
        isRunning = false;
        return;
    }
    preloadResources();
    map = new Map();
    waveSystem = new WaveSystem();
    createEnemyPool(50);
    moneySystem = new Money(200);
    lives = 5;
    UISystem = new UI(renderer);
    if (!UISystem->init())
    {
        std::cout << "Failed to initialize UI!" << std::endl;
        isRunning = false;
        return;
    }
    inputSystem = new InputSystem(this);
    if (!inputSystem) {
        std::cout << "Failed to initialize input system!" << std::endl;
        isRunning = false;
		return;
    }
}

void Game::startGame() {
    if (waveSystem) {
        waveSystem->startNextWave();
    }
    inMenu = false;
    SoundManager::Instance().PlayMusic("background", -1);
}

void Game::loadAudioAssets() {
    if (!SoundManager::Instance().LoadMusic("background", "Assets/Sound/game_music.mp3")) {
        std::cout << "Failed to load background music!" << std::endl;
    }
    else {
        SoundManager::Instance().PlayMusic("background", -1);
    }
    SoundManager::Instance().LoadSound("tower_place", "Assets/Sound/tower_place.wav");
    SoundManager::Instance().LoadSound("game_over", "Assets/Sound/game_over.wav");
    SoundManager::Instance().LoadSound("game_win", "Assets/Sound/game_win.wav");
    SoundManager::Instance().SetMusicVolume(70);
}

void Game::preloadResources() {
    if (!m_resourcesPreloaded) {
        m_goblinTexture = TextureManager::LoadTexture("Assets/Enemy/spr_goblin.png", renderer);
        if (!m_goblinTexture) {
            std::cout << "Failed to load enemy texture!" << std::endl;
            isRunning = false;
        }
        m_skeletonTexture = TextureManager::LoadTexture("Assets/Enemy/spr_skeleton.png", renderer);
        if (!m_skeletonTexture) {
            std::cout << "Failed to load skeleton texture!" << std::endl;
            isRunning = false;
        }
        m_demonTexture = TextureManager::LoadTexture("Assets/Enemy/spr_demon.png", renderer);
        if (!m_demonTexture) {
            std::cout << "Failed to load demon texture!" << std::endl;
            isRunning = false;
        }
        m_dragonTexture = TextureManager::LoadTexture("Assets/Enemy/spr_dragon.png", renderer);
        if (!m_dragonTexture) {
            std::cout << "Failed to load dragon texture!" << std::endl;
            isRunning = false;
        }
        m_resourcesPreloaded = true;
    }
}

void Game::createEnemyPool(int poolSize) {
    size_t halfPoolSize = poolSize / 2;
    // Pre-create enemies for object pooling
    for (int i = 0; i < halfPoolSize; ++i) {
        enemyPool.push_back(new Goblin(0, 0, renderer, map->map, m_goblinTexture));
        enemyPool.push_back(new Skeleton(0, 0, renderer, map->map, m_skeletonTexture));
        enemyPool.push_back(new Demon(0, 0, renderer, map->map, m_demonTexture));
        enemyPool.push_back(new Dragon(0, 0, renderer, map->map, m_dragonTexture));
    }
    // Deactivate all enemies initially
    for (auto& enemy : enemyPool) {
        enemy->deactivate();
    }
}

void Game::handleEvents() {
    if (inputSystem) {
        inputSystem->handleEvents();
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
    if (towers.size() >= MAX_TOWERS) {
        showMaxTowersMessage = true;
        messageStartTime = SDL_GetTicks();
        return;
    }
    TowerSelection selectedType = UISystem->getSelectedTower();
    int cost = getTowerCost(selectedType);
    if (canPlaceTower(x, y) && moneySystem->spendMoney(cost)) {
        int gridX = (x / 32) * 32;
        int gridY = (y / 32) * 32;
        Tower* newTower = createTower(selectedType, gridX, gridY);
        if (newTower) {
            towers.push_back(newTower);
            SoundManager::Instance().PlaySound("tower_place", 0);
        }
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
    int upgradeCost = tower->getUpgradePrice();
    if (upgradeCost > 0 && moneySystem->spendMoney(upgradeCost)) {
        tower->Upgrade();
    }
}

void Game::deleteTower(Tower* tower) {
    if (!tower) return;
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        if (*it == tower) {
            TowerType type = getTowerType(tower);
            int level = static_cast<int>(tower->getLevel());
			int refund = tower->calculateRefundAmount();
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
    else if (dynamic_cast<Demon*>(enemy)) {
        moneySystem->addMoney(Money::DEMON_REWARD);
    }
    else if (dynamic_cast<Dragon*>(enemy)) {
        moneySystem->addMoney(Money::DRAGON_REWARD);
    }
    else {
        std::cout << "Unknown enemy type!" << std::endl;
    }
}

Tower* Game::createTower(TowerSelection type, int x, int y) {
    switch (type) {
    case TowerSelection::ARCHER:
        return new ArcherTower(renderer, &activeEnemies, &towers, x, y);
    case TowerSelection::CANNON:
        return new CannonTower(renderer, &activeEnemies, &towers, x, y);
    case TowerSelection::LIGHTNING:
        return new LightningTower(renderer, &activeEnemies, &towers, x, y);
    default:
        return new ArcherTower(renderer, &activeEnemies, &towers, x, y);
    }
}

int Game::getTowerCost(TowerSelection type) const {
    switch (type) {
    case TowerSelection::ARCHER:
        return Money::ARCHER_TOWER_COST;
    case TowerSelection::CANNON:
        return Money::CANNON_TOWER_COST;
    case TowerSelection::LIGHTNING:
        return Money::LIGHTNING_TOWER_COST;
    default:
        return Money::ARCHER_TOWER_COST;
    }
}

TowerType Game::getTowerType(Tower* tower) const {
    if (dynamic_cast<ArcherTower*>(tower)) {
        return TowerType::ARCHER;
    }
    if (dynamic_cast<CannonTower*>(tower)) {
        return TowerType::CANNON;
    }
    if (dynamic_cast<LightningTower*>(tower)) {
        return TowerType::LIGHTNING;
    }
    return TowerType::NONE;
}

int Game::getUpgradeCost(TowerType type, int currentLevel) const {
    switch (type) {
    case TowerType::ARCHER:
        if (currentLevel == static_cast<int>(TowerLevel::LEVEL1))
            return Money::ARCHER_UPGRADE_LVL2_COST;
        if (currentLevel == static_cast<int>(TowerLevel::LEVEL2))
            return Money::ARCHER_UPGRADE_LVL3_COST;
        break;
    case TowerType::CANNON:
        if (currentLevel == static_cast<int>(TowerLevel::LEVEL1))
            return Money::CANNON_UPGRADE_LVL2_COST;
        if (currentLevel == static_cast<int>(TowerLevel::LEVEL2))
            return Money::CANNON_UPGRADE_LVL3_COST;
        break;
    case TowerType::LIGHTNING:
        if (currentLevel == static_cast<int>(TowerLevel::LEVEL1))
            return Money::LIGHTNING_UPGRADE_LVL2_COST;
        if (currentLevel == static_cast<int>(TowerLevel::LEVEL2))
            return Money::LIGHTNING_UPGRADE_LVL3_COST;
        break;
    default:
        break;
    }
    return 0;
}

void Game::update() {
    static Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = SDL_GetTicks();
    deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Calculate delta time
    if (deltaTime > 0.1f) { // Cap deltaTime to avoid large jumps
        deltaTime = 0.1f;
    }
    lastFrameTime = currentFrameTime;
    if (inMenu) {
        return;
    }
    if (inLoseScreen) {
        if (loseScreen->isDisplayTimeElapsed()) {
            inLoseScreen = false;
            inMenu = true;
            resetGame();
            SoundManager::Instance().PlayMusic("background", -1);
        }
        return;
    }
    if (inWinScreen) {
        if (winScreen->isDisplayTimeElapsed()) {
            inWinScreen = false;
            inMenu = true;
            resetGame();
            SoundManager::Instance().PlayMusic("background", -1);
        }
        return;
    }
    if (showMaxTowersMessage && SDL_GetTicks() - messageStartTime > MESSAGE_DURATION) {
        showMaxTowersMessage = false;
    }
    cnt++;
    waveSystem->update(deltaTime);

    if (waveSystem->aboutToSpawnEnemy()) {
        spawnEnemy();
    }
    if (waveSystem->isWaveCompleted() && activeEnemies.empty()) {
        waveSystem->startNextWave();
    }
    if (waveSystem->isLastWave() && activeEnemies.empty()) {
        inWinScreen = true;
        winScreen->resetDisplayTime();
        SoundManager::Instance().PlaySound("game_win");
        return;
    }
    else if (waveSystem->isWaveCompleted() && activeEnemies.empty()) {
        waveSystem->startNextWave();
    }
    for (auto tower : towers) {
        tower->Update();
    }
    for (auto it = activeEnemies.begin(); it != activeEnemies.end();) {
        (*it)->move(deltaTime);
        if ((*it)->hasReachedEnd()) {
            lives -= 1;
            if (lives <= 0) {
                lives = 0;
                inLoseScreen = true;
                loseScreen->resetDisplayTime();
                std::cout << "Game Over!" << std::endl;
                SoundManager::Instance().StopMusic();
                SoundManager::Instance().PlaySound("game_over");
                return;
            }
            (*it)->deactivate();
            it = activeEnemies.erase(it);
        }
        else if ((*it)->isDead()) {
            rewardEnemyKilled(*it);
            (*it)->deactivate();
            it = activeEnemies.erase(it);
        }
        else {
            ++it;
        }
    }
    UISystem->update(moneySystem->getMoney(), waveSystem->getCurrentWave(), lives);
}

void Game::spawnEnemy() {
    EnemyType waveEnemyType = waveSystem->getNextEnemyType();
    Enemy* spawnedEnemy = nullptr;

    if (waveEnemyType == EnemyType::SKELETON) {
        for (Enemy* enemy : enemyPool) {
            if (Skeleton* skeleton = dynamic_cast<Skeleton*>(enemy)) {
                if (!skeleton->isAlive()) {
                    spawnedEnemy = skeleton;
                    break;
                }
            }
        }
    }
    else if (waveEnemyType == EnemyType::GOBLIN) {
        for (Enemy* enemy : enemyPool) {
            if (Goblin* goblin = dynamic_cast<Goblin*>(enemy)) {
                if (!goblin->isAlive()) {
                    spawnedEnemy = goblin;
                    break;
                }
            }
        }
    }
    else if (waveEnemyType == EnemyType::DEMON) {
        for (Enemy* enemy : enemyPool) {
            if (Demon* demon = dynamic_cast<Demon*>(enemy)) {
                if (!demon->isAlive()) {
                    spawnedEnemy = demon;
                    break;
                }
            }
        }
    }
    else if (waveEnemyType == EnemyType::DRAGON) {
        for (Enemy* enemy : enemyPool) {
            if (Dragon* dragon = dynamic_cast<Dragon*>(enemy)) {
                if (!dragon->isAlive()) {
                    spawnedEnemy = dragon;
                    break;
                }
            }
        }
    }
    else
        if (!spawnedEnemy) {
            for (Enemy* enemy : enemyPool) {
                if (!enemy->isAlive()) {
                    spawnedEnemy = enemy;
                    break;
                }
            }
        }
    if (spawnedEnemy) {
        spawnedEnemy->reset(map->map);
        activeEnemies.push_back(spawnedEnemy);
    }
    else {
        std::cout << "WARNING: Unable to spawn enemy - pool exhausted" << std::endl;
    }
}

void Game::render() {
    if (inMenu) {
        menuSystem->render(renderer);
        return;
    }
    if (inLoseScreen) {
        loseScreen->render(renderer);
        return;
    }
    if (inWinScreen) {
        winScreen->render(renderer);
        return;
    }
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
    if (showMaxTowersMessage) {
        SDL_Rect messageRect = { 200, 200, 400, 50 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(renderer, &messageRect);
        UISystem->renderText("Maximum towers placed!", 280, 215, renderer);
    }
    if (buildTowerMode) { // Preview towers when drag and place
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int gridX = (mouseX / 32) * 32;
        int gridY = (mouseY / 32) * 32;
        const char* texturePath;
        switch (UISystem->getSelectedTower()) {
        case TowerSelection::CANNON:
            texturePath = "Assets/Tower/spr_tower_cannon.png";
            break;
        case TowerSelection::ARCHER:
            texturePath = "Assets/Tower/spr_tower_archer.png";
            break;
        case TowerSelection::LIGHTNING:
            texturePath = "Assets/Tower/spr_tower_lightning.png";
            break;
        default:
            texturePath = "Assets/Tower/spr_tower_archer.png";
            break;
        }
        SDL_Texture* previewTexture = TextureManager::LoadTexture(texturePath, renderer);
        SDL_Rect previewRect = { gridX, gridY, 32, 32 };
        // Show valid/invalid placement
        if (canPlaceTower(mouseX, mouseY)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
            SDL_RenderFillRect(renderer, &previewRect);
            SDL_RenderCopy(renderer, previewTexture, nullptr, &previewRect);
        }
        else {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
            SDL_RenderFillRect(renderer, &previewRect);
        }
        SDL_DestroyTexture(previewTexture);
    }
    if (inputSystem) {
        inputSystem->renderPreviewTowerWhenClicked();
    }
    SDL_RenderPresent(renderer);
}

void Game::resetGame() {
    // Clear active enemies
    activeEnemies.clear();
    // Deactivate all enemies in the pool
    for (auto& enemy : enemyPool) {
        enemy->deactivate();
    }
    // Delete all towers
    for (auto tower : towers) {
        delete tower;
    }
    towers.clear();
    selectedTower = nullptr;
    // Reset wave system
    delete waveSystem;
    waveSystem = new WaveSystem();
    // Reset money
    delete moneySystem;
    moneySystem = new Money(200);
    // Reset game status
    lives = 5;
    buildTowerMode = false;
    showMaxTowersMessage = false;
	std::cout << "Game reset" << std::endl;
}

void Game::toggleFullScreen() {
    m_isFullScreen = !m_isFullScreen;
    SDL_SetWindowFullscreen(window, m_isFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void Game::clean() {
    Sound::Instance().StopMusic();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}

