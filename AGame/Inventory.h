#pragma once
#include "Weapon.h"
#include <map>
#include <string>

#if defined(DEBUG) | defined(_DEBUG)
#include <iostream>
#include <iomanip>
#endif

class Inventory
{
	public:
		Inventory();
		~Inventory();

		bool Inventory_EquipWeapon(std::string const& name);
		const Weapon& Inventory_GetCurrentWeapon() const;

		bool Inventory_SetWeaponUnlocked(std::string const& name);
		bool Inventory_CheckWeaponUnlocked(std::string const& name) const;

		#if defined(DEBUG) | defined(_DEBUG)
			void Inventory_PrintCurrentWeapon() const;
		#endif

	private:
		std::map<std::string, Weapon*> inventory_weapon;
		Weapon* equipped_weapon;
};