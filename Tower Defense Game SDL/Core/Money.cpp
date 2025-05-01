#include "Money.h"
#include <iostream>

Money::Money(int startingMoney) : money(startingMoney) {
	std::cout << "Starting money: " << money << " Emeralds" << std::endl;
}
void Money::addMoney(int amount) {
	if (amount <= 0) return;
	money += amount;
	std::cout << "Added " << amount << " Emeralds. Total: " << money << " Emeralds" << std::endl;
}
bool Money::spendMoney(int spent) {
	if (spent <= 0 || spent > money) return false;
	if (money >= spent) {
		money -= spent;
		std::cout << "Spent " << spent << " Emeralds. Remaining: " << money << " Emeralds" << std::endl;
		return true;
	}
	std::cout << "Not enough Emeralds! " << spent << ". Remaining: " << money << " Emeralds" << std::endl;
	return false;
}