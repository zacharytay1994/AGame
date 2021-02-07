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
	fp_update				_update;
	bool					_initialized = false;

	Chunk*					_current_chunk = nullptr;
	int						_current_id = -1;
	float					_dt = -1.0f;
	template <typename...T>
	void Initialize(fp_update u) {
		((_mask[component_description_v<T>._bit] = 1), ...);
		_update = u;
		_initialized = true;
	}
	System() = default;
	void UpdateComponent() {
		if (_initialized) {
			_update(*this);
		}
	}
	template <typename T>
	T& c() {
		return _current_chunk->GetComponent<T>(_current_id);
	}
};

class SystemDatabase {
	SystemDatabase() = default;
	std::vector<std::unique_ptr<System>> _database;
public:
	static SystemDatabase& Instance();
	template <typename...T>
	void RegisterSystem(fp_update u) {
		_database.push_back(std::make_unique<System>());
		_database.back()->Initialize<T...>(u);
		std::cout << "SYSTEM |" << typeid(fp_update).name() << "| registered.";
	}
	void SystemDatabaseUpdate(const float& dt);
};