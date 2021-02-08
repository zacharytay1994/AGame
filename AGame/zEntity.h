#pragma once
#include "zArchetype.h"
#include "zChunk.h"

//template <typename...T_COMPONENTS>
struct Entity {
	std::shared_ptr<Chunk>	_chunk{ nullptr };
	int						_id{ 0 };

	//template <typename...T_COMPONENTS>
	Entity() {}
	template <typename...T_COMPONENTS>
	void Initialize() {
		std::shared_ptr<Archetype> archetype = ArchetypeDatabase::Instance().CreateArchetype<T_COMPONENTS ...>();
		_id = archetype->Add(_chunk);
		((_chunk->GetComponent<T_COMPONENTS>(_id) = T_COMPONENTS()), ...);
	}
	template <typename T>
	T& Get() {
		return _chunk->GetComponent<T>(_id);
	}
	void Destroy();
};