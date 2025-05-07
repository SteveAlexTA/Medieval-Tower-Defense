#include "LightningTower.h"
#include "../Sound/SoundManager.h"

LightningTower::LightningTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
    : Tower(renderer, enemies, towers, x, y)
{
    type = TowerType::LIGHTNING;
    baseDamage = 200;
    baseRange = 300.0f;
    baseFireRate = 10;
    LoadTexture("Assets/Tower/spr_tower_lightning.png");
    Sound::Instance().LoadSound("lightning", "Assets/Sound/lightning_strike.wav");
    UpgradeStats();
}