#include "Weapon.h"

Weapon::Weapon()
	: weapon_ID(0),
	weapon_Damage(0),
	weapon_ReloadTime(0),
	weapon_Capacity(0),
	weapon_curr_ReloadTimer(0),
	weapon_curr_Capacity(0),
	weapon_unlocked(false)
{

}

Weapon::Weapon(Weapon const& rhs)
	: weapon_ID(0),
	weapon_Damage(rhs.weapon_Damage),
	weapon_ReloadTime(rhs.weapon_ReloadTime),
	weapon_Capacity(rhs.weapon_Capacity),
	weapon_curr_ReloadTimer(0),
	weapon_curr_Capacity(rhs.weapon_curr_Capacity),
	weapon_unlocked(false)
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
	weapon_curr_Capacity(0),
	weapon_unlocked(false)
{

}

Weapon::~Weapon()
{

}

void Weapon::Weapon_Shoot()
{
	Factory::SpriteData data{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };
	for (unsigned int i = 0; i < weapon_Pattern.size(); i++)
	{
		// Spawn bullet at weapon_Pattern[i] relative to player
	}
}

void Weapon::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction) const
{
	Factory::SpriteData data{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };
	for (unsigned int i = 0; i < weapon_Pattern.size(); i++)
	{
		// Spawn bullet at weapon_Pattern[i] relative to player
		Factory::Instance().FF_Createproj(data, spawn.x, spawn.y, direction);
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

const std::string&	Weapon::GetWeapon_Name() const
{
	return weapon_Name;
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

const bool Weapon::GetWeapon_Unlocked() const
{
	return weapon_unlocked;
}

const std::vector<BulletSpawn>& Weapon::GetWeapon_Pattern() const
{
	return weapon_Pattern;
}

int&	Weapon::Weapon_Curr_ReloadTimer()
{
	return weapon_curr_ReloadTimer;
}

int&	Weapon::Weapon_Curr_Capacity()
{
	return weapon_curr_ReloadTimer;
}

void	Weapon::Weapon_Unlock()
{
	weapon_unlocked = true;
}

void Weapon::SetWeapon_Name(std::string const& new_Name)
{
	weapon_Name = new_Name;
}

void Weapon::SetWeapon_Damage(unsigned int new_Damage)
{
	weapon_Damage = new_Damage;
}

void Weapon::SetWeapon_ReloadTime(unsigned int new_ReloadTime)
{
	weapon_ReloadTime = new_ReloadTime;
}

void Weapon::SetWeapon_Capacity(unsigned int new_Capacity)
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

NoWeapon::NoWeapon() : Weapon(0)
{
	SetWeapon_Name("NoWeapon");
	Weapon_Unlock();
}

Pistol::Pistol() : Weapon(1)
{
	SetWeapon_Name("Pistol");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(2);
	SetWeapon_Capacity(6);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 0 });
	SetWeapon_Pattern(temp_Pattern);
}