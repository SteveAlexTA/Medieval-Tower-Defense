#pragma once
#include <SDL.h>
#include "../Core/TextureManager.h"
#include "../UI & Sound/SoundManager.h"
#include "../Enemy/Enemy.h"
#include <vector>
#include "Tower.h"

class LightningProjectile;
class LightningTower : public Tower {
public:
    LightningTower(int x, int y, SDL_Renderer* renderer, int damage);
    virtual ~LightningTower();
    virtual void Update(std::vector<Enemy*>& enemies) override;
    virtual void Render() override;
    virtual void shoot(std::vector<Enemy*>& enemies) override;
    virtual void upgrade() override;

private:
    std::vector<LightningProjectile*> projectiles;
    Mix_Chunk* shootSound;
};

class LightningProjectile {
public:
    LightningProjectile(int x, int y, Enemy* target, SDL_Renderer* renderer);
    ~LightningProjectile();
    void Update();
    void Render();
    bool isOutOfBounds() const;
    bool enemyHit();
    Enemy* getTarget() const { return target; }

private:
    int x, y;
    SDL_Texture* texture;
    SDL_Rect dest;
    SDL_Rect src;
    SDL_Renderer* renderer;
    Enemy* target;
    float speed;
};
