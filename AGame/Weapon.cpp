#include "Weapon.h"
#include "CSHeaderDef.h"

Weapon::Weapon()
	: weapon_ID(0),
	weapon_Damage(0),
	weapon_ReloadTime(0),
	weapon_Capacity(0),
	weapon_curr_ReloadTimer(0),
	weapon_curr_Capacity(0),
	weapon_unlocked(false),
	weapon_Cost(0)
{

}

Weapon::Weapon(Weapon const& rhs)
	: weapon_ID(0),
	weapon_Damage(rhs.weapon_Damage),
	weapon_ReloadTime(rhs.weapon_ReloadTime),
	weapon_Capacity(rhs.weapon_Capacity),
	weapon_curr_ReloadTimer(0),
	weapon_curr_Capacity(rhs.weapon_curr_Capacity),
	weapon_unlocked(false),
	weapon_Cost(0)
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
	Factory::SpriteData data{ "bullet.png", 50.0f, 100.0f, 2, 2, 4, 0.1f };
	for (unsigned int i = 0; i < weapon_Pattern.size(); i++)
	{
		// Spawn bullet at weapon_Pattern[i] relative to player
	}
}

void Weapon::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap) const
{
	Factory::SpriteData data{ "bullet.png", 50.0f, 100.0f, 2, 2, 4, 0.1f };
	for (unsigned int i = 0; i < weapon_Pattern.size(); i++)
	{
		// Spawn bullet at weapon_Pattern[i] relative to player
		if (direction.currdir == direction.right) {
			eid j = Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[i].x, spawn.y + weapon_Pattern[i].y, 1, 0, tilemap);
			Factory::Instance()[j].AddComponent<Com_YLayering>();
		}
		else if (direction.currdir == direction.left) {
			eid j = Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[i].x, spawn.y - weapon_Pattern[i].y, -1, 0, tilemap);
			Factory::Instance()[j].AddComponent<Com_YLayering>();
		}
		else if (direction.currdir == direction.up) {
			eid j = Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[i].y, spawn.y - weapon_Pattern[i].x, 0, 1, tilemap);
			Factory::Instance()[j].AddComponent<Com_YLayering>();
		}
		else if (direction.currdir == direction.down) {
			eid j = Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[i].y, spawn.y + weapon_Pattern[i].x, 0, -1, tilemap);
			Factory::Instance()[j].AddComponent<Com_YLayering>();
		}
	}
	ResourceManager::Instance().ShootingSound();
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

const std::string& Weapon::GetWeapon_Description() const
{
	return weapon_Description;
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

void Weapon::SetWeapon_Description(std::string const& new_Desc)
{
	weapon_Description = new_Desc;
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
	SetWeapon_Description("Shoots forward");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(2);
	SetWeapon_Capacity(6);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 0 });
	SetWeapon_Pattern(temp_Pattern);
}

TrickPistol::TrickPistol() : Weapon(2)
{
	SetWeapon_Name("TrickPistol");
	SetWeapon_Description("Shoots forward & backward");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(2);
	SetWeapon_Capacity(6);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 0 });
	temp_Pattern.push_back({ -1, 0 });
	SetWeapon_Pattern(temp_Pattern);
}

void TrickPistol::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap) const
{
	Factory::SpriteData data{ "bullet.png", 50.0f, 100.0f, 2, 2, 4, 0.1f };

	// Spawn bullet at weapon_Pattern[i] relative to player
	if (direction.currdir == direction.right)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[0].x, spawn.y + weapon_Pattern[0].y, 1, 0, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[1].x, spawn.y + weapon_Pattern[1].y, -1, 0, tilemap);
	}
	else if (direction.currdir == direction.left)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[0].x, spawn.y - weapon_Pattern[0].y, -1, 0, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[1].x, spawn.y - weapon_Pattern[1].y, 1, 0, tilemap);
	}
	else if (direction.currdir == direction.up)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[0].y, spawn.y - weapon_Pattern[0].x, 0, 1, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[1].y, spawn.y - weapon_Pattern[1].x, 0, -1, tilemap);
	}
	else if (direction.currdir == direction.down)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[0].y, spawn.y + weapon_Pattern[0].x, 0, -1, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[1].y, spawn.y + weapon_Pattern[1].x, 0, 1, tilemap);
	}
	ResourceManager::Instance().ShootingSound();
}

DualPistol::DualPistol() : Weapon(3)
{
	SetWeapon_Name("DualPistol");
	SetWeapon_Description("Shoots 2 bullets forward");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(2);
	SetWeapon_Capacity(6);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 1 });
	temp_Pattern.push_back({ 1, -1 });
	SetWeapon_Pattern(temp_Pattern);
}

DualDiagPistol::DualDiagPistol() : Weapon(4)
{
	SetWeapon_Name("DualDiagPistol");
	SetWeapon_Description("Shoots 2 bullets diagonally");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(2);
	SetWeapon_Capacity(6);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 1 });
	temp_Pattern.push_back({ 1, -1 });
	SetWeapon_Pattern(temp_Pattern);
}

void DualDiagPistol::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap) const
{
	Factory::SpriteData data{ "bullet.png", 50.0f, 100.0f, 2, 2, 4, 0.1f };

	// Spawn bullet at weapon_Pattern[i] relative to player
	if (direction.currdir == direction.right)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[0].x, spawn.y + weapon_Pattern[0].y, 1, -1, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[1].x, spawn.y + weapon_Pattern[1].y, 1, 1, tilemap);
	}
	else if (direction.currdir == direction.left)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[0].x, spawn.y - weapon_Pattern[0].y, -1, 1, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[1].x, spawn.y - weapon_Pattern[1].y, -1, -1, tilemap);
	}
	else if (direction.currdir == direction.up)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[0].y, spawn.y - weapon_Pattern[0].x, -1, 1, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[1].y, spawn.y - weapon_Pattern[1].x, 1, 1, tilemap);
	}
	else if (direction.currdir == direction.down)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[0].y, spawn.y + weapon_Pattern[0].x, 1, -1, tilemap);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[1].y, spawn.y + weapon_Pattern[1].x, -1, -1, tilemap);
	}
	ResourceManager::Instance().ShootingSound();
}

Dagger::Dagger()
{
	SetWeapon_Name("Dagger");
	SetWeapon_Description("Short range attack");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(2);
	SetWeapon_Capacity(6);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 1 });
	temp_Pattern.push_back({ 1, 0 });
	temp_Pattern.push_back({ 1, -1 });
	SetWeapon_Pattern(temp_Pattern);
}

void Dagger::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap) const
{
	Factory::SpriteData data{ "bullet.png", 50.0f, 100.0f, 2, 2, 4, 0.1f };

	// Spawn bullet at weapon_Pattern[i] relative to player
	if (direction.currdir == direction.right)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[0].x, spawn.y + weapon_Pattern[0].y, 1, -1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[1].x, spawn.y + weapon_Pattern[1].y, 1, -1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[2].x, spawn.y + weapon_Pattern[2].y, 1, -1, tilemap, 1);
	}
	else if (direction.currdir == direction.left)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[0].x, spawn.y - weapon_Pattern[0].y, -1, 1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[1].x, spawn.y - weapon_Pattern[1].y, -1, 1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[2].x, spawn.y - weapon_Pattern[2].y, -1, 1, tilemap, 1);
	}
	else if (direction.currdir == direction.up)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[0].y, spawn.y - weapon_Pattern[0].x, -1, 1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[1].y, spawn.y - weapon_Pattern[1].x, -1, 1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x - weapon_Pattern[2].y, spawn.y - weapon_Pattern[2].x, -1, 1, tilemap, 1);
	}
	else if (direction.currdir == direction.down)
	{
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[0].y, spawn.y + weapon_Pattern[0].x, 1, -1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[1].y, spawn.y + weapon_Pattern[1].x, 1, -1, tilemap, 1);
		Factory::Instance().FF_Createproj2(data, spawn.x + weapon_Pattern[2].y, spawn.y + weapon_Pattern[2].x, 1, -1, tilemap, 1);
	}
	ResourceManager::Instance().StabbingSound();
}