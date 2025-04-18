#pragma once
#include "../PathFinder.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
class Enemy {
public:
	Enemy(float x, float y, int hp, float speed);
	virtual ~Enemy(); //Virtual for polymorphism with multiple enemy types

    bool isDead() const { return m_hp <= 0; }
    bool isAlive() const { return m_alive; }
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
    virtual void renderHPBar(SDL_Renderer* renderer) const;

    virtual void reset(int map[20][25]) {
        m_alive = true;
        m_hp = m_maxHP;
        initPath(map);
    }
protected:
    float m_x, m_y, m_speed;
    int m_hp, m_maxHP;
    bool m_alive;
    std::vector<PathFinder::Point> m_path;
    size_t m_currentPathIndex;
};

class Goblin : public Enemy {
public:
	static const int GOBLIN_HP = 100;
	static const int GOBLIN_SPEED = 100;

    Goblin(float x, float y, SDL_Renderer* renderer, int map[20][25], SDL_Texture* texture);
    ~Goblin();

    void display(SDL_Renderer* renderer) override;
private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;

    int frameWidth = 12;   // Width of a single frame 
    int frameHeight = 8;   // Height of a single frame
    int totalFrames = 2;   // Number of frames in the animation
    int currentFrame = 0;  // Current frame index
    Uint32 lastFrameTime = 0; // Last time the frame was updated
    Uint32 frameDelay = 100;
};

class Skeleton : public Enemy {
public:
	static const int SKELETON_HP = 150;
	static const int SKELETON_SPEED = 150;
	Skeleton(float x, float y, SDL_Renderer* renderer, int map[20][25], SDL_Texture* texture);
	~Skeleton();
	void display(SDL_Renderer* renderer) override;
private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;

	int frameWidth = 8;   // Width of a single frame
	int frameHeight = 7;   // Height of a single frame
	int totalFrames = 4;   // Number of frames in the animation
	int currentFrame = 0;  // Current frame index
	Uint32 lastFrameTime = 0; // Last time the frame was updated
	Uint32 frameDelay = 100;
};