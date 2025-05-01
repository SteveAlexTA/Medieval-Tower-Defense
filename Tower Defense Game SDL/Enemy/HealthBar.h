#pragma once
#include <SDL.h>
#include <SDL_image.h>

class HealthBar {
public:
	HealthBar();
	~HealthBar();

	void update(int x, int y, float proportion);
	void render(SDL_Renderer* renderer);

private:
	int m_barWidth;
	int m_barHeight;
	int m_x;
	int m_y;
	float m_proportion;
};

