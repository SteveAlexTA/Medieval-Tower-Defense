#pragma once
#include "../Map/PathFinder.h"
#include "HealthBar.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <map>
class Enemy {
public:
	Enemy(float x, float y, int hp, float speed);
	virtual ~Enemy(); //Virtual for polymorphism with multiple enemy types

    bool isDead() const { return m_hp <= 0; }
    bool isAlive() const { return m_alive; }
	bool hasReachedEnd() const { return m_reachedEnd; }

    void takeDamage(int damage);
    void deactivate() { m_alive = false; }

    int getHP() const { return m_hp; }
    int getMaxHP() const { return m_maxHP; }
    int getX() const { return static_cast<int>(m_x); }
    int getY() const { return static_cast<int>(m_y); }
    float getSpeed() const { return m_speed; }

    void initPath(int map[20][25]);
    virtual void move(float deltaTime);
    virtual void display(SDL_Renderer* renderer) = 0;
    void renderHPBar(SDL_Renderer* renderer) const;

    virtual void reset(int map[20][25]) {
        m_alive = true;
        m_hp = m_maxHP;
        m_reachedEnd = false;
        initPath(map);
    }

protected:
    float m_x, m_y, m_speed;
    int m_hp, m_maxHP;
    bool m_alive;
    std::vector<PathFinder::Point> m_path;
    size_t m_currentPathIndex;
	bool m_reachedEnd = false;
    HealthBar* m_healthBar;
};
