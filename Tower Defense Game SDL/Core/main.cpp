#include <SDL.h>
#include "Game.h"

Game* game = nullptr;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
	int frameTime;

    game = new Game();
    game->init("TowerDefense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

    SDL_Surface* icon = SDL_LoadBMP("gameicon.bmp");
    if (icon) {
        SDL_SetWindowIcon(game->getWindow(), icon);
        SDL_FreeSurface(icon);
    }
    else {
        SDL_Log("Failed to load icon: %s\n", SDL_GetError());
    }

    while (game->running()) {
        frameStart = SDL_GetTicks();
        
        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
    }

    game->clean();
    delete game;  
    return 0;
}
