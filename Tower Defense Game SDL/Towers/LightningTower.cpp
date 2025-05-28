#include "LightningTower.h"
#include "../Managers/SoundManager.h"

LightningTower::LightningTower(SDL_Renderer* renderer, std::vector<Enemy*>* enemies, std::vector<Tower*>* towers, int x, int y)
    : Tower(renderer, enemies, towers, x, y)
{
    type = TowerType::LIGHTNING;
    baseDamage = 100;
    baseRange = 300.0f;
	baseFireRate = 120; //1 shot every 2 seconds (2x60fps)
    LoadTexture("Assets/Tower/spr_tower_lightning.png");
    Sound::Instance().LoadSound("lightning", "Assets/Sound/lightning_strike.wav");
    UpgradeStats();
}