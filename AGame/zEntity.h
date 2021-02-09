#pragma once
#include "zArchetype.h"
#include "zChunk.h"

/*___________________________________________________________________________________
* Brief:	Entity is a handler to the underlying component/data stored in
*			the ArchetypeDatabase. Entity instances are currently stored in
*			Factory.h/cpp
*
* Used:		Entity instances are not meant to be defined as is. Look at Factory.h
*			Factory::CreateEntity(), Factory::FF_CreateBasicSprite() as examples
*			of how to instantiate an entity.
* 
* Relation:	Factory
*			- std::vector<Entity> _entities;
____________________________________________________________________________________*/
struct Entity {
	std::shared_ptr<Chunk>	_chunk{ nullptr };
	int						_id{ 0 };
	Entity() {}
	/*______________________________________________________
	* Brief:	Initializes the entity and creates/fetches a unique
	*			Archetype generated with its component combination.
	*			A chunk of that archetype will be returned 
	*
	* Access:	public (called by factory, might make private and
	*			friend in future)
	*
	* Use e.g:	Initialize<Com_Position,Com_Velocity>();
	*			- Builds new archetype based on pos & vel.
	________________________________________________________*/
	template <typename...T_COMPONENTS>
	void Initialize() {
		std::shared_ptr<Archetype> archetype = ArchetypeDatabase::Instance().CreateArchetype<T_COMPONENTS ...>();
		_id = archetype->Add(_chunk);
		((_chunk->GetComponent<T_COMPONENTS>(_id) = T_COMPONENTS()), ...);
	}
	/*______________________________________________________
	* Brief:	Gets a component that an entity has. There
	*			is no underlying check, up to user to validate.
	*			Will UDB if getting non-existing component.
	* 
	* Access:	public
	*
	* Use e.g:	Entity& e1 = Factory::CreateEntity<Com_Position>();
	*			int x = e1.Get<Com_Position>().x;
	*
	* \return	Reference the gotten type.
	________________________________________________________*/
	template <typename T>
	T& Get() {
		return _chunk->GetComponent<T>(_id);
	}
	/*______________________________________________________
	* Brief:	Destroy this instance.
	*
	* Access:	public
	*
	* Use e.g:	Entity& e1 = Factory::CreateEntity<Com_Position>();
	*			e1.Destroy();
	________________________________________________________*/
	void Destroy();
};