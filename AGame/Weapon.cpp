#include "Weapon.h"

Weapon::Weapon() 
	: weapon_ID(0), 
	weapon_Damage(0), 
	weapon_ReloadTime(0), 
	weapon_Capacity(0), 
	weapon_curr_ReloadTimer(0),
	weapon_curr_Capacity(0)
{

}

Weapon::Weapon(Weapon const& rhs)
	: weapon_ID(0), 
	weapon_Damage(rhs.weapon_Damage), 
	weapon_ReloadTime(rhs.weapon_ReloadTime), 
	weapon_Capacity(rhs.weapon_Capacity), 
	weapon_curr_ReloadTimer(0),
	weapon_curr_Capacity(rhs.weapon_curr_Capacity)
{
	for (unsigned int i = 0; i < rhs.weapon_Pattern.size(); i++)
	{
		weapon_Pattern.push_back(rhs.weapon_Pattern[i]);
	}
}

Weapon::Weapon(unsigned int const& ID)
	: weapon_ID(ID),
	weapon_Damage(0),
	weapon_ReloadTime(0),
	weapon_Capacity(0),
	weapon_curr_ReloadTimer(0),
	weapon_curr_Capacity(0)
{

}

Weapon::~Weapon()
{

}

void Weapon::Weapon_Shoot()
{
	for (unsigned int i = 0; i < weapon_Pattern.size(); i++)
	{
		// Spawn bullet at weapon_Pattern[i] relative to player
	}
}

void Weapon::Weapon_Reload()
{
	weapon_curr_Capacity = weapon_Capacity;
}

const unsigned int	Weapon::GetWeapon_ID() const
{
	return weapon_ID;
}

const unsigned int	Weapon::GetWeapon_Damage() const
{
	return weapon_Damage;
}

const unsigned int	Weapon::GetWeapon_ReloadTime() const
{
	return weapon_ReloadTime;
}

const unsigned int	Weapon::GetWeapon_Capacity() const
{
	return weapon_Capacity;
}

int&	Weapon::Weapon_Curr_ReloadTimer()
{
	return weapon_curr_ReloadTimer;
}

int&	Weapon::Weapon_Curr_Capacity()
{
	return weapon_curr_ReloadTimer;
}

void Weapon::SetWeapon_Damage(unsigned int const& new_Damage)
{
	weapon_Damage = new_Damage;
}

void Weapon::SetWeapon_ReloadTime(unsigned int const& new_ReloadTime)
{
	weapon_ReloadTime = new_ReloadTime;
}

void Weapon::SetWeapon_Capacity(unsigned int const& new_Capacity)
{
	weapon_Capacity = new_Capacity;
}

void Weapon::SetWeapon_Pattern(std::vector<BulletSpawn> const& rhs)
{
	weapon_Pattern.clear();
	for (unsigned int i = 0; i < rhs.size(); i++)
	{
		weapon_Pattern.push_back(rhs[i]);
	}
}

Pistol::Pistol() : Weapon(1)
{

}