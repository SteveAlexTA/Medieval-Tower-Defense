#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
class Money {
public:
	Money(int startingMoney = 100);
	int getMoney() const { return money; }
	void addMoney(int amount);
	bool spendMoney(int amount);
	//Tower cost
	static const int TOWER_BASE_COST = 100;
	static const int TOWER_UPGRADE_ARCHER_COST = 300;
	static const int TOWER_UPGRADE_CANNON_COST = 700;
	//Enemy reward
	static const int GOBLIN_REWARD = 20;
	static const int SKELETON_REWARD = 40;
private:
	int money;
};