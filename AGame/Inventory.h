#pragma once
#include "Weapon.h"
#include <map>
#include <string>

class Inventory
{
	public:
		Inventory();
		~Inventory();

	private:
		std::map<std::string, Weapon*> inventory_weapon;
};