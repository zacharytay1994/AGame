#include "zChunk.h"
#include "zArchetype.h"
#include <iostream>

Chunk::Chunk(Archetype* holder, const uint32_t& size)
	:
	_owning_archetype(holder),
	_number_of_entities(0)
{
	int bytesize = 0;
	for (auto i : _owning_archetype->_descriptions) {
		bytesize += i->_size;
	}
	_data = std::make_unique<char[]>(bytesize*size);
	/*for (int i = 0; i < bytesize * size; ++i) {
		std::cout << i << ": " << (int)*(_data.get() + i) << std::endl;
	}*/
}

int Chunk::Add() {
	++_number_of_entities;
	return _number_of_entities - 1;
}
