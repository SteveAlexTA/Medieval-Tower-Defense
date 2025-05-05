#pragma once
#include "Tower.h"

class ArcherTower : public Tower {
public:
    ArcherTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x = 0, int y = 0);
    virtual ~ArcherTower() = default;
};