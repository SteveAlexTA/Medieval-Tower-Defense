#pragma once
#include <SDL.h>
#include "../Core/TextureManager.h"
#include "../UI & Sound/SoundManager.h"
#include "../Enemy/Enemy.h"
#include <vector>
#include "Tower.h"

class ShooterProjectile;
class ShooterTower : public Tower {
public:
    ShooterTower(int x, int y, SDL_Renderer* renderer, int damage);
    virtual ~ShooterTower();
    virtual void Update(std::vector<Enemy*>& enemies) override;
    virtual void Render() override;
    virtual void shoot(std::vector<Enemy*>& enemies) override;
    virtual void upgrade() override;

private:
    std::vector<ShooterProjectile*> projectiles;
    Mix_Chunk* shootSound;
};

class ShooterProjectile {
public:
    ShooterProjectile(int x, int y, Enemy* target, SDL_Renderer* renderer);
    ~ShooterProjectile();
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

