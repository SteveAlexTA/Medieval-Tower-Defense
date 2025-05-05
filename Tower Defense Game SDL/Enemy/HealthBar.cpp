#include "HealthBar.h"

HealthBar::HealthBar()
    : m_barWidth(30), m_barHeight(5), m_x(0), m_y(0), m_proportion(1.0f)
{
}

HealthBar::~HealthBar()
{
}

void HealthBar::update(int x, int y, float proportion)
{
    m_x = x;
    m_y = y;
    m_proportion = proportion;

    if (m_proportion < 0.0f) m_proportion = 0.0f;
    if (m_proportion > 1.0f) m_proportion = 1.0f;
}

void HealthBar::render(SDL_Renderer* renderer)
{
    if (!renderer) return;

    int barX = m_x - (m_barWidth / 2);
    int barY = m_y;
    int healthWidth = static_cast<int>(m_barWidth * m_proportion);

    SDL_Rect healthBar = { barX, barY, healthWidth, m_barHeight };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderFillRect(renderer, &healthBar);
}