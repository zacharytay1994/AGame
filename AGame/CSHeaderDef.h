#pragma once
#include <iostream>
#include <string>

#include "zComponent.h"
#include "zSystem.h"

/*______________________________________________________________________
	Component	- Example_Position			
	System		- Example_PrintPositionSys	<Position>
					.. Prints out position to the console.
________________________________________________________________________*/
struct Com_Position {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Sys_Example_PrintPosition : public System {
	std::string s = "Check out this awesome position: ";
	void UpdateComponent() override {
		std::cout << s << get<Com_Position>().x << "|" << get<Com_Position>().y << std::endl;
		
	}
};

/*______________________________________________________________________
	Component	- Example_Velocity
	System		- Example_UpdatePosition <Position, ArrowKeys>
					.. Changes position based on arrow key input.
________________________________________________________________________*/
struct Com_Example_Velocity {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Sys_Example_UpdatePosition : public System {
	std::string s = "I am updating position of entity: ";
	void UpdateComponent() override {
		get<Com_Position>().x += get<Com_Example_Velocity>().x * _dt;
		get<Com_Position>().y += get<Com_Example_Velocity>().y * _dt;
		Com_Position& test = get<Com_Position>();
		std::cout << s << _current_id << " |" << get<Com_Position>().x << "," << get<Com_Position>().y << std::endl;
		RemoveEntity();
	}
};

/*______________________________________________________________________
	Component	- Com_Sprite
	System		- Sys_DrawSprite <Com_Position, Com_Sprite>
					.. Adds velocity to the position.
________________________________________________________________________*/
struct Com_Sprite {
	AEGfxTexture*		_texture	= nullptr;
	AEGfxVertexList*	_mesh		= nullptr;
	float				_x_scale	= 1.0f;
	float				_y_scale	= 1.0f;
	float				_rotation	= 0.0f;
	int					_frames		= 1;
	int					_current_frame = 0;
	float				_frame_interval = 1;
	float				_frame_interval_counter = 0.0f;
	int					_row = 1;
	int					_col = 1;
	float				_offset_x = 0.0f;
	float				_offset_y = 0.0f;
	AEMtx33				_transform { 0 };
};

struct Sys_DrawSprite : public System {
	std::vector<Com_Sprite*> con;
	void UpdateComponent() override {
		//	// form the matrix
		AEMtx33 trans{ 0 }, scale{ 0 }, rot{ 0 };
		Draw(get<Com_Sprite>(), get<Com_Position>());
	}
	void Draw(Com_Sprite& sprite, Com_Position& position) {
		AEMtx33 trans, scale, rot;
		// increment frame
		if (sprite._frame_interval_counter > sprite._frame_interval) {
			sprite._current_frame = ++sprite._current_frame >= sprite._frames ? 0 : sprite._current_frame;
			sprite._offset_x = (sprite._current_frame % sprite._col) * 1.0f/ (float)sprite._col;
			sprite._offset_y = (sprite._current_frame / sprite._col) * 1.0f / (float)sprite._row;
			sprite._frame_interval_counter = 0.0f;
		}
		else {
			sprite._frame_interval_counter += _dt;
		}
		AEMtx33Scale(&scale, sprite._x_scale, sprite._y_scale);
		AEMtx33Rot(&rot, sprite._rotation);
		AEMtx33Trans(&trans, position.x, position.y);
		AEMtx33Concat(&sprite._transform, &rot, &scale);
		AEMtx33Concat(&sprite._transform, &trans, &sprite._transform);
		// set aegfx variables
		AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(sprite._transform.m);
		AEGfxTextureSet(sprite._texture, sprite._offset_x, sprite._offset_y);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(sprite._mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
	}
};

/*______________________________________________________________________
	Component	- ArrowKeys
	System		- Example_UpdatePosition <Position, Velocity>
					.. Adds velocity to the position.
________________________________________________________________________*/
struct Com_ArrowKeys {
	char _filler = 0; // filler 
};

struct Sys_ArrowKeys : public System {
	void UpdateComponent() override {
		if (AEInputCheckCurr(VK_LEFT)) {
			get<Com_Position>().x -= 100.0f * _dt;
		}
		if (AEInputCheckCurr(VK_RIGHT)) {
			get<Com_Position>().x += 100.0f * _dt;
		}
		if (AEInputCheckCurr(VK_UP)) {
			get<Com_Position>().y += 100.0f * _dt;
		}
		if (AEInputCheckCurr(VK_DOWN)) {
			get<Com_Position>().y -= 100.0f * _dt;
		}
	}
};

/*______________________________________________________________________
	Component	- Com_Tilemap
	System		- Sys_Tilemap <Com_Tilemap>
					.. Renders a tilemap
________________________________________________________________________*/
struct Com_Tilemap {
	std::vector<int> _map;
	std::vector<int> _floor_mask;
	int _width = 0;
	int _height = 0;
	float _x = 0.0f;
	float _y = 0.0f;
	float _scale_x = 1.0f;
	float _scale_y = 1.0f;
	AEGfxTexture* _texture = nullptr;
	AEGfxVertexList* _mesh = nullptr;
	bool _initialized = false;
};

struct Sys_Tilemap : public System {
	void UpdateComponent() override {
		if (get<Com_Tilemap>()._initialized) {
			//// render tilemap
			//if (AEInputCheckCurr(VK_RIGHT)) {
			//	get<Com_Tilemap>()._x += _dt * 10.0f;
			//}
			//if (AEInputCheckCurr(VK_UP)) {
			//	get<Com_Tilemap>()._y += _dt * 10.0f;
			//}
			DrawTilemap(get<Com_Tilemap>());
		}
	}
	void DrawTilemap(const Com_Tilemap& tilemap) {
		AEMtx33 trans, scale, transform;
		AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
		AEMtx33Scale(&scale, tilemap._scale_x, tilemap._scale_y);
		for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
			for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
				if (tilemap._floor_mask[x * (size_t)tilemap._height + y] == -1) { continue; }
				AEMtx33Trans(&trans, (float)x+tilemap._x, -(float)y+tilemap._y);
				AEMtx33Concat(&transform, &scale, &trans);
				AEGfxSetTransform(transform.m);
				if (tilemap._floor_mask[x * (size_t)tilemap._height + y]) {
					// sample texture according to collision mask
					float offset_x = (tilemap._floor_mask[x * (size_t)tilemap._height + y] % 4) * 1.0f / (float)4;
					float offset_y = (tilemap._floor_mask[x * (size_t)tilemap._height + y] / 4) * 1.0f / (float)4;
					AEGfxTextureSet(tilemap._texture, offset_x, offset_y);
					AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
					AEGfxMeshDraw(tilemap._mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
				}
			}
		}
	}
};

/*______________________________________________________________________
	Component	- Com_TilePosition
	System		- Sys_TilePosition <Com_TilePosition, Com_Position>
					.. Binds an entity position to the 
________________________________________________________________________*/
struct Com_TilemapRef {
	Com_Tilemap* _tilemap = nullptr;
};

struct Com_TilePosition {
	int _grid_x = 0;
	int _grid_y = 0;
	int _vgrid_x = 0;	// verified grid positions - do not set
	int _vgrid_y = 0;	// verified grid positions - do not set
};

struct Sys_TilePosition : public System {
	void UpdateComponent() override {
		//Com_TilemapRef& tilemapref = get<Com_TilemapRef>();
		//Com_Tilemap* tilemap = get<Com_TilemapRef>()._tilemap;
		//Com_Position& position = get<Com_Position>();
		//Com_TilePosition& t_position = get<Com_TilePosition>();
		//if (tilemap) {
		//	// check if new tile position is within grid - would be checked with collision_mask after
		//	if (!tilemap->_floor_mask[(size_t)t_position._grid_x * (size_t)tilemap->_height + (size_t)t_position._grid_y]) {
		//		t_position._vgrid_x = t_position._grid_x;
		//		t_position._vgrid_y = t_position._grid_y;
		//	}
		//	// bind position to grid position
		//	position.x = tilemap->_x + (float)t_position._vgrid_x * tilemap->_scale_x;
		//	position.y = tilemap->_y + (float)t_position._vgrid_y * tilemap->_scale_y;
		//}
	}
};