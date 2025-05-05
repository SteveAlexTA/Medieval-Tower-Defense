#pragma once
#include "Tower.h"

class CannonTower : public Tower {
public:
    CannonTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x = 0, int y = 0);
    virtual ~CannonTower() = default;
};