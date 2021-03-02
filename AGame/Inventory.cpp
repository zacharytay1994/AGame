#include "Inventory.h"

Inventory::Inventory()
{
	inventory_weapon.insert(std::make_pair("Pistol", new Pistol()));
}

Inventory::~Inventory()
{
	std::map<std::string, Weapon*>::iterator it;
	for (it = inventory_weapon.begin(); it != inventory_weapon.end(); it++)
	{
		delete it->second;
	}
}