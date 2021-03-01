#pragma once
#include <vector>

#include "zEntity.h"

class Factory {
	uint64_t _unique_ids = 0;
	std::vector<Entity> _entities;
	Factory() = default;
public:
	static Factory& Instance();
	template <typename...T>
	Entity& CreateEntity() {
		_entities.emplace_back();
		_entities.back().Initialize<T...>();
		++_unique_ids;
		return _entities.back();
	}
	void RemoveEntity(Entity* entity);
	Entity& GetEntity(const int& id);

	/*_________________________________________________________
	* Custom Factory Functions
	___________________________________________________________*/
	int FF_Sprite(const std::string& texturename, const int& row, const int& col, const int& frames, const float& interval, const float& scalex, const float& scaley);
	int FF_Tilemap(const std::string& texture, const std::string& bottom, const std::string& top);
	int FF_SpriteRandomPosition(const std::string& texturename, const int& row, const int& col, const int& frames, const float& interval, const float& scalex, const float& scaley,
		const float& x, const float& y);
};