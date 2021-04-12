/******************************************************************************/
/*!
\File Name		: zSystem.cpp
\Project Name	: AGame
\Authors 		:
				Primary - Zachary Tay (100%)
				Secondary -
\brief		System for ECS

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/
#include "zSystem.h"
#include "zArchetype.h"
#include "zChunk.h"

//system instance
SystemDatabase& SystemDatabase::Instance() {
	static SystemDatabase instance;
	return instance;
}
//system database update
void SystemDatabase::SystemDatabaseUpdate(const float& dt) {
	ArchetypeDatabase& adb = ArchetypeDatabase::Instance();
	for (auto& system : _database) {
		for (auto& archetype : adb._database) {
			// and the mask to see which archetypes are of interest
			if ((system.second->_mask & archetype.second->_mask) == system.second->_mask) {
				system.second->_dt = dt;
				system.second->OncePerFrame();
				// loop through
				for (auto& chunk : archetype.second->_chunk_database) {
					system.second->_current_chunk = chunk.get();
					for (int i = 0; i < (int)chunk->_number_of_entities; ++i) {
						if (system.second->_current_chunk->_active_flags[i]) {
							system.second->_current_id = i;
							system.second->UpdateComponent();
						}
					}
				}
			}
		}
	}
}
//remove all entities from database
void SystemDatabase::RemoveAllEntities()
{
	for (auto& system : _database) {
		system.second->RemoveAllEntities();
	}
}
//remove all entites from system
void System::RemoveAllEntities()
{
	for (auto tbr : _to_be_removed) {
		tbr._chunk->Remove(tbr._id);
	}
	_to_be_removed.clear();
}
//remove entity
void System::RemoveEntity()
{
	if (_current_chunk) {
		_to_be_removed.push_back({ _current_chunk,_current_id });
		//_current_chunk->Remove(_current_id);
	}
}