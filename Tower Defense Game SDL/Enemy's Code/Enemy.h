#pragma once
#include "../PathFinder.h"
#include <SDL.h>
#include <vector>
class Enemy
{
public:
	Enemy(int x, int y, int hp, int speed);
	virtual ~Enemy(); //Virtual for polymorphism with multiple enemy types
    bool isDead() const;
    void takeDamage(int damage);

    int getHP() const;
    int getX() const;
    int getY() const;
    int getSpeed() const;

    void initPath(int map[20][25]);
    virtual void move();
    virtual void display(SDL_Renderer* renderer) = 0;

protected:
    int m_hp, m_x, m_y, m_speed;
    bool m_alive;
    std::vector<PathFinder::Point> m_path;
    size_t m_currentPathIndex;
};
