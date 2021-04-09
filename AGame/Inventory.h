#pragma once
#include "Weapon.h"
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <cstdio>

class Inventory
{
	public:
		Inventory();
		~Inventory();

		bool Inventory_EquipWeapon(std::string const& name);
		const Weapon& Inventory_GetCurrentWeapon() const;

		bool Inventory_SetWeaponUnlocked(std::string const& name);
		bool Inventory_CheckWeaponUnlocked(std::string const& name) const;

		void Inventory_PrintCurrentWeapon() const;

		int coins;
		size_t Inventory_GetSize() const;

	private:
		std::map<std::string, Weapon*> inventory_weapon;
		Weapon* equipped_weapon;
		Weapon* equipped_secondary_weapon;
};