/******************************************************************************/
/*!
\File Name		: Inventory.cpp
\Project Name	: AGame
\Authors 		:
				Primary - Noel Ho (100%)
				Secondary -
\brief		Functions for inventory system and weapons

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/
#include "Inventory.h"

/**************************************************************************/
	/*!
	  \brief
		Default constructor
	*/
/**************************************************************************/
Inventory::Inventory() : equipped_weapon{ nullptr }
{
	inventory_weapon.insert(std::make_pair("NoWeapon", new NoWeapon()));
	inventory_weapon.insert(std::make_pair("Pistol", new Pistol()));
	inventory_weapon.insert(std::make_pair("TrickPistol", new TrickPistol()));
	inventory_weapon.insert(std::make_pair("DualPistol", new DualPistol()));
	inventory_weapon.insert(std::make_pair("DualDiagPistol", new DualDiagPistol()));
	inventory_weapon.insert(std::make_pair("Dagger", new Dagger()));

	Inventory_EquipWeapon("Dagger");
	Inventory_EquipSecondaryWeapon("Dagger");
	coins = 100; // cannot be more than 1000000, will cause memory leaks
}

/**************************************************************************/
	/*!
	  \brief
		Destructor
	*/
/**************************************************************************/
Inventory::~Inventory()
{
	std::map<std::string, Weapon*>::iterator it;
	for (it = inventory_weapon.begin(); it != inventory_weapon.end(); it++)
	{
		delete it->second;
	}
}

/**************************************************************************/
	/*!
	  \brief
		Equips a weapon, if it is unlocked

	  \param name
	    The name of the weapon to equip

	  \return
	    True if weapon is successfully equipped
	*/
/**************************************************************************/
bool Inventory::Inventory_EquipWeapon(std::string const& name)
{
	std::map<std::string, Weapon*>::iterator it = inventory_weapon.find(name);
	if (it != inventory_weapon.end() && it->second->GetWeapon_Unlocked())
	{
		equipped_weapon = it->second;
		return true;
	}
	return false;
}

/**************************************************************************/
	/*!
	  \brief
		Equips a secondary weapon, if it is unlocked

	  \param name
		The name of the weapon to equip

	  \return
		True if weapon is successfully equipped
	*/
/**************************************************************************/
bool Inventory::Inventory_EquipSecondaryWeapon(std::string const& name)
{
	std::map<std::string, Weapon*>::iterator it = inventory_weapon.find(name);
	if (it != inventory_weapon.end() && it->second->GetWeapon_Unlocked())
	{
		equipped_secondary_weapon = it->second;
		return true;
	}
	return false;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the currently equipped weapon

	  \return
		The weapon class
	*/
/**************************************************************************/
Weapon& Inventory::Inventory_GetCurrentWeapon()
{
	return *equipped_weapon;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the currently equipped secondary weapon

	  \return
		The weapon class
	*/
/**************************************************************************/
Weapon& Inventory::Inventory_GetCurrentSecondaryWeapon()
{
	return *equipped_secondary_weapon;
}

/**************************************************************************/
	/*!
	  \brief
		Unlocks a weapon, if the player has enough coins

	  \param name
	    The name of the weapon

	  \return
		Whether the weapon was successfully unlocked
	*/
/**************************************************************************/
bool Inventory::Inventory_SetWeaponUnlocked(std::string const& name)
{
	std::map<std::string, Weapon*>::iterator it = inventory_weapon.find(name);
	if (it != inventory_weapon.end())
	{
		if (it->second->GetWeapon_Unlocked())
		{
			return true;
		}

		unsigned int weapon_cost = it->second->GetWeapon_Cost();
		if (coins >= (int)weapon_cost)
		{
			Inventory_AddCoins(-static_cast<int>(weapon_cost));
			it->second->Weapon_Unlock();
			return true;
		}
	}
	return false;
}

/**************************************************************************/
	/*!
	  \brief
		Checks if a weapon is unlocked

	  \param name
		The name of the weapon

	  \return
		True if the weapon is unlocked, false if locked
	*/
/**************************************************************************/
bool Inventory::Inventory_CheckWeaponUnlocked(std::string const& name) const
{
	std::map<std::string, Weapon*>::const_iterator it = inventory_weapon.find(name);
	if (it != inventory_weapon.end())
	{
		return it->second->GetWeapon_Unlocked();
	}
	return false;
}

/**************************************************************************/
	/*!
	  \brief
		Prints the current weapon details to console
	*/
/**************************************************************************/
void Inventory::Inventory_PrintCurrentWeapon() const
{
	if (equipped_weapon != nullptr)
	{
		std::cout << "**** PRINT WEAPON ****" << std::endl;
		std::cout << "* ID: "	<< std::setw(14) << std::left << equipped_weapon->GetWeapon_ID()		<< " *" << std::endl;
		std::cout << "* Name: " << std::setw(12) << std::left << equipped_weapon->GetWeapon_Name()	<< " *" << std::endl;
		std::cout << "* DMG: "	<< std::setw(13) << std::left << equipped_weapon->GetWeapon_Damage()	<< " *" << std::endl;
		std::cout << "* Reload: " << std::setw(10) << std::left << equipped_weapon->GetWeapon_ReloadTime() << " *" << std::endl;
		int** printMap;
		unsigned int height = 3, width = 3;
		int maxheight = 1, minheight = -1, maxwidth = 1, minwidth = -1, currheight = 0, currwidth = 0;
		for (int i = 0; i < equipped_weapon->GetWeapon_Pattern().size(); i++)
		{
			currwidth = equipped_weapon->GetWeapon_Pattern()[i].x;
			currheight = equipped_weapon->GetWeapon_Pattern()[i].y;
			minheight	= (currheight	< minheight)	? currheight : minheight;
			minwidth	= (currwidth	< minwidth)		? currwidth : minwidth;
			maxheight	= (currheight	> maxheight)	? currheight : maxheight;
			maxwidth	= (currwidth	> maxwidth)		? currwidth : maxwidth;
		}
		height = maxheight - minheight + 1;
		width = maxwidth - minwidth + 1;

		printMap = new int* [height];
		for (unsigned int i = 0; i < height; i++)
		{
			printMap[i] = new int[width];
			for (unsigned int j = 0; j < width; j++)
			{
				printMap[i][j] = 0;
			}
		}

		for (int i = 0; i < equipped_weapon->GetWeapon_Pattern().size(); i++)
		{
			currwidth = equipped_weapon->GetWeapon_Pattern()[i].x - minwidth;
			currheight = equipped_weapon->GetWeapon_Pattern()[i].y - minheight;
			printMap[currheight][currwidth] = 1;
		}
		printMap[0 - minheight][0 - minwidth] = 9;
		for (unsigned int i = 0; i < height; i++)
		{
			if (i == (unsigned int)(maxheight * 0.5f))
			{
				std::cout << "* Pattern: ";
			}
			else
			{
				std::cout  << "*          ";
			}

			for (unsigned int j = 0; j < width; j++)
			{
				std::cout << printMap[i][j] << " ";
			}
			if (width < 7)
			{
				for (unsigned int k = 0; k < 7 - width; k++)
				{
					std::cout << " ";
				}
			}
			std::cout << "*" << std::endl;
		}
		for (unsigned int i = 0; i < width; i++)
		{
			delete[] printMap[i];
		}
		delete[] printMap;
		std::cout << "**********************" << std::endl;
	}
	else
	{
		std::cout << "**** NO WEPAON ****" << std::endl;
	}
}

/**************************************************************************/
	/*!
	  \brief
		Adds coins to inventory (max 1,000,000)

	  \param newcoins
		The number of coins to add (can be negative)

	  \return
		The current total amount of coins the player has
	*/
/**************************************************************************/
int Inventory::Inventory_AddCoins(int newcoins)
{
	coins += newcoins;
	if (coins >= 1000000) coins = 999999;	// cannot be more than 1000000, will cause memory leaks
	if (coins < 0) coins = 0;
	return coins;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the size of the inventory

	  \return
		Size of inventory
	*/
/**************************************************************************/
size_t Inventory::Inventory_GetSize() const
{
	return inventory_weapon.size();
}

//#endif