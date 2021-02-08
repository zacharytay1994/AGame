#pragma once
#include <iostream>
#include <string>

#include "zComponent.h"
#include "zSystem.h"

/*______________________________________________________________________
	Component	- Example_Position			
	System		- Example_PrintPosition		<Position>
					.. Prints out position to the console.
				- Example_PrintPositionSys	<Position>
					.. Prints out position to the console.
________________________________________________________________________*/
struct Position {
	float x{ 0.0f };
	float y{ 0.0f };
};

void Example_PrintPosition(System& s) {
	std::cout << s._current_id << ": " << s.c<Position>().x << "|" << s.c<Position>().y << std::endl;
}

struct Example_PrintPositionSys : public System {
	std::string s = "hi ";
	void UpdateComponent() override {
		std::cout << s << c<Position>().x << "|" << c<Position>().y << std::endl;
	}
};

/*______________________________________________________________________
	Component	- Example_Velocity
	System		- Example_UpdatePosition <Position, Velocity>
					.. Adds velocity to the position.
________________________________________________________________________*/
struct Example_Velocity {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Example_UpdatePosition : public System {
	std::string s = "I am updating position of entity: ";
	void UpdateComponent() override {
		c<Position>().x += c<Example_Velocity>().x * _dt;
		c<Position>().y += c<Example_Velocity>().y * _dt;
		Position& test = c<Position>();
		std::cout << s << _current_id << " |" << c<Position>().x << "," << c<Position>().y << std::endl;
		RemoveEntity();
	}
};

/*______________________________________________________________________
	Component	- Sprite
	System		- Example_UpdatePosition <Position, Velocity>
					.. Adds velocity to the position.
________________________________________________________________________*/
struct Sprite {
	AEGfxTexture*		_texture	= nullptr;
	AEGfxVertexList*	_mesh		= nullptr;
	float				_x_scale	= 1.0f;
	float				_y_scale	= 1.0f;
	float				_rotation	= 0.0f;
	AEMtx33				_transform;
};

void DrawSprite(System& s) {
	// form the matrix
	AEMtx33 trans, scale, rot;
	Sprite& sprite = s.c<Sprite>();
	Position& position = s.c<Position>();
	AEMtx33Scale(&scale, sprite._x_scale, sprite._y_scale);
	AEMtx33Rot(&rot, sprite._rotation);
	AEMtx33Trans(&trans, position.x, position.y);
	AEMtx33Concat(&sprite._transform, &scale, &rot);
	AEMtx33Concat(&sprite._transform, &sprite._transform, &trans);
	// set aegfx variables
	AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(sprite._transform.m);
	AEGfxTextureSet(sprite._texture, 0.0f, 0.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(sprite._mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
}