#pragma once
#include "Tower.h"

class HumanSoldier : public Tower {
public:
    HumanSoldier(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x = 0, int y = 0);
    virtual ~HumanSoldier() = default;
};