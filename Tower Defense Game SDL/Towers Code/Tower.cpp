#include "Tower.h"
#include "../TextureManager.h"

Tower::Tower(int x, int y, SDL_Renderer* renderer, const char* texturePath): x(x), y(y), renderer(renderer) //Tower constructor
{
    texture = TextureManager::LoadTexture(texturePath, renderer);
    src = { 0,0,32,32 }; //Source rectangle = full texture
    dest = { x,y,32,32 }; //Destination rectangle = pos and size
}

Tower::~Tower() { //Tower destructor
    SDL_DestroyTexture(texture);
}

void Tower::Update() 
{

}

void Tower::Render() {
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
}