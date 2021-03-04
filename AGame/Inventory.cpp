#include "Inventory.h"

Inventory::Inventory() : equipped_weapon{ nullptr }
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
		equipped_weapon = it->second;
		return true;
	}
	return false;
}

const Weapon& Inventory::Inventory_GetCurrentWeapon() const
{
	return *equipped_weapon;
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

#if defined(DEBUG) | defined(_DEBUG)
void Inventory::Inventory_PrintCurrentWeapon() const
{
	if (equipped_weapon != nullptr)
	{
		std::cout << "**** PRINT WEAPON ****" << std::endl;
		std::cout << "* ID: "	<< std::setw(14) << std::left << equipped_weapon->GetWeapon_ID()		<< " *" << std::endl;
		std::cout << "* Name: " << std::setw(12) << std::left << equipped_weapon->GetWeapon_Name()	<< " *" << std::endl;
		std::cout << "* DMG: "	<< std::setw(13) << std::left << equipped_weapon->GetWeapon_Damage()	<< " *" << std::endl;
		std::cout << "* Reload: " << std::setw(10) << std::left << equipped_weapon->GetWeapon_ReloadTime() << " *" << std::endl;
		std::cout << "* Capacity: " << std::setw(8) << std::left << equipped_weapon->GetWeapon_Capacity() << " *" << std::endl;
		std::cout << "**********************" << std::endl;
	}
	else
	{
		std::cout << "**** NO WEPAON ****" << std::endl;
	}

}
#endif