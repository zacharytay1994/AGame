/******************************************************************************/
/*!
\File Name		: Weapon.h
\Project Name	: AGame
\Authors 		:
				Primary - Noel Ho (100%)
				Secondary -
\brief		Functions for weapons

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/

#pragma once
#include <vector>
#include <string>
#include "Factory.h"

struct BulletSpawn
{
	int x;
	int y;
};

class Weapon
{
	public:
		Weapon();						// Default constructor
		Weapon(Weapon const&);			// Copy constructor
		Weapon(unsigned int const& ID);	// Non-default constructor
		~Weapon();						// Default destructor

		virtual void Weapon_Shoot();	// What happens when weapon is fired
		virtual void Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap);
		//getting weapon info
		const unsigned int	GetWeapon_ID() const;
		const std::string&	GetWeapon_Name() const;
		const std::string&  GetWeapon_Description() const;
		const unsigned int	GetWeapon_Damage() const;
		const unsigned int	GetWeapon_ReloadTime() const;
		const bool			GetWeapon_Unlocked() const;
		const unsigned int	GetWeapon_Cost() const;

		const std::vector<BulletSpawn>& GetWeapon_Pattern() const;


		int&	Weapon_Curr_ReloadTimer();
		void	Weapon_Unlock();

	protected:
		//set weapon
		void SetWeapon_Name(std::string const& new_Name);
		void SetWeapon_Description(std::string const& new_Desc);
		void SetWeapon_Damage(unsigned int new_Damage);
		void SetWeapon_ReloadTime(int new_ReloadTime);
		void SetWeapon_Pattern(std::vector<BulletSpawn> const& rhs);
		void SetWeapon_Cost(unsigned int new_Cost);

		std::vector<BulletSpawn> weapon_Pattern;

	private:
		//weapon info
		const unsigned int weapon_ID;
		std::string weapon_Name;
		std::string weapon_Description;
		unsigned int weapon_Cost;
		unsigned int weapon_Damage;
		unsigned int weapon_ReloadTime;

		int weapon_curr_ReloadTimer;
		bool weapon_unlocked;
};

class NoWeapon : public Weapon
{
	public:
		NoWeapon();				// Default constructor
};

class Pistol : public Weapon
{
	public:
		Pistol();				// Default constructor
};

class TrickPistol : public Weapon
{
	public:
		TrickPistol();// Default constructor
		virtual void Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap);
};

class DualPistol : public Weapon
{
	public:
		DualPistol(); // Default constructor
};

class DualDiagPistol : public Weapon
{
	public:
		DualDiagPistol();// Default constructor
		virtual void Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap);
};

class Dagger : public Weapon
{
	public:
		Dagger();// Default constructor
		virtual void Weapon_Shoot(BulletSpawn spawn, const Com_Direction& direction, eid const& tilemap);
};