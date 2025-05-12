#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class LoseScreen {
public:
	LoseScreen(SDL_Renderer* renderer);
	~LoseScreen();

	bool init();
	void render(SDL_Renderer* renderer);
	void resetDisplayTime();
	bool isDisplayTimeElapsed() const;
	Uint32 getDisplayStartTime() const { return displayStartTime; }
private:
	SDL_Renderer* renderer;
	SDL_Texture* titleText;
	SDL_Texture* backgroundTexture;
	TTF_Font* font;
	SDL_Rect titleRect;
	Uint32 displayStartTime;
	const Uint32 DISPLAY_DURATION = 3000;
};