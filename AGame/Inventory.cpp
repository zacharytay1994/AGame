#include "Inventory.h"

Inventory::Inventory() : equippped_weapon{ nullptr }
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

bool Inventory::Inventory_EquipWeapon(std::string const& name)
{
	std::map<std::string, Weapon*>::iterator it = inventory_weapon.find(name);
	if (it != inventory_weapon.end() && it->second->GetWeapon_Unlocked())
	{
		equippped_weapon = it->second;
		return true;
	}
	return false;
}

bool Inventory::Inventory_SetWeaponUnlocked(std::string const& name)
{
	std::map<std::string, Weapon*>::iterator it = inventory_weapon.find(name);
	if (it != inventory_weapon.end())
	{
		it->second->Weapon_Unlock();
		return true;
	}
	return false;
}

bool Inventory::Inventory_CheckWeaponUnlocked(std::string const& name) const
{
	std::map<std::string, Weapon*>::const_iterator it = inventory_weapon.find(name);
	if (it != inventory_weapon.end())
	{
		return it->second->GetWeapon_Unlocked();
	}
	return false;
}