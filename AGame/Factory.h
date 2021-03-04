#pragma once
#include <vector>
#include <assert.h>

#include "zEntity.h"

typedef int eid; // entity id

class Factory {
	uint64_t _unique_ids = 0;
	std::vector<Entity> _entities;
	Factory() = default;
public:
	static Factory& Instance();
	Entity& operator[](const int& i) {
		assert(i < _unique_ids);
		return _entities[i];
	}
	template <typename...T>
	eid CreateEntity() {
		_entities.emplace_back();
		_entities.back().Initialize<T...>();
		return (eid)_unique_ids++;
	}
	void RemoveEntity(Entity* entity);
	Entity& GetEntity(const int& id);
	void Free();

	/*_________________________________________________________
	* Custom Factory Functions
	___________________________________________________________*/
	struct SpriteData {
		std::string texturename;
		int row;
		int col;
		int frames;
		float interval;
		float scalex;
		float scaley;
	};
	eid FF_Sprite(const SpriteData& data, const float& x, const float& y);
	eid FF_Tilemap(const std::string& texture, const std::string& bottom, const std::string& top);
	eid FF_SpriteTile(const SpriteData& data, const eid& tilemap, const int& x, const int& y);
	template <typename MovementType>
	eid FF_SpriteMovableTile(const SpriteData& data, const eid& tilemap, const int& x, const int& y) {
		eid id = FF_SpriteTile(data, tilemap, x, y);
		Factory::Instance()[id].AddComponent<MovementType>();
		return id;
	}


	//created by wilf for testing 
	eid FF_Createproj(const SpriteData& data, const int& x, const int& y);


};