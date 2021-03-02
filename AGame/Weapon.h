#pragma once

#include <vector>

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
		virtual void Weapon_Reload();	// What happens when weapon is reloaded

		const unsigned int	GetWeapon_ID() const;
		const unsigned int	GetWeapon_Damage() const;
		const unsigned int	GetWeapon_ReloadTime() const;
		const unsigned int	GetWeapon_Capacity() const;

		int&	Weapon_Curr_ReloadTimer();
		int&	Weapon_Curr_Capacity();

	protected:
		void SetWeapon_Damage(unsigned int const& new_Damage);
		void SetWeapon_ReloadTime(unsigned int const& new_ReloadTime);
		void SetWeapon_Capacity(unsigned int const& new_Capacity);
		void SetWeapon_Pattern(std::vector<BulletSpawn> const& rhs);

	private:
		const unsigned int weapon_ID;
		unsigned int weapon_Damage;
		unsigned int weapon_ReloadTime;
		unsigned int weapon_Capacity;

		int weapon_curr_ReloadTimer;
		int weapon_curr_Capacity;
		std::vector<BulletSpawn> weapon_Pattern;
};

class Pistol : public Weapon
{
	public:
		Pistol();				// Default constructor
};