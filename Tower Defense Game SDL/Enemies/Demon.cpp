#include "Demon.h"

Demon::Demon(float x, float y, SDL_Renderer* renderer, int map[20][25], SDL_Texture* texture)
	: Enemy(x, y, 0, 0), m_renderer(renderer), m_texture(texture)
{
	m_hp = 800;
	m_speed = 80;
	m_maxHP = m_hp;
	initPath(map);
}

Demon::~Demon()
{
}

void Demon::display(SDL_Renderer* renderer)
{
	if (!renderer || !m_texture || !m_alive) return;
	
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime > lastFrameTime + frameDelay) {
		currentFrame = (currentFrame + 1) % totalFrames;
		lastFrameTime = currentTime;
	}

	// Draw the demon sprite
	SDL_Rect src = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
	SDL_Rect dest = { static_cast<int>(m_x - 16), static_cast<int>(m_y - 16), 32, 32 };
	SDL_RenderCopy(renderer, m_texture, &src, &dest);
	renderHPBar(renderer);
}
