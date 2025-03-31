#include "TextureManager.h"
#include "Game.h"
#include <SDL_image.h>
SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* rend) {
	SDL_Surface* tempSurface = IMG_Load(texture);
	if (!tempSurface) 
	{
		std::cout << "Failed to load image! " << texture << IMG_GetError() << std::endl;
		return nullptr;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}
void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) {
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}