#include "Game.h"
#include "TextureManager.h"
#include "Map.h"

SDL_Renderer* Game::renderer = nullptr;
Map* map;

Game::Game() : cnt(0), isRunning(false), window(nullptr) {
	window = nullptr;
	renderer = nullptr;
	isRunning = false;
}

Game::~Game() {}

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
        if (window)
        {
            std::cout << "Window created successfully!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
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
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) 
    {
        std::cout << "SDL_Image failed to load! " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
    //Decorations
    map->AddDecoration("Assets/Map/spr_tree_01_normal.png", 1, 5);
    map->AddDecoration("Assets/Map/spr_rock_02.png", 1, 7);
    map->AddDecoration("Assets/Map/spr_tree_02_normal.png", 4, 8);
	map->AddDecoration("Assets/Map/spr_tree_01_normal.png", 6, 8);
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
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
        CrossbowTower* newTower = new CrossbowTower(gridX, gridY, renderer);
        towers.push_back(newTower);
    }
}
void Game::update() {
    cnt++;
    enemySpawnTimer++;
    if (enemySpawnTimer >= 60) {
        spawnEnemy();
        enemySpawnTimer = 0;
    }
    std::cout << "Number of enemies: " << enemies.size() << std::endl;
    for (auto it = enemies.begin(); it != enemies.end();) {
        std::cout << "Moving enemy at: (" << (*it)->getX() << ", " << (*it)->getY() << ")" << std::endl;

        (*it)->move();
        std::cout << "Enemy moved to: (" << (*it)->getX() << ", " << (*it)->getY() << ")" << std::endl;

        if ((*it)->isDead()) {
            delete* it;
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Game::spawnEnemy() {
    if (!map) {
        std::cerr << "Cannot spawn enemy: Map is null!" << std::endl;
        return;
    }

    try {
        // Print out the map for debugging
        std::cout << "Map contents for path finding:" << std::endl;
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 25; x++) {
                std::cout << map->map[y][x] << " ";
            }
            std::cout << std::endl;
        }

        // Create new Goblin enemy
        Goblin* newEnemy = new Goblin(0, 0, renderer, map->map);

        // Verify path generation
        PathFinder::Point start, end;
        PathFinder::FindPathStartEnd(map->map, start, end);
        std::cout << "Path Start: (" << start.x << ", " << start.y << ")" << std::endl;
        std::cout << "Path End: (" << end.x << ", " << end.y << ")" << std::endl;

        // Add to enemies vector
        enemies.push_back(newEnemy);

        std::cout << "Enemy spawned successfully! Total enemies: " << enemies.size() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error spawning enemy: " << e.what() << std::endl;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
	map->DrawMap();
    for (auto tower : towers)
    {
        tower->Render();
    }
    for (auto enemy : enemies)
    {
        enemy->display(renderer);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}
