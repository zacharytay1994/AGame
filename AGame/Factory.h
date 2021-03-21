#pragma once
#include <vector>
#include <assert.h>

#include "zEntity.h"

typedef int eid; // entity id
//forward declare to draw from cpp 
struct Com_Direction;
struct Com_GUISurface;

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
		std::cout << "entity created" << std::endl;
		return (eid)_unique_ids++;
	}
	void RemoveEntity(Entity* entity);
	Entity& GetEntity(const int& id);
	void Free();

	/*_________________________________________________________
	* Custom Factory Functions
	___________________________________________________________*/
	struct SpriteData {
		int _layer;
		std::string _texturename;
		int _row;
		int _col;
		int _frames;
		float _interval;
		float _scalex;
		float _scaley;
		SpriteData(const std::string& name, const float& scalex = 1.0f, const float& scaley = 1.0f, int row = 1.0, int col = 1.0, int frames = 1.0f, const float& interval = 1.0f, int layer = 0)
			:
			_texturename(name),
			_scalex(scalex),
			_scaley(scaley),
			_row(row),
			_col(col),
			_frames(frames),
			_interval(interval),
			_layer(layer)
		{}
	};
	eid FF_Sprite(const SpriteData& data, const float& x, const float& y);
	eid FF_Tilemap(const std::string& texture, const std::string& bottom, const std::string& top);
	eid FF_SpriteTile(const SpriteData& data, const eid& tilemap, const int& x, const int& y);
	//eid FF_SpriteRandomPosition(const SpriteData& data, const float& x, const float& y, const float& velX, const float& velY);

	template <typename MovementType>
	eid FF_SpriteMovableTile(const SpriteData& data, const eid& tilemap, const int& x, const int& y) {
		eid id = FF_SpriteTile(data, tilemap, x, y);
		Factory::Instance()[id].AddComponent<MovementType>();
		return id;
	}

	// gui ffs
	eid FF_CreateGUISurface(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, int layer);
	eid FF_CreateGUISurfaceText(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, const std::string& text, const std::string& font, int layer);
	eid FF_CreateGUIClickableSurface(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*), int layer);
	eid FF_CreateGUIClickableSurfaceText(const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*), const std::string& text, const std::string& font, int layer);
	eid FF_CreateGUIChildSurface(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height);
	eid FF_CreateGUIChildSurfaceText(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, const std::string& text, const std::string& font);
	eid FF_CreateGUIChildClickableSurface(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*));
	eid FF_CreateGUIChildClickableSurfaceText(eid parent, const SpriteData& data, const float& x, const float& y, const float& width, const float& height, void(*onclick)(Com_GUISurface*), const std::string& text, const std::string& font);
	eid FF_CreateGUISettings();

	//created by wilf for testing 
	eid FF_Createproj(const SpriteData& data, const int& x, const int& y,const Com_Direction& direction); //create projectile 
	eid FF_Createproj2(const SpriteData& data, const int& x, const int& y, const int& vel_x, const int& vel_y, eid const& tilemap, int lifetime = -1); //modified by Noel for Tilebased
	//eid FF_CreateEnemy(const SpriteData& data, const eid& tilemap, const int& x, const int& y);


	eid FF_CreateEnemy(const SpriteData& data, const eid& tilemap, const int& x, const int& y); //create enemy
	eid FF_CreateParticle(const SpriteData& data, const int& x, const int& y, const float& velx, const float& vely); // create particle
	eid FF_CreateBomb(const SpriteData& data, const int& x, const int& y);//create bomb
};