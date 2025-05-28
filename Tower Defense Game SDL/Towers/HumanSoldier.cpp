#include "HumanSoldier.h"
#include "../Sound/SoundManager.h"

HumanSoldier::HumanSoldier(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
    : Tower(renderer, enemies, towers, x, y)
{
    type = TowerType::SOLDIER;
    baseDamage = 50;
    baseRange = 220.0f;
    baseFireRate = 600; //10 shots per 1 seconds (10x60fps)
    LoadTexture("Assets/Tower/spr_human_soldier.png");
    Sound::Instance().LoadSound("bullet", "Assets/Sound/battle_rifle.wav");
    srcRect = { 0, 0, 32, 32 };
    destRect = { towerX, towerY, 32, 32 };
    UpgradeStats();
}