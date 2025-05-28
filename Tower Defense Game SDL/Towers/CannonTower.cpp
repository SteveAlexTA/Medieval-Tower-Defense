#include "CannonTower.h"
#include "../Managers/SoundManager.h"

CannonTower::CannonTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
    : Tower(renderer, enemies, towers, x, y)
{
    type = TowerType::CANNON;
    baseDamage = 60;
    baseRange = 250.0f;
	baseFireRate = 90; //1 shot per 1.5 seconds (1.5x60fps)
    LoadTexture("Assets/Tower/spr_tower_cannon.png");
    Sound::Instance().LoadSound("cannon", "Assets/Sound/cannon.wav");
    UpgradeStats();
}