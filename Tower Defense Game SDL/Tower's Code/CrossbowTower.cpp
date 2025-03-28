#include "CrossbowTower.h"
#include "../TextureManager.h"	
#include<cmath>
CrossbowTower::CrossbowTower(int x, int y, SDL_Renderer* renderer): x(x), y(y), renderer(renderer)
{ //Tower Constructor
	texture = TextureManager::LoadTexture("Assets/Tower/spr_tower_crossbow.png", renderer);
	src = { 0,0,32,32 };
	dest = { x,y,32,32 };
}
CrossbowTower::~CrossbowTower() //Tower Destructor
{
	SDL_DestroyTexture(texture);
}
void CrossbowTower::Update()
{

}
void CrossbowTower::Render()
{
	TextureManager::Draw(texture, src, dest);
}