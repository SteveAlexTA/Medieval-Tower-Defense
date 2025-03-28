#include "Goblin.h"
#include <SDL_image.h>
#include <iostream>

Goblin::Goblin(int x, int y, SDL_Renderer* renderer, int map[20][25]) : Enemy(x, y, GOBLIN_HP, GOBLIN_SPEED), m_renderer(renderer), m_texture(nullptr) 
{
	m_texture = LoadTexture("Assets/Enemy/spr_goblin.png");
    initPath(map);
}
SDL_Texture* Goblin::LoadTexture(const std::string& filename)
{
    if (!m_renderer) {
        std::cerr << "Renderer is null!" << std::endl;
        return nullptr;
    }
    SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image! " << filename << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
    if (!newTexture) {
        std::cerr << "Unable to create texture from " << filename << "! SDL Error: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void Goblin::display(SDL_Renderer* renderer) 
{
    if (!renderer || !m_texture) return;
    SDL_Rect dest = { m_x, m_y, 32,32 };
    SDL_RenderCopy(renderer, m_texture, NULL, &dest);
}

Goblin::~Goblin()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}
