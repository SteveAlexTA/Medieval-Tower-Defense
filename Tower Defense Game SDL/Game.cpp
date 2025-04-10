#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Enemy's Code/Enemy.h"
#include "Enemy's Code/Wave.h"

SDL_Renderer* Game::renderer = nullptr;
Map* map;

Game::Game() : cnt(0), isRunning(false), window(nullptr) {
	window = nullptr;
	renderer = nullptr;
	isRunning = false;
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
    map = new Map();
	waveSystem = new WaveSystem(5, 1.0f);
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_Image failed to load! " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
	waveSystem = new WaveSystem(5, 1.0f);
	preloadResources();
	createEnemyPool(50);
	waveSystem->startNextWave();
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
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            placeTower(event.button.x, event.button.y);
        }
        break;
    default:
        break;
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
    if (canPlaceTower(x, y))
    {
        int gridX = (x / 32) * 32;
        int gridY = (y / 32) * 32;
        Tower* newTower = new CrossbowTower(gridX, gridY, renderer);
        towers.push_back(newTower);
    }
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
    // Check if we should spawn an enemy
    if (waveSystem->aboutToSpawnEnemy()) {
        spawnEnemy();
    }
    // If current wave is complete, advance to next wave
    if (waveSystem->isWaveCompleted() && activeEnemies.empty()) {
        waveSystem->startNextWave();
    }
    // Update towers
    for (auto tower : towers) {
        tower->Update(activeEnemies);
    }
    // Update enemies
    for (auto it = activeEnemies.begin(); it != activeEnemies.end();) {
        (*it)->move(deltaTime);
        if ((*it)->isDead()) {
            (*it)->deactivate();
            it = activeEnemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Game::spawnEnemy() {
    int wave = waveSystem->getCurrentWave();
    bool spawnSkeleton = (rand() % 100) < (10 * wave); // Increase skeleton chance by 10% per wave
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
    for (auto tower : towers)
    {
        tower->Render();
    }
    for (auto& enemy : activeEnemies) {
        if (enemy && !enemy->isDead()) {
            enemy->display(renderer);
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}
