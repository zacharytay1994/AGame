#pragma once
#include <bitset>
#include <memory>
#include <vector>
#include <iostream>

#include "zComponent.h"
#include "zChunk.h"

struct Chunk;
struct System;
struct Archetype;
typedef void (*fp_update)(System& system);
struct System {
	std::bitset<64>			_mask;
	fp_update				_update = nullptr;

	Chunk*					_current_chunk = nullptr;
	int						_current_id = -1;
	float					_dt = -1.0f;
	template <typename...T>
	void BuildMask() {
		((_mask[component_description_v<T>._bit] = 1), ...);
	}
	System() = default;
	virtual void OncePerFrame() {}
	virtual void UpdateComponent() {
		if (_update) {
			_update(*this);
		}
	}
	template <typename T>
	T& get() {
		return _current_chunk->GetComponent<T>(_current_id);
	}
	void RemoveEntity();
};

class SystemDatabase {
	SystemDatabase() = default;
	std::vector<std::unique_ptr<System>> _database;
public:
	static SystemDatabase& Instance();
	template <typename...T>
	void RegisterSystem(fp_update u) {
		_database.push_back(std::make_unique<System>());
		_database.back()->_update = u;
		_database.back()->BuildMask<T...>();
		std::cout << "SYSTEM UPDATE REGISTERED." << std::endl;
	}
	template <typename T, typename...COMPONENTS>
	void RegisterSystem() {
		_database.push_back(std::make_unique<T>());
		_database.back()->BuildMask<COMPONENTS...>();
		std::cout << "SYSTEM OVERRIDE |" << typeid(T).name() << "| REGISTERED." << std::endl;
	}
	void SystemDatabaseUpdate(const float& dt);
	template <typename T>
	std::vector<T*> GetAllComponents() {
		std::vector<T*> out;
		ArchetypeDatabase& adb = ArchetypeDatabase::Instance();
		std::bitset<64> mask;
		mask[component_description_v<T>._bit] = 1;
		for (auto& archetype : adb._database) {
			// and the mask to see which archetypes are of interest
			if ((mask & archetype.second->_mask) == mask) {
				// loop through
				for (auto& chunk : archetype.second->_chunk_database) {
					for (int i = 0; i < chunk->_number_of_entities; ++i) {
						out.emplace_back(&chunk->GetComponent<T>(i));
					}
				}
			}
		}
		return out;
	}
};