#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
class Money {
public:
	Money(int startingMoney = 200);
	int getMoney() const { return money; }
	void addMoney(int amount);
	bool spendMoney(int amount);
	//Tower cost
	static const int ARCHER_TOWER_COST = 100;
	static const int CANNON_TOWER_COST = 250;
	static const int LIGHTNING_TOWER_COST = 1000;
	static const int SHOOTER_TOWER_COST = 500;

	static const int ARCHER_UPGRADE_LVL2_COST = 150;
	static const int ARCHER_UPGRADE_LVL3_COST = 300;

	static const int CANNON_UPGRADE_LVL2_COST = 500;
	static const int CANNON_UPGRADE_LVL3_COST = 1000;

	static const int LIGHTNING_UPGRADE_LVL2_COST = 2500;
	static const int LIGHTNING_UPGRADE_LVL3_COST = 5000;

	static const int SHOOTER_UPGRADE_LVL2_COST = 1000;
	static const int SHOOTER_UPGRADE_LVL3_COST = 3000;

	//Enemy reward
	static const int GOBLIN_REWARD = 20;
	static const int SKELETON_REWARD = 30;
	static const int DEMON_REWARD = 50;
	static const int DRAGON_REWARD = 100;
private:
	int money;
};