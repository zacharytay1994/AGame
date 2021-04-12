/******************************************************************************/
/*!
\file		zArchetype.h
\author 	Zachary Tay
\par    	email: z.tay\@digipen.edu
\date   	April 12, 2021
\brief		Archetype for ECS

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "zComponent.h"
#include <memory>
#include <bitset>
#include <unordered_map>
#include <string>

struct Chunk;
struct Archetype {
	uint32_t									_chunk_stride = 0;
	uint32_t									_entities_per_chunk = 100;
	std::vector<const ComponentDescription*>	_descriptions;
	std::unordered_map<std::string, int>		_type_offset;
	std::bitset<64>								_mask;
	std::vector<std::shared_ptr<Chunk>>			_chunk_database;
	template <typename T_COMPONENT>
	void AddDescriptions() {
		_descriptions.push_back(&component_description_v<T_COMPONENT>);
		_type_offset[typeid(T_COMPONENT).name()] = _chunk_stride;
		_chunk_stride += sizeof(T_COMPONENT);
	}
	int Add(std::shared_ptr<Chunk>& chunk);
};

struct ArchetypeDatabase {
	static ArchetypeDatabase& Instance();
	template <typename...T_COMPONENTS>
	std::shared_ptr<Archetype> CreateArchetype() {
		// create mask & check if archetype already exist
		std::bitset<64> mask;
		((mask[component_description_v<T_COMPONENTS>._bit] = 1), ...);
		if (_database.find(mask) != _database.end()) {
			return _database[mask];
		} // else creates it
		_database[mask] = std::make_shared<Archetype>();
		((_database[mask]->AddDescriptions<T_COMPONENTS>()), ...);
		_database[mask]->_mask = mask;
		return _database[mask];
	}
	bool CloneArchetype(const std::bitset<64>& mask, Archetype* archetype, std::shared_ptr<Archetype>& outarche);
	void FlushEntities();
	std::unordered_map<std::bitset<64>, std::shared_ptr<Archetype>> _database;
private:
	ArchetypeDatabase() = default;
};