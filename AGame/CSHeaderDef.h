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
		std::cout << s << c<Com_Position>().x << "|" << c<Com_Position>().y << std::endl;
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
		c<Com_Position>().x += c<Com_Example_Velocity>().x * _dt;
		c<Com_Position>().y += c<Com_Example_Velocity>().y * _dt;
		Com_Position& test = c<Com_Position>();
		std::cout << s << _current_id << " |" << c<Com_Position>().x << "," << c<Com_Position>().y << std::endl;
		RemoveEntity();
	}
};

/*______________________________________________________________________
	Component	- Sprite
	System		- Example_UpdatePosition <Position, Velocity>
					.. Adds velocity to the position.
________________________________________________________________________*/
struct Com_Sprite {
	AEGfxTexture*		_texture	= nullptr;
	AEGfxVertexList*	_mesh		= nullptr;
	float				_x_scale	= 1.0f;
	float				_y_scale	= 1.0f;
	float				_rotation	= 0.0f;
	AEMtx33				_transform;
};

struct Sys_DrawSprite : public System {
	void UpdateComponent() override {
		//	// form the matrix
		AEMtx33 trans, scale, rot;
		Com_Sprite& sprite = c<Com_Sprite>();
		Com_Position& position = c<Com_Position>();
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
			c<Com_Position>().x -= 10.0f * _dt;
		}
		if (AEInputCheckCurr(VK_RIGHT)) {
			c<Com_Position>().x += 10.0f * _dt;
		}
		if (AEInputCheckCurr(VK_UP)) {
			c<Com_Position>().y += 10.0f * _dt;
		}
		if (AEInputCheckCurr(VK_DOWN)) {
			c<Com_Position>().y -= 10.0f * _dt;
		}
	}
};