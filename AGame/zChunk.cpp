#include "zChunk.h"
#include "zArchetype.h"
#include <iostream>

Chunk::Chunk(Archetype* holder, const uint32_t& size)
	:
	_owning_archetype(holder),
	_number_of_entities(0)
{
	size_t bytesize = 0;
	for (auto i : _owning_archetype->_descriptions) {
		bytesize += i->_size;
	}
	_data = std::make_unique<char[]>(bytesize*(size_t)size);
}

int Chunk::Add() {
	// check if free ids stack has any to give
	if (_free_ids.empty()) {
		++_number_of_entities;
		return _number_of_entities - 1;
	}
	int id = _free_ids.top();
	_free_ids.pop();
	return id;
}

void Chunk::Remove(const int& id)
{
	--_number_of_entities;
	_free_ids.push(id);
}
