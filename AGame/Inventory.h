#pragma once
/******************************************************************************/
/*!
\file		Inventory.h
\author 	Noel Ho Sing Nam
\par    	email: s.ho\@digipen.edu
\date   	April 12, 2021
\brief		Functions for inventory

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

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
		bool Inventory_EquipSecondaryWeapon(std::string const& name);
		Weapon& Inventory_GetCurrentWeapon();
		Weapon& Inventory_GetCurrentSecondaryWeapon();

		bool Inventory_SetWeaponUnlocked(std::string const& name);
		bool Inventory_CheckWeaponUnlocked(std::string const& name) const;

		void Inventory_PrintCurrentWeapon() const;

		int coins;
		int Inventory_AddCoins(int newcoins);

		size_t Inventory_GetSize() const;

	private:
		std::map<std::string, Weapon*> inventory_weapon;
		Weapon* equipped_weapon;
		Weapon* equipped_secondary_weapon;
};