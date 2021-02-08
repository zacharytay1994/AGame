#pragma once
#include <vector>

#include "zEntity.h"

class Factory {
	std::vector<Entity> _entities;
	Factory() = default;
public:
	static Factory& Instance();
	template <typename...T>
	Entity& CreateEntity() {
		_entities.emplace_back();
		_entities.back().Initialize<T...>();
		return _entities.back();
	}
	void RemoveEntity(Entity* entity);
};