#pragma once
#include "zArchetype.h"

template <typename...T_COMPONENTS>
struct Entity {
	std::shared_ptr<Chunk>	_chunk;
	int						_id;

	//template <typename...T_COMPONENTS>
	Entity() {
		Archetype& archetype = ArchetypeDatabase::Instance().CreateArchetype<T_COMPONENTS ...>();
		_id = archetype.Add(_chunk);
		((_chunk->GetComponent<T_COMPONENTS>(_id) = T_COMPONENTS()), ...);
	}
	template <typename T>
	T& Get() {
		return _chunk->GetComponent<T>(_id);
	}
};