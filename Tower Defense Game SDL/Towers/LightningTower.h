#pragma once
#include "Tower.h"

class LightningTower : public Tower {
public:
    LightningTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x = 0, int y = 0);
    virtual ~LightningTower() = default;
};