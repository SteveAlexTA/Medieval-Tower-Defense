#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <iostream>
Enemy::Enemy(int x, int y, int hp, int speed) : m_x(x), m_y(y), m_hp(hp), m_speed(speed), m_alive(true), m_currentPathIndex(0) {};

Enemy::~Enemy() {}

bool Enemy::isDead() const {
	if (m_hp <= 0) return true;
	else return false;
}
void Enemy::takeDamage(int damage)  {
	m_hp -= damage;
	if (m_hp <= 0) {
		m_alive = false;
		m_hp = 0;
	}
}
int Enemy::getHP() const {
	return m_hp;
}
int Enemy::getSpeed() const {
	return m_speed;
}
int Enemy::getX() const {
	return m_x;
}
int Enemy::getY() const {
	return m_y;
}
void Enemy::initPath(int map[20][25]) {
    PathFinder::Point start, end;
    PathFinder::FindPathStartEnd(map, start, end);

    if (start.x == -1 || end.x == -1) {
        std::cerr << "ERROR: No valid start or end position found!" << std::endl;
        std::cerr << "Start: (" << start.x << ", " << start.y << ")" << std::endl;
        std::cerr << "End: (" << end.x << ", " << end.y << ")" << std::endl;
        std::cerr << "Map contents:" << std::endl;
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 25; x++) {
                std::cerr << map[y][x] << " ";
            }
            std::cerr << std::endl;
        }
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

    for (size_t i = 0; i < m_path.size(); ++i) {
        std::cout << "Path Point " << i << ": ("
            << m_path[i].x << ", " << m_path[i].y << ")" << std::endl;
    }
}

void Enemy::move() {
    if (m_path.empty() || m_currentPathIndex >= m_path.size()-1) {
        std::cout << "ERROR: Path is empty!" << std::endl;
        return;
    }

    PathFinder::Point current = m_path[m_currentPathIndex];
    PathFinder::Point next = m_path[m_currentPathIndex + 1];

    // Calculate target pixel coordinates
    int targetX = next.x * 32;
    int targetY = next.y * 32;

    // Simplified movement logic
    bool movedX = false, movedY = false;

    // Move X coordinate
    if (m_x < targetX) {
        m_x += m_speed;
        movedX = true;
    }
    else if (m_x > targetX) {
        m_x -= m_speed;
        movedX = true;
    }

    // Move Y coordinate
    if (m_y < targetY) {
        m_y += m_speed;
        movedY = true;
    }
    else if (m_y > targetY) {
        m_y -= m_speed;
        movedY = true;
    }

    // Check if reached next point
    if (std::abs(m_x - targetX) <= m_speed && std::abs(m_y - targetY) <= m_speed) {
        m_x = targetX;
        m_y = targetY;
        m_currentPathIndex++;
    }
}


