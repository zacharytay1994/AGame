#pragma once

#include <vector>
#include <string>

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
		const std::string&	GetWeapon_Name() const;
		const unsigned int	GetWeapon_Damage() const;
		const unsigned int	GetWeapon_ReloadTime() const;
		const unsigned int	GetWeapon_Capacity() const;
		const bool			GetWeapon_Unlocked() const;
		#if defined(DEBUG) | defined(_DEBUG)
			const std::vector<BulletSpawn>& GetWeapon_Pattern() const;
		#endif

		int&	Weapon_Curr_ReloadTimer();
		int&	Weapon_Curr_Capacity();
		void	Weapon_Unlock();

	protected:
		void SetWeapon_Name(std::string const& new_Name);
		void SetWeapon_Damage(unsigned int new_Damage);
		void SetWeapon_ReloadTime(unsigned int new_ReloadTime);
		void SetWeapon_Capacity(unsigned int new_Capacity);
		void SetWeapon_Pattern(std::vector<BulletSpawn> const& rhs);

	private:
		const unsigned int weapon_ID;
		std::string weapon_Name;
		unsigned int weapon_Damage;
		unsigned int weapon_ReloadTime;
		unsigned int weapon_Capacity;

		int weapon_curr_ReloadTimer;
		int weapon_curr_Capacity;
		bool weapon_unlocked;

		std::vector<BulletSpawn> weapon_Pattern;
};

class Pistol : public Weapon
{
	public:
		Pistol();				// Default constructor
};