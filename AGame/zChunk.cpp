/******************************************************************************/
/*!
\File Name		: zChunk.cpp
\Project Name	: AGame
\Authors 		:
				Primary - Zachary Tay (100%)
				Secondary -
\brief		chunk for ecs

All content � 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/
#include "zChunk.h"
#include "zArchetype.h"
#include <iostream>

//chunk init 
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

//add chunk
int Chunk::Add() {
	// check if free ids stack has any to give
	if (_free_ids.empty()) {
		++_number_of_entities;
		_active_flags.emplace_back(1);
		return _number_of_entities - 1;
	}
	int id = _free_ids.top();
	_active_flags[id] = 1;
	_free_ids.pop();
	return id;
}
//remove chunk
void Chunk::Remove(const int& id)
{
	_active_flags[id] = 0;
	ZeroData(id);
	_free_ids.push(id);
}
//get data
char* Chunk::GetDataBegin(const int& id)
{
	return _data.get() + (size_t)id * (size_t)_owning_archetype->_chunk_stride;
}
//move chunk data
void Chunk::MoveData(const int destination, const int source)
{
	char* data = _data.get();
	char* dest = data + (size_t)destination * (size_t)_owning_archetype->_chunk_stride;
	char* src = data + (size_t)source * (size_t)_owning_archetype->_chunk_stride;
	memcpy(dest, src, _owning_archetype->_chunk_stride);
}
//chunk zero specific data id 
void Chunk::ZeroData(const int id)
{
	char* data = _data.get();
	char* mem = data + (size_t)id * (size_t)_owning_archetype->_chunk_stride;
	memset(mem, 0, _owning_archetype->_chunk_stride);
}
//chunk zero all data
void Chunk::ZeroAllData()
{
	char* data = _data.get();
	int i = (size_t)_owning_archetype->_chunk_stride * (size_t)_owning_archetype->_entities_per_chunk;
	memset(data, 0, i);
}
//free chunk
void Chunk::Free()
{
	_number_of_entities = 0;
	_free_ids = std::stack<int>();
	_active_flags = std::vector<int>();
	ZeroAllData();
}
