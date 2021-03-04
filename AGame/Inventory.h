#pragma once
#include "Weapon.h"
#include <map>
#include <string>

class Inventory
{
	public:
		Inventory();
		~Inventory();

		bool Inventory_EquipWeapon(std::string const& name);
		const Weapon& Inventory_GetCurrentWeapon() const;

		bool Inventory_SetWeaponUnlocked(std::string const& name);
		bool Inventory_CheckWeaponUnlocked(std::string const& name) const;

	private:
		std::map<std::string, Weapon*> inventory_weapon;
		Weapon* equippped_weapon;
};