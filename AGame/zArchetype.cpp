#include "zArchetype.h"
#include "zChunk.h"

int Archetype::Add(std::shared_ptr<Chunk>& chunk) {
	// check if there are any empty chunks
	for (auto i : _chunk_database) {
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
