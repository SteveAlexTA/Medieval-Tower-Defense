#pragma once
#include <SDL.h>
class Game;
class Tower;

class InputSystem {
public:
	InputSystem(Game* game);
	~InputSystem();
	void handleEvents();
private:
	Game* m_game;
	void handleMouseMotion(const SDL_Event& event);
	void handleMouseButtonDown(const SDL_Event& event);
	void handleMouseButtonUp(const SDL_Event& event);
	void handleKeyDown(const SDL_Event& event);
	void handleMenuInput(const SDL_Event& event);
	void handleGameInput(const SDL_Event& event);
	void handleTowerPlacement(int mouseX, int mouseY);
	void handleTowerSelection(int mouseX, int mouseY);
	bool handleUIClick(int mouseX, int mouseY);
	bool isClickInUpgradeUI(int mouseX, int mouseY, Tower* tower) const;
	bool isClickInSellUI(int mouseX, int mouseY, Tower* tower) const;
};