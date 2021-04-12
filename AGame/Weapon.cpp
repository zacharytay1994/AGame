/******************************************************************************/
/*!
\File Name		: Weapon.cpp
\Project Name	: AGame
\Authors 		:
				Primary - Noel Ho (100%)
				Secondary -
\brief		Functions for weapons

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/


#include "Weapon.h"
#include "CSHeaderDef.h"

/**************************************************************************/
	/*!
	  \brief
		Default Constructor
	*/
/**************************************************************************/
Weapon::Weapon()
	: weapon_ID(0),
	weapon_Damage(0),
	weapon_ReloadTime(0),
	weapon_curr_ReloadTimer(0),
	weapon_unlocked(false),
	weapon_Cost(50)
{

}

/**************************************************************************/
	/*!
	  \brief
		Copy constructor

	  \param rhs
		The weapon to copy
	*/
/**************************************************************************/
Weapon::Weapon(Weapon const& rhs)
	: weapon_ID(0),
	weapon_Damage(rhs.weapon_Damage),
	weapon_ReloadTime(rhs.weapon_ReloadTime),
	weapon_curr_ReloadTimer(0),
	weapon_unlocked(false),
	weapon_Cost(50)
{
	for (unsigned int i = 0; i < rhs.weapon_Pattern.size(); i++)
	{
		weapon_Pattern.push_back(rhs.weapon_Pattern[i]);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Constructor with preset ID

	  \param ID
		The ID to register on the weapon
	*/
/**************************************************************************/
Weapon::Weapon(unsigned int const& ID)
	: weapon_ID(ID),
	weapon_Damage(0),
	weapon_ReloadTime(0),
	weapon_curr_ReloadTimer(0),
	weapon_unlocked(false),
	weapon_Cost(50)
{

}

/**************************************************************************/
	/*!
	  \brief
		Destructor
	*/
/**************************************************************************/
Weapon::~Weapon()
{

}

/**************************************************************************/
	/*!
	  \brief
		Fires the weapon
	*/
/**************************************************************************/
void Weapon::Weapon_Shoot()
{
	if (static_cast<float>(AEGetTime(nullptr)) - Weapon_Curr_ReloadTimer() > GetWeapon_ReloadTime())
	{
		Weapon_Curr_ReloadTimer() = static_cast<int>(AEGetTime(nullptr));
	}
	else
	{
		return;
	}

	Factory::SpriteData data{ "bullet.png", 50.0f, 100.0f, 2, 2, 4, 0.1f };
	for (unsigned int i = 0; i < weapon_Pattern.size(); i++)
	{
		// Spawn bullet at weapon_Pattern[i] relative to player
	}
}

/**************************************************************************/
	/*!
	  \brief
		Fires the weapon, with information passed in

	  \param spawn
		The position to spawn from

	  \param direction
	    The direction the bullets are facing

	  \param tilemap
	    The tilemap to spawn on
	*/
/**************************************************************************/
void Weapon::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap)
{
	if (static_cast<float>(AEGetTime(nullptr)) - Weapon_Curr_ReloadTimer() > GetWeapon_ReloadTime())
	{
		Weapon_Curr_ReloadTimer() = static_cast<int>(AEGetTime(nullptr));
	}
	else
	{
		return;
	}

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
	ResourceManager::Instance().ShootingSound(1.f);
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's ID

	  \return
		The weapon's ID
	*/
/**************************************************************************/
const unsigned int	Weapon::GetWeapon_ID() const
{
	return weapon_ID;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's name

	  \return
		The weapon's name
	*/
/**************************************************************************/
const std::string&	Weapon::GetWeapon_Name() const
{
	return weapon_Name;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's description

	  \return
		The weapon's description
	*/
/**************************************************************************/
const std::string& Weapon::GetWeapon_Description() const
{
	return weapon_Description;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's damage

	  \return
		The weapon's damage
	*/
/**************************************************************************/
const unsigned int	Weapon::GetWeapon_Damage() const
{
	return weapon_Damage;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's reload time

	  \return
		The weapon's reload time
	*/
/**************************************************************************/
const unsigned int Weapon::GetWeapon_ReloadTime() const
{
	return weapon_ReloadTime;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's unlocked status

	  \return
		True if the weapon is unlocked, false if it is locked
	*/
/**************************************************************************/
const bool Weapon::GetWeapon_Unlocked() const
{
	return weapon_unlocked;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's cost

	  \return
		The weapon's cost
	*/
/**************************************************************************/
const unsigned int Weapon::GetWeapon_Cost() const
{
	return weapon_Cost;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's bullet pattern

	  \return
		The weapon's bullet pattern
	*/
/**************************************************************************/
const std::vector<BulletSpawn>& Weapon::GetWeapon_Pattern() const
{
	return weapon_Pattern;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the weapon's current reloading timer

	  \return
		The current reloading timer
	*/
/**************************************************************************/
int&	Weapon::Weapon_Curr_ReloadTimer()
{
	return weapon_curr_ReloadTimer;
}

/**************************************************************************/
	/*!
	  \brief
		Unlocks the weapon
	*/
/**************************************************************************/
void	Weapon::Weapon_Unlock()
{
	weapon_unlocked = true;
}

/**************************************************************************/
	/*!
	  \brief
		Sets the weapon's name

	  \param new_Name
		The new name to set the weapon
	*/
/**************************************************************************/
void Weapon::SetWeapon_Name(std::string const& new_Name)
{
	weapon_Name = new_Name;
}

/**************************************************************************/
	/*!
	  \brief
		Sets the weapon's description

	  \param new_Desc
		The new description to set the weapon
	*/
/**************************************************************************/
void Weapon::SetWeapon_Description(std::string const& new_Desc)
{
	weapon_Description = new_Desc;
}

/**************************************************************************/
	/*!
	  \brief
		Sets the weapon's damage

	  \param new_Damage
		The new damage to set the weapon
	*/
/**************************************************************************/
void Weapon::SetWeapon_Damage(unsigned int new_Damage)
{
	weapon_Damage = new_Damage;
}

/**************************************************************************/
	/*!
	  \brief
		Sets the weapon's reload time

	  \param new_ReloadTime
		The new reload time to set the weapon
	*/
/**************************************************************************/
void Weapon::SetWeapon_ReloadTime(int new_ReloadTime)
{
	weapon_ReloadTime = new_ReloadTime;
}

/**************************************************************************/
	/*!
	  \brief
		Sets the weapon's bullet pattern

	  \param rhs
		The new bullet pattern to set the weapon
	*/
/**************************************************************************/
void Weapon::SetWeapon_Pattern(std::vector<BulletSpawn> const& rhs)
{
	weapon_Pattern.clear();
	for (unsigned int i = 0; i < rhs.size(); i++)
	{
		weapon_Pattern.push_back(rhs[i]);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Sets the weapon's cost

	  \param new_Cost
		The new cost to set the weapon
	*/
/**************************************************************************/
void Weapon::SetWeapon_Cost(unsigned int new_Cost)
{
	weapon_Cost = new_Cost;
}

/**************************************************************************/
	/*!
	  \brief
		Constructor for NoWeapon, ID is 0
	*/
/**************************************************************************/
NoWeapon::NoWeapon() : Weapon(0)
{
	SetWeapon_Name("NoWeapon");
	Weapon_Unlock();
}

/**************************************************************************/
	/*!
	  \brief
		Constructor for NoWeapon, ID is 1
	*/
	/**************************************************************************/
Pistol::Pistol() : Weapon(1)
{
	SetWeapon_Name("Pistol");
	SetWeapon_Description("Shoots forward");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(0);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 0 });
	SetWeapon_Pattern(temp_Pattern);
	SetWeapon_Cost(50);
}

/**************************************************************************/
	/*!
	  \brief
		Constructor for NoWeapon, ID is 2
	*/
/**************************************************************************/
TrickPistol::TrickPistol() : Weapon(2)
{
	SetWeapon_Name("TrickPistol");
	SetWeapon_Description("Shoots forward & backward");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(1);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 0 });
	temp_Pattern.push_back({ -1, 0 });
	SetWeapon_Pattern(temp_Pattern);
	SetWeapon_Cost(50);
}

/**************************************************************************/
	/*!
	  \brief
		Fires the Trick Pistol weapon, with information passed in

	  \param spawn
		The position to spawn from

	  \param direction
		The direction the bullets are facing

	  \param tilemap
		The tilemap to spawn on
	*/
/**************************************************************************/
void TrickPistol::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap)
{
	if (static_cast<float>(AEGetTime(nullptr)) - Weapon_Curr_ReloadTimer() > GetWeapon_ReloadTime())
	{
		Weapon_Curr_ReloadTimer() = static_cast<int>(AEGetTime(nullptr));
	}
	else
	{
		return;
	}

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
	ResourceManager::Instance().ShootingSound(0.5f);
}

/**************************************************************************/
	/*!
	  \brief
		Constructor for DualPistol, ID is 3
	*/
/**************************************************************************/
DualPistol::DualPistol() : Weapon(3)
{
	SetWeapon_Name("DualPistol");
	SetWeapon_Description("Shoots 2 bullets forward");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(1);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 1 });
	temp_Pattern.push_back({ 1, -1 });
	SetWeapon_Pattern(temp_Pattern);
	SetWeapon_Cost(50);
}

/**************************************************************************/
	/*!
	  \brief
		Constructor for DualDiag, ID is 4
	*/
/**************************************************************************/
DualDiagPistol::DualDiagPistol() : Weapon(4)
{
	SetWeapon_Name("DualDiagPistol");
	SetWeapon_Description("Shoots 2 bullets diagonally");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(1);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 1 });
	temp_Pattern.push_back({ 1, -1 });
	SetWeapon_Pattern(temp_Pattern);
	SetWeapon_Cost(50);
}

/**************************************************************************/
	/*!
	  \brief
		Fires the Dual Diagonal Pistol weapon, with information passed in

	  \param spawn
		The position to spawn from

	  \param direction
		The direction the bullets are facing

	  \param tilemap
		The tilemap to spawn on
	*/
/**************************************************************************/
void DualDiagPistol::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap)
{
	if (static_cast<float>(AEGetTime(nullptr)) - Weapon_Curr_ReloadTimer() > GetWeapon_ReloadTime())
	{
		Weapon_Curr_ReloadTimer() = static_cast<int>(AEGetTime(nullptr));
	}
	else
	{
		return;
	}

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
	ResourceManager::Instance().ShootingSound(2.f);
}

/**************************************************************************/
	/*!
	  \brief
		Constructor for Dagger
	*/
/**************************************************************************/
Dagger::Dagger()
{
	SetWeapon_Name("Dagger");
	SetWeapon_Description("Short range attack");
	SetWeapon_Damage(1);
	SetWeapon_ReloadTime(0);
	std::vector<BulletSpawn> temp_Pattern;
	temp_Pattern.push_back({ 1, 1 });
	temp_Pattern.push_back({ 1, 0 });
	temp_Pattern.push_back({ 1, -1 });
	SetWeapon_Pattern(temp_Pattern);
	Weapon_Unlock();
}

/**************************************************************************/
	/*!
	  \brief
		Fires the Dagger weapon, with information passed in

	  \param spawn
		The position to spawn from

	  \param direction
		The direction the bullets are facing

	  \param tilemap
		The tilemap to spawn on
	*/
/**************************************************************************/
void Dagger::Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap)
{
	Factory::SpriteData data{ "slash.png", 50.0f, 100.0f, 2, 2, 4, 0.1f };

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