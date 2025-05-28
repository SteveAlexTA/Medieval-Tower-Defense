#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <iostream>
Enemy::Enemy(float x, float y, int hp, float speed)
    : m_x(x)
    , m_y(y)
    , m_hp(hp)
    , m_maxHP(hp)
    , m_speed(speed)
    , m_alive(true)
    , m_currentPathIndex(0)
    , m_reachedEnd(false) 
{};

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
    const int BAR_H = 3;
    const int BAR_OFFSET_Y = -20; 
    // Calculate health percentage and remaining health bar width
    float healthPercentage = static_cast<float>(m_hp) / m_maxHP; 
    int healthWidth = static_cast<int>(static_cast<float>(BAR_W * healthPercentage));
    // Draw current health bar (red)
    int barX = static_cast<int>(m_x - static_cast<float>(healthWidth / 2));
    int barY = static_cast<int>(m_y) + BAR_OFFSET_Y;
    SDL_Rect healthBar = { barX, barY, healthWidth, BAR_H };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &healthBar);
}

void Enemy::initPath(int map[20][25]) {
    PathFinder::Point start, end;
    PathFinder::FindPathStartEnd(map, start, end);
    if (start.x == -1 || end.x == -1) {
        return;
    }
    // Set initial position to start of path
    m_x = static_cast<float>(start.x * 32 + 16);
    m_y = static_cast<float>(start.y * 32 + 16);
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
    float targetX = static_cast<float>(next.x * 32 + 16);
    float targetY = static_cast<float>(next.y * 32 + 16);
    // Adjust speed based on delta time
	float dx = targetX - m_x;
	float dy = targetY - m_y;
	float dist = sqrt(dx * dx + dy * dy);
	if (dist < 2.0f) {
		m_currentPathIndex++;
        if (m_currentPathIndex >= m_path.size() - 1) {
            m_reachedEnd = true;
        }
        return;
	}
    float dirX = dx / dist;
    float dirY = dy / dist;
    float moveStep = m_speed * deltaTime;
    m_x += dirX * moveStep;
    m_y += dirY * moveStep;
}
