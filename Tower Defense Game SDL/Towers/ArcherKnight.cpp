#include "ArcherKnight.h"
#include "../Sound/SoundManager.h"

ArcherKnight::ArcherKnight(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
    : Tower(renderer, enemies, towers, x, y)
{
    type = TowerType::ARCHERKNIGHT;
    baseDamage = 30;
    baseRange = 200.0f;
    baseFireRate = 60; //1 shot per 1 seconds (1x60fps)
    LoadTexture("Assets/Tower/spr_human_archer.png");
    Sound::Instance().LoadSound("arrow", "Assets/Sound/arrow.wav");
    srcRect = { 0, 0, 32, 32 };
    destRect = { towerX, towerY, 32, 32 };
    UpgradeStats();
}