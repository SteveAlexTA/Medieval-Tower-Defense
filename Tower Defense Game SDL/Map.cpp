#include "Map.h"
#include "TextureManager.h"
#include "Game.h"
Map::Map() { //Constructor
    dirt = TextureManager::LoadTexture("Assets/Map/spr_grass_03.png", Game::renderer);
    grass01 = TextureManager::LoadTexture("Assets/Map/spr_grass_01.png", Game::renderer);
    grass02 = TextureManager::LoadTexture("Assets/Map/spr_grass_02.png", Game::renderer);
    LoadMap(lvl1);
    src.x = src.y = 0;
    src.w = dest.w = 32;
    src.h = dest.h = 32;
    dest.x = dest.y = 0;
}
Map::~Map() //Destructor 
{
    if (dirt) SDL_DestroyTexture(dirt);
    if (grass01) SDL_DestroyTexture(grass01);   
    if (grass02) SDL_DestroyTexture(grass02);
    for (auto& decoration : decorations) {
        if (decoration.texture) {
            SDL_DestroyTexture(decoration.texture);
        }
    }
}
void Map::LoadMap(int arr[20][25]) //Load map
{
    for (int row = 0; row < 20; row++) {
        for (int column = 0; column < 25; column++) {
            map[row][column] = arr[row][column];
        }
    }
}
void Map::AddDecoration(const char* texturePath, int row, int col) {
	if (row < 0 || row >= 20 || col < 0 || col >= 25) {
		std::cerr << "ERROR: Invalid decoration position (" << row << ", " << col << ")" << std::endl;
		return;
	}
    int tileType = map[row][col];
    if (tileType == 0 || tileType == 1 || tileType == 2) {
		SDL_Texture* decorTexture = TextureManager::LoadTexture(texturePath, Game::renderer);
        if (decorTexture) {
            Decoration decoration;
            decoration.texture = decorTexture;
			decoration.row = row;
			decoration.col = col;
            decorations.push_back(decoration);
        }
    }
}
void Map::DrawMap() { //Draw map
    int type = 0;
    for (int row = 0; row < 20; row++) {
        for (int column = 0; column < 25; column++) {
            type = map[row][column];
			dest.x = column * 32;
			dest.y = row * 32;
            switch (type)
            {
            case 0:
                TextureManager::Draw(grass02, src, dest);
                break;
            case 1:
				TextureManager::Draw(grass01, src, dest);
				break;
            case 2:
				TextureManager::Draw(dirt, src, dest);
                break;
            default:
                break;
            }
        }
    }
    for (const auto& decoration : decorations) {
		dest.x = decoration.col * 32;
		dest.y = decoration.row * 32;
		TextureManager::Draw(decoration.texture, src, dest);
    }
}
bool Map::IsEnemyPath(int row, int column) const
{
    if (row >= 0 && row < 20 && column >= 0 && column < 25) {
        return map[row][column] == 2;
    }
    return false; 
}