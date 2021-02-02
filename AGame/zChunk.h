#pragma once
#include <memory>
#include "zArchetype.h"

struct Chunk {
	Archetype*				_owning_archetype;
	std::unique_ptr<char>	_data;
	uint32_t				_number_of_entities;
	Chunk(Archetype* holder, const uint32_t& size);
	int Add();
	template <typename T>
	T& GetComponent(const int& id) {
		char* data = _data.get();
		data += (_number_of_entities - 1) * _owning_archetype->_chunk_stride + _owning_archetype->_type_offset[typeid(T).name()];
		return *((T*)data);
	}
};