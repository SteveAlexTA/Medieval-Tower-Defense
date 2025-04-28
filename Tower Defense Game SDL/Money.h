#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
class Money {
public:
	Money(int startingMoney = 150);
	int getMoney() const { return money; }
	void addMoney(int amount);
	bool spendMoney(int amount);
	//Tower cost
	static const int ARCHER_TOWER_COST = 80;
	static const int CANNON_TOWER_COST = 150;
	static const int ARCHER_UPGRADE_LVL2_COST = 150;
	static const int ARCHER_UPGRADE_LVL3_COST = 250;
	static const int CANNON_UPGRADE_LVL2_COST = 250;
	static const int CANNON_UPGRADE_LVL3_COST = 400;
	//Enemy reward
	static const int GOBLIN_REWARD = 20;
	static const int SKELETON_REWARD = 40;
private:
	int money;
};