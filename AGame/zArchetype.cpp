#include <iostream>
#include <stack>
#include "zArchetype.h"
#include "zChunk.h"
#include "CSHeaderDef.h"

int Archetype::Add(std::shared_ptr<Chunk>& chunk) {
	// check if there are any empty chunks
	for (auto& i : _chunk_database) {
		if (i->_number_of_entities < _entities_per_chunk) {
			chunk = i;
			return i->Add();
		}
	}
	// add new chunk
	_chunk_database.emplace_back(std::make_shared<Chunk>(this, _entities_per_chunk));
	chunk = _chunk_database.back();
	return _chunk_database.back()->Add();
}

ArchetypeDatabase& ArchetypeDatabase::Instance() {
	static ArchetypeDatabase instance;
	return instance;
}

bool ArchetypeDatabase::CloneArchetype(const std::bitset<64>& mask, Archetype* archetype, 
	std::shared_ptr<Archetype>& outarche)
{
	//if (_database.find(mask) != _database.end()) {
	//	outarche = _database[mask];
	//	return false;
	//} // else creates it
	auto archetype_ptr = std::make_shared<Archetype>();
	archetype_ptr->_type_offset = archetype->_type_offset;
	archetype_ptr->_descriptions = archetype->_descriptions;
	archetype_ptr->_chunk_stride = archetype->_chunk_stride;
	//archetype_ptr->AddDescriptions(mask);
	archetype_ptr->_mask = mask;
	_database[mask] = archetype_ptr;
	outarche = _database[mask];
	return true;
}

void ArchetypeDatabase::FlushEntities() {

	std::vector<Com_Node*> cn = SystemDatabase::Instance().GetAllComponents<Com_Node>();
	for (auto cnft : cn)
	{
		cnft->~Com_Node();
	}
	
	// manual destruction calling, for allocated data in components
	std::vector<Com_Tilemap*> v = SystemDatabase::Instance().GetAllComponents<Com_Tilemap>();
	for (auto t : v) {
		t->~Com_Tilemap();
	}

	// freeing of resources
	for (auto& archetype : _database) {
		for (auto& chunk : archetype.second->_chunk_database) {
			// zero out memory
			chunk->Free();
		}
	}
	//_database = std::unordered_map<std::bitset<64>, std::shared_ptr<Archetype>>();
	std::cout << "ARCHETYPE_DATABASE :: FLUSHED ENTITIES." << std::endl;
}
