#include "zSystem.h"
#include "zArchetype.h"
#include "zChunk.h"

SystemDatabase& SystemDatabase::Instance() {
	static SystemDatabase instance;
	return instance;
}

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

void SystemDatabase::RemoveAllEntities()
{
	for (auto& system : _database) {
		system.second->RemoveAllEntities();
	}
}

void System::RemoveAllEntities()
{
	for (auto tbr : _to_be_removed) {
		tbr._chunk->Remove(tbr._id);
	}
	_to_be_removed.clear();
}

void System::RemoveEntity()
{
	if (_current_chunk) {
		_to_be_removed.push_back({ _current_chunk,_current_id });
		//_current_chunk->Remove(_current_id);
	}
}