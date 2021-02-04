#pragma once
#include <bitset>
#include <memory>
#include <vector>

#include "zComponent.h"

struct Chunk;
struct System;
struct Archetype;
typedef void (*fp_update)(const float&, Chunk& chunk, const int& id);

struct System {
	std::bitset<64>			_mask;
	fp_update				_update;
	bool					_initialized = false;
	template <typename...T>
	void Initialize(fp_update u) {
		((_mask[component_description_v<T>._bit] = 1), ...);
		_update = u;
		_initialized = true;
	}
	System() = default;
	void UpdateComponent(const float& dt, Chunk& chunk, const int& id) {
		if (_initialized) {
			_update(dt, chunk, id);
		}
	}
};

class SystemDatabase {
	SystemDatabase() = default;
	std::vector<std::unique_ptr<System>> _database;
public:
	static SystemDatabase& Instance();
	template <typename...T>
	void AddSystem(fp_update u) {
		_database.push_back(std::make_unique<System>());
		_database.back()->Initialize<T...>(u);
	}
	void SystemDatabaseUpdate(const float& dt);
};