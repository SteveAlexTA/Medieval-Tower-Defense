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
	static const int ARCHER_TOWER_COST = 100;
	static const int CANNON_TOWER_COST = 200;

	static const int ARCHER_UPGRADE_LVL2_COST = 200;
	static const int ARCHER_UPGRADE_LVL3_COST = 400;

	static const int CANNON_UPGRADE_LVL2_COST = 400;
	static const int CANNON_UPGRADE_LVL3_COST = 800;
	//Enemy reward
	static const int GOBLIN_REWARD = 20;
	static const int SKELETON_REWARD = 40;
	static const int DEMON_REWARD = 50;
	static const int DRAGON_REWARD = 100;
private:
	int money;
};