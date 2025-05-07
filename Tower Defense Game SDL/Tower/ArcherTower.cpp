#include "ArcherTower.h"
#include "../Sound/SoundManager.h"

ArcherTower::ArcherTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
    : Tower(renderer, enemies, towers, x, y)
{
    type = TowerType::ARCHER;
    baseDamage = 50;
    baseRange = 150.0f;
    baseFireRate = 50;
    LoadTexture("Assets/Tower/spr_tower_archer.png");
    Sound::Instance().LoadSound("arrow", "Assets/Sound/arrow.wav");
    UpgradeStats();
}