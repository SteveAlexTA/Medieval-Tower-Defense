#include "CannonTower.h"
#include "../Sound/SoundManager.h"

CannonTower::CannonTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
    : Tower(renderer, enemies, towers, x, y)
{
    type = TowerType::CANNON;
    baseDamage = 100;
    baseRange = 200.0f;
    baseFireRate = 30;
    LoadTexture("Assets/Tower/spr_tower_cannon.png");
    Sound::Instance().LoadSound("cannon", "Assets/Sound/battle_rifle.wav");
    UpgradeStats();
}