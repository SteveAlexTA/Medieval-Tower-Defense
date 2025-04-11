#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <iostream>
Enemy::Enemy(float x, float y, int hp, float speed) : m_x(x), m_y(y), m_hp(hp), m_maxHP(hp), m_speed(speed), m_alive(true), m_currentPathIndex(0) {};

Enemy::~Enemy() {}

void Enemy::takeDamage(int damage)  {
	m_hp -= damage;
	if (m_hp <= 0) {
		m_alive = false;
		m_hp = 0;
	}
}
void Enemy::renderHPBar(SDL_Renderer* renderer) const {
    if (!m_alive) return;

    // Set up health bar dimensions
    const int BAR_W = 30;
    const int BAR_H = 5;
    const int BAR_OFFSET_Y = -20; // Display bar above the enemy

    // Calculate health percentage and remaining health bar width
    float healthPercentage = static_cast<float>(m_hp) / m_maxHP; 
    int healthWidth = static_cast<int>(BAR_W * healthPercentage);

    // Draw current health bar (green)
    int barX = m_x - (healthWidth / 2);
    int barY = m_y + BAR_OFFSET_Y;

    SDL_Rect healthBar = { barX, barY, healthWidth, BAR_H };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green only
    SDL_RenderFillRect(renderer, &healthBar);
}

void Enemy::initPath(int map[20][25]) {
    PathFinder::Point start, end;
    PathFinder::FindPathStartEnd(map, start, end);

    if (start.x == -1 || end.x == -1) {
        return;
    }
    // Set initial position to start of path
    m_x = start.x * 32;
    m_y = start.y * 32;
    // Find the path
    m_path = PathFinder::FindPath(map);

    if (m_path.empty()) {
        std::cerr << "ERROR: Path finding failed. No path generated!" << std::endl;
        return;
    }
    m_currentPathIndex = 0;
}

void Enemy::move(float deltaTime) {
    if (m_path.empty() || m_currentPathIndex >= m_path.size()-1) {
        return;
    }

    PathFinder::Point current = m_path[m_currentPathIndex];
    PathFinder::Point next = m_path[m_currentPathIndex + 1];

    // Calculate target pixel coordinates
    float targetX = next.x * 32 + 16;
    float targetY = next.y * 32 + 16;
    // Adjust speed based on delta time
	float dx = targetX - m_x;
	float dy = targetY - m_y;

	float dist = sqrt(dx * dx + dy * dy);
	if (dist < 2.0f) {
		m_currentPathIndex++;
        return;
	}
    float dirX = dx / dist;
    float dirY = dy / dist;
    float moveStep = m_speed * deltaTime;
    m_x += dirX * moveStep;
    m_y += dirY * moveStep;
}

Goblin::Goblin(float x, float y, SDL_Renderer* renderer, int map[20][25], SDL_Texture* texture) : Enemy(x, y, GOBLIN_HP, GOBLIN_SPEED), m_renderer(renderer), m_texture(texture) {
    m_maxHP = GOBLIN_HP;
    initPath(map);
}

Goblin::~Goblin() {}

void Goblin::display(SDL_Renderer* renderer) {
    if (!renderer || !m_texture || !m_alive) return;

    // Update animation frame
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % totalFrames;
        lastFrameTime = currentTime;
    }

    SDL_Rect src = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
    SDL_Rect dest = { static_cast<int>(m_x - 16), static_cast<int>(m_y - 16), 32, 32 }; 
    SDL_RenderCopy(renderer, m_texture, &src, &dest);
    renderHPBar(renderer);
}

Skeleton::Skeleton(float x, float y, SDL_Renderer* renderer, int map[20][25], SDL_Texture* texture) : Enemy(x, y, SKELETON_HP, SKELETON_SPEED), m_renderer(renderer), m_texture(texture) {
    m_maxHP = SKELETON_HP;
	initPath(map);
}

Skeleton::~Skeleton() {}

void Skeleton::display(SDL_Renderer* renderer) {
    if (!renderer || !m_texture || !m_alive) return;

    // Update animation frame
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % totalFrames;
        lastFrameTime = currentTime;
    }

    SDL_Rect src = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
    // Centers the sprite on the path point and scales to 32x32
    int destX = static_cast<int>(m_x - 16);
    int destY = static_cast<int>(m_y - 16);

    SDL_Rect dest = { destX, destY, 32, 32 }; 
    SDL_RenderCopy(renderer, m_texture, &src, &dest);
    renderHPBar(renderer);
}