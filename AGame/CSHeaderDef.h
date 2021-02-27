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
	AEMtx33				_transform;
};

struct Sys_DrawSprite : public System {
	void UpdateComponent() override {
		//	// form the matrix
		AEMtx33 trans, scale, rot;
		Draw(trans, scale, rot, get<Com_Sprite>(), get<Com_Position>());
	}
	void Draw(AEMtx33& trans, AEMtx33& scale, AEMtx33& rot, Com_Sprite& sprite, Com_Position& position) {
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
		AEMtx33Concat(&sprite._transform, &scale, &rot);
		AEMtx33Concat(&sprite._transform, &sprite._transform, &trans);
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
			get<Com_Position>().x -= 10.0f * _dt;
		}
		if (AEInputCheckCurr(VK_RIGHT)) {
			get<Com_Position>().x += 10.0f * _dt;
		}
		if (AEInputCheckCurr(VK_UP)) {
			get<Com_Position>().y += 10.0f * _dt;
		}
		if (AEInputCheckCurr(VK_DOWN)) {
			get<Com_Position>().y -= 10.0f * _dt;
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
	int _width = 0;
	int _height = 0;
};

struct Sys_Tilemap : public System {

};