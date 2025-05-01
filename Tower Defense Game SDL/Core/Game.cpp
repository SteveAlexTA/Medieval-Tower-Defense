#include "Game.h"
#include "TextureManager.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Wave.h"
#include "../Enemy/Goblin.h"
#include "../Enemy/Skeleton.h"
#include "../Enemy/Demon.h"
#include "../Enemy/Dragon.h"
#include "../Tower/Tower.h"
#include "../Tower/ArcherTower.h"
#include "../Tower/CannonTower.h"

SDL_Renderer* Game::renderer = nullptr;

Game::Game()
    : cnt(0)
    , isRunning(false)
    , window(nullptr)
    , map(nullptr)
    , waveSystem(nullptr)
	, moneySystem(nullptr)
	, UISystem(nullptr)
    , m_goblinTexture(nullptr)
    , m_skeletonTexture(nullptr)
	, m_demonTexture(nullptr)
	, m_dragonTexture(nullptr)
    , m_resourcesPreloaded(false)
    , selectedTower(nullptr)
    , deltaTime(0.0f)
{}

Game::~Game() {
	for (auto enemy : enemyPool) 
    {
		delete enemy;
	}
    enemyPool.clear();

	for (auto tower : towers) 
    {
		delete tower;
	}
	towers.clear();

	delete waveSystem;
    delete map;
    delete moneySystem;
	delete UISystem;

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
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
    {
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
    if (!(IMG_Init(imgFlags) & imgFlags)) 
    {
        std::cout << "SDL_Image failed to load! " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
    if (!Sound::Init()) 
    {
        std::cout << "Sound system failed to initialize!" << std::endl;
        isRunning = false;
        return;
    }
	preloadResources();
    map = new Map();
    waveSystem = new WaveSystem();
	createEnemyPool(50);
	waveSystem->startNextWave();
	moneySystem = new Money(200);
    lives = 3;
    gameOver = false;
	initBackgroundMusic();
    UISystem = new UI(renderer);
	if (!UISystem->init()) 
    {
		std::cout << "Failed to initialize UI!" << std::endl;
		isRunning = false;
        return;
	}
}

void Game::initBackgroundMusic() 
{
    backgroundMusic = Sound::GetMusic("Assets/Sound/easy_mode_background_music.mp3");

    if (!backgroundMusic) 
    {
        std::cout << "Failed to load background music!" << std::endl;
        return;
    }

    // Set music volume (0-128)
    Sound::SetMusicVolume(128);  // 100% volume

    // Loop -1 = infinite
    Sound::PlayMusic(backgroundMusic, -1);
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
    int halfPoolSize = poolSize / 2;
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
    SDL_Event event;
    int mouseX = 0;
	int mouseY = 0;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION) {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            mouseX = event.button.x;
            mouseY = event.button.y;
        }
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEMOTION:
            UISystem->archerTowerHovered = UISystem->isArcherTowerClicked(mouseX, mouseY);
            UISystem->cannonTowerHovered = UISystem->isCannonTowerClicked(mouseX, mouseY);

            break;
        case SDL_MOUSEBUTTONDOWN:
            if (UISystem->isArcherTowerClicked(mouseX, mouseY)) {
                buildTowerMode = !buildTowerMode;
                if (buildTowerMode && selectedTower) {
                    selectedTower->setSelected(false);
                    selectedTower = nullptr;
                }
                return;
            }
            else if (UISystem->isCannonTowerClicked(mouseX, mouseY)) {
                UISystem->setSelectedTower(TowerSelection::CANNON);
                buildTowerMode = !buildTowerMode;
                if (buildTowerMode && selectedTower) {
                    selectedTower->setSelected(false);
                    selectedTower = nullptr;
                }
                return;
            }
            if (buildTowerMode) {
                placeTower(mouseX, mouseY);
                buildTowerMode = false;
                UISystem->resetSelectedTower();
                return;
            }
            if (event.button.button == SDL_BUTTON_LEFT) {
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
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (selectedTower) {
                    selectedTower->setSelected(false);
                    selectedTower = nullptr;
                }
				if (buildTowerMode) {
					buildTowerMode = false;
					UISystem->resetSelectedTower();
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
    TowerSelection selectedType = UISystem->getSelectedTower();
    int cost = getTowerCost(selectedType);
    if (canPlaceTower(x, y) && moneySystem->spendMoney(cost)) {
        int gridX = (x / 32) * 32;
        int gridY = (y / 32) * 32;
        Tower* newTower = createTower(selectedType, gridX, gridY);
        if (newTower) {
            towers.push_back(newTower);
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
    TowerType type = getTowerType(tower);
    int level = static_cast<int>(tower->getLevel());
    int upgradeCost = getUpgradeCost(type, level);

    if (upgradeCost > 0 && moneySystem->spendMoney(upgradeCost)) {
        tower->upgrade();
    }
}

void Game::deleteTower(Tower* tower) {
    if (!tower) return;
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        if (*it == tower) {
            TowerType type = getTowerType(tower);
            int level = static_cast<int>(tower->getLevel());
            int refund = getRefundAmount(type, level);
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

Tower* Game::createTower(TowerSelection type, int x, int y) {
    switch (type) {
    case TowerSelection::ARCHER:
        return new ArcherTower(x, y, renderer, 25);
    case TowerSelection::CANNON:
        return new CannonTower(x, y, renderer, 35);
    default:
        return new ArcherTower(x, y, renderer, 25);
    }
}

int Game::getTowerCost(TowerSelection type) const {
    switch (type) {
    case TowerSelection::ARCHER:
        return Money::ARCHER_TOWER_COST;
    case TowerSelection::CANNON:
        return Money::CANNON_TOWER_COST;
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
    }
    return 0;
}

int Game::getRefundAmount(TowerType type, int level) const {
    int totalCost = 0;
    switch (type) {
    case TowerType::ARCHER:
        totalCost = Money::ARCHER_TOWER_COST;
        if (level >= static_cast<int>(TowerLevel::LEVEL2))
            totalCost += Money::ARCHER_UPGRADE_LVL2_COST;
        if (level >= static_cast<int>(TowerLevel::LEVEL3))
            totalCost += Money::ARCHER_UPGRADE_LVL3_COST;
        break;
    case TowerType::CANNON:
        totalCost = Money::CANNON_TOWER_COST;
        if (level >= static_cast<int>(TowerLevel::LEVEL2))
            totalCost += Money::CANNON_UPGRADE_LVL2_COST;
        if (level >= static_cast<int>(TowerLevel::LEVEL3))
            totalCost += Money::CANNON_UPGRADE_LVL3_COST;
        break;
    }
    return totalCost / 2; 
}

void Game::update() {
    static Uint32 lastFrameTime = SDL_GetTicks();
	Uint32 currentFrameTime = SDL_GetTicks();
	deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Calculate delta time
	if (deltaTime > 0.1f) { // Cap deltaTime to avoid large jumps
		deltaTime = 0.1f;
	}
	lastFrameTime = currentFrameTime;
	if (gameOver) {
		return;
	}

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
		if ((*it)->hasReachedEnd()) {
			lives -= 1;
			if (lives <= 0) {
                lives = 0;
				gameOver = true;
				std::cout << "Game Over!" << std::endl;
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
    if (buildTowerMode) {
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
    if (gameOver) {
        SDL_Rect gameOverRect = { 200, 150, 400, 300 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(renderer, &gameOverRect);
        UISystem->renderText("GAME OVER", 320, 280, renderer);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
	Sound::Clean();
    Sound::StopMusic();
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}
