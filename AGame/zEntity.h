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
	std::shared_ptr<Chunk>	_chunk;
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
		int i = 0;
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

	template <typename...T>
	Entity& AddComponent() {
		// get mask
		std::bitset<64> mask = _chunk->_owning_archetype->_mask;
		int flag = 0;
		// if already has components in it
		((flag = flag ? flag : mask[component_description_v<T>._bit]), ...);
		if (flag) {
			return *this;
		}
		((mask[component_description_v<T>._bit] = 1), ...);
		// recreate archetype with new mask
		std::shared_ptr<Archetype> archetype;
		ArchetypeDatabase& adb = ArchetypeDatabase::Instance();
		if (adb._database.find(mask) != adb._database.end()) {
			archetype = adb._database[mask];
		}
		else {
			adb.CloneArchetype(mask, _chunk->_owning_archetype, archetype);
			((archetype->AddDescriptions<T>()), ...);
		}
		std::shared_ptr<Chunk> temp;
		int temp_id = archetype->Add(temp);
		// perform shallow copy from current chunk to temp
		memcpy(temp->GetDataBegin(temp_id), _chunk->GetDataBegin(_id), _chunk->_owning_archetype->_chunk_stride);
		// free data in old archetype/chunk
		FreeData();
		// assign new and initialize new component
		_chunk = temp;
		_id = temp_id;
		((_chunk->GetComponent<T>(_id) = T()), ...);
		std::cout << "Component Added!" << std::endl;
		return *this;
	}
	void FreeData();
};