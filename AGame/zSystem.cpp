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
			if ((system->_mask & archetype.second->_mask) == system->_mask) {
				// loop through
				for (auto& chunk : archetype.second->_chunk_database) {
					for (int i = 0; i < chunk->_number_of_entities; ++i) {
						system->UpdateComponent(dt, *(chunk.get()), i);
					}
				}
			}
		}
	}
}

//System::System()
//{
//}

//void System::BaseUpdate(const float& dt)
//{
//}
