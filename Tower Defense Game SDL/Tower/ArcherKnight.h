#pragma once
#include "Tower.h"

class ArcherKnight : public Tower {
public:
    ArcherKnight(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x = 0, int y = 0);
    virtual ~ArcherKnight() = default;
};