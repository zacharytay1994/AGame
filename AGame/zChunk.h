#pragma once
#include <memory>
#include <stack>
#include "zArchetype.h"

struct Chunk {
	std::shared_ptr<Archetype>	_owning_archetype;
	std::unique_ptr<char[]>		_data;
	uint32_t					_number_of_entities;
	std::stack<int>				_free_ids;
	std::vector<int>			_active_flags;
	Chunk(Archetype* holder, const uint32_t& size);
	int Add();
	void Remove(const int& id);
	template <typename T>
	T& GetComponent(const int& id) {
		char* data = _data.get();
		data += (size_t)id * (size_t)_owning_archetype->_chunk_stride + (size_t)_owning_archetype->_type_offset[typeid(T).name()];
		T& test = *((T*)data);
		return *((T*)data);
	}
	char* GetDataBegin(const int& id);
	void MoveData(const int destination, const int source);
	void ZeroData(const int id);
	void ZeroAllData();
	void Free();
};