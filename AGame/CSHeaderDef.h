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
struct Com_Velocity {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Sys_Example_UpdatePosition : public System {
	std::string s = "I am updating position of entity: ";
	void UpdateComponent() override {
		get<Com_Position>().x += get<Com_Velocity>().x * _dt;
		get<Com_Position>().y += get<Com_Velocity>().y * _dt;
		Com_Position& test = get<Com_Position>();
		std::cout << s << _current_id << " |" << get<Com_Position>().x << "," << get<Com_Position>().y << std::endl;
		//RemoveEntity();
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
	float _offset_x = 0.0f;
	float _offset_y = 0.0f;
	int _width = 0;
	int _height = 0;
	float _scale_x = 1.0f;
	float _scale_y = 1.0f;
	AEGfxTexture* _texture = nullptr;
	AEGfxVertexList* _mesh = nullptr;
	bool _initialized = false;
};

struct Sys_Tilemap : public System {
	void UpdateComponent() override {
		Com_Tilemap& tilemap = get<Com_Tilemap>();
		if (tilemap._initialized) {
			Com_Position& position = get<Com_Position>();
			DrawTilemap(tilemap,position);
		}
	}
	void DrawTilemap(const Com_Tilemap& tilemap, const Com_Position& position) {
		AEMtx33 trans, scale, transform;
		AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
		AEMtx33Scale(&scale, tilemap._scale_x, tilemap._scale_y);
		for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
			for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
				if (tilemap._floor_mask[x * (size_t)tilemap._height + y] == -1) { continue; }
				AEMtx33Trans(&trans, (float)x+tilemap._offset_x, -(float)y+tilemap._offset_y);
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

struct Sys_TilemapPosition : public System {
	void UpdateComponent() override {
		Com_Tilemap& tilemap = get<Com_Tilemap>(); 
		Com_Position& position = get<Com_Position>();
		tilemap._offset_x = position.x;
		tilemap._offset_y = position.y;
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
		Com_TilemapRef& tilemapref = get<Com_TilemapRef>();
		Com_Tilemap* tilemap = tilemapref._tilemap;
		Com_Position& position = get<Com_Position>();
		Com_TilePosition& t_position = get<Com_TilePosition>();
		if (tilemap) {
			// check if new tile position is within grid - would be checked with collision_mask after
			if (t_position._grid_x >= 0 && t_position._grid_x < tilemap->_width && t_position._grid_y >= 0 && t_position._grid_y < tilemap->_height &&
				tilemap->_floor_mask[(size_t)t_position._grid_x * (size_t)tilemap->_height + (size_t)t_position._grid_y] >= 0) {
				t_position._vgrid_x = t_position._grid_x;
				t_position._vgrid_y = t_position._grid_y;
			}
			else {
				t_position._grid_x = t_position._vgrid_x;
				t_position._grid_y = t_position._vgrid_y;
			}
			// bind position to grid position
			position.x = tilemap->_offset_x * tilemap->_scale_x + (float)t_position._vgrid_x * tilemap->_scale_x;
			position.y = tilemap->_offset_y * tilemap->_scale_y - (float)t_position._vgrid_y * tilemap->_scale_y;
		}
	}
};

struct Com_ArrowKeysTilemap {
	char _filler = 0;
};

struct Sys_ArrowKeysTilemap : public System {
	void UpdateComponent() override {
		Com_TilePosition& pos = get<Com_TilePosition>();
		if (AEInputCheckTriggered(VK_LEFT)) {
			pos._grid_x -= 1;
		}
		if (AEInputCheckTriggered(VK_RIGHT)) {
			pos._grid_x += 1;
		}
		if (AEInputCheckTriggered(VK_UP)) {
			pos._grid_y -= 1;
		}
		if (AEInputCheckTriggered(VK_DOWN)) {
			pos._grid_y += 1;
		}
	}
};

/*___________________________________________________________________________________________________________________________
	Component - Com_Direction
______________________________________________________________________________________________________________________*/
struct Com_Direction {
	enum Direction { up, down, left, right };
	int currdir;
};

/*___________________________________________________________________________________________________________________________
	Component - Com_BoundingBox
	System - Sys_Boundingbox <Com_BoundingBox,Com_Position>
	Calculates the bounding box of entity
______________________________________________________________________________________________________________________*/
struct Com_BoundingBox
{
	Com_Position min;
	Com_Position max;
	float scale;
};

struct Sys_Boundingbox : public System {
	void UpdateComponent() override {
		//calculate AABB bounding box 
		calculateAABB(get<Com_BoundingBox>(), get<Com_Position>(), get<Com_BoundingBox>());
	}
	/*-----------------------------------------

	@brief updates the bounding box
	@param Com_BoundingBox& BoundingBox
	x,y
	@param Com_Position& Position
	x,y
	@param Com_BoundingBox& scale
	scale of object
	@return void

	------------------------------------------*/
	void calculateAABB(Com_BoundingBox& boundingbox, Com_Position& position, Com_BoundingBox& scale)
	{
		//calculate min max
		boundingbox.max.x = -0.5f * boundingbox.scale + position.x;
		boundingbox.min.x = 0.5f * boundingbox.scale + position.x;
		boundingbox.min.y = -0.5f * boundingbox.scale + position.y;
		boundingbox.max.y = 0.5f * boundingbox.scale + position.y;
	}

};


/*___________________________________________________________________________________________________________________________
	System - Sys_AABB <Com_BoundingBox,Com_Position,Com_velocity>
	AABB Collision detection
______________________________________________________________________________________________________________________*/
struct CollisionData {
	Com_BoundingBox* aabb;
	Com_Velocity* vel;
};

struct Sys_AABB : public System {
	std::vector<CollisionData> AABBTest; //to store all collision data 
	void UpdateComponent() override {
		//calculate AABB detection
		int collisionflag = 0;
		Com_BoundingBox* AABB = &get<Com_BoundingBox>();
		Com_Velocity* vel = &get<Com_Velocity>();
		for (int i{ 0 }; i < AABBTest.size(); ++i) {
			collisionflag = CollisionAABB(*AABB, *vel, *AABBTest[i].aabb, *AABBTest[i].vel);
		}
		AABBTest.emplace_back(CollisionData{ AABB,vel });
	}

	/*-----------------------------------------
	@brief does an AABB Collision check
	@param Com_BoundingBox obj 1
	@param Com_Velocity obj 1
	@param Com_BoundingBox obj 2
	@param Com_Velocity obj 2
	@return bool
	1 - collide/ 0 - no collide
	------------------------------------------*/
	bool CollisionAABB(const Com_BoundingBox& object1, const Com_Velocity& objvel1,
		const Com_BoundingBox& object2, const Com_Velocity& objvel2)
	{
		if ((object1.max.x < object2.min.x || object1.min.x > object2.max.x) || (object1.max.y < object2.min.y || object1.min.y > object2.max.y)) { // check static collision
			// initialzing time of first and last contact 
			float tFirst = 0.0f; //init tfirst 
			float tLast = (float)AEFrameRateControllerGetFrameTime(); //inti tlast 
			AEVec2 Rvel;
			Rvel.x = objvel2.x - objvel1.x;
			Rvel.y = objvel2.y - objvel1.y;
			if (Rvel.x < 0.0) { // if relative velocity x < 0
				if (object2.max.x < object1.min.x) {
					return 0;//no collision
				}
				if (object1.max.x < object2.min.x) {
					tFirst = max((object1.max.x - object2.min.x) / Rvel.x, tFirst); //calculate tfirst touch
				}
				if (object2.max.x > object1.min.x) {
					tLast = min((object1.min.x - object2.max.x) / Rvel.x, tLast); //calculate tlast touch
				}
			}
			if (Rvel.x > 0.0) { //if relative velocity x > 0
				if (object2.min.x > object1.max.x) {
					return 0;//no collision
				}
				if (object2.max.x < object1.min.x) {
					tFirst = max((object1.min.x - object2.max.x) / Rvel.x, tFirst);  // calculate tfirst touch
				}
				if (object1.max.x > object2.min.x) {
					tLast = min((object1.max.x - object2.min.x) / Rvel.x, tLast); //calculate tlast touch 
				}
			}
			if (Rvel.y < 0.0) //if relative velocity y < 0
			{
				if (object2.max.y < object1.min.y) {
					return 0; // not collison moving apart 
				}
				if (object1.max.y < object2.min.y) {
					tFirst = max((object1.max.y - object2.min.y) / Rvel.y, tFirst); //calculate tfirst touch
				}
				if (object2.max.y > object1.min.y) {
					tLast = min((object1.min.y - object2.max.y) / Rvel.y, tLast); //calculate tlast touch
				}
			}
			if (Rvel.y > 0.0)
			{
				if (object2.min.y > object1.max.y) {
					return 0; // not collison moving apart 

				}
				if (object2.max.y < object1.min.y) {
					tFirst = max((object1.min.y - object2.max.y) / Rvel.y, tFirst);  // cacluate tfirst touch

				}
				if (object1.max.y > object2.min.y) {
					tLast = min((object1.max.y - object2.min.y) / Rvel.y, tLast);  //calculate tlast touch
				}
			}
			if (tFirst > tLast) {
				return 0; //no collision
			}
			else { return 1; }  //dynamic collision
		}
		return 1;  //static collision 
	}
};

/*___________________________________________________________________________________________________________________________
	Components - Com_Projectile , Com_GameObj
	System - Sys_Projectile<Com_Position,
______________________________________________________________________________________________________________________*/

struct Sys_Projectile : public System {
	void UpdateComponent() override {
		Com_Direction& direction= get<Com_Direction>();
		//if space triggered 
		if (AEInputCheckCurr(VK_SPACE)) {
			if (direction.currdir == direction.right) {
				//create an entity of bullet 
				CreateProjectile(get<Com_Position>(), get<Com_Velocity>(), get<Com_Direction>());
			}
			if (direction.currdir == direction.left) {
				//create an entity of bullet 
				CreateProjectile(get<Com_Position>(), get<Com_Velocity>(), get<Com_Direction>());
			}
			if (direction.currdir == direction.up) {
				//create an entity of bullet 
				CreateProjectile(get<Com_Position>(), get<Com_Velocity>(), get<Com_Direction>());
			}
			if (direction.currdir == direction.down) {
				//create an entity of bullet 
				CreateProjectile(get<Com_Position>(), get<Com_Velocity>(), get<Com_Direction>());
			}
	
		}
	}
	void CreateProjectile(Com_Position& position, Com_Velocity& velocity, Com_Direction& direction) {
		//creat projectile based on direction
		//Factory::Instance().CreateEntity<Com_Sprite, Com_Velocity, Com_Position, Com_BoundingBox, Com_Direction>();
		//set entity sprite, position, velocity, direction
	}
};

/*___________________________________________________________________________________________________________________________
	System - Sys_CheckBoundaries
______________________________________________________________________________________________________________________*/

struct Sys_Boundary : public System {
	void UpdateComponent() override {
		//check the boundary
		checkboundary(get<Com_Position>());
	}
	void checkboundary(Com_Position position) {
		//if outside the view port 
		if (position.x > AEGfxGetWinMaxX()) {
			//destroy the entity
			RemoveEntity();
		}
		if (position.x < AEGfxGetWinMinX()) {
			//destroy the entity
			RemoveEntity();
		}
		if (position.y > AEGfxGetWinMaxY()) {
			//destroy the entity
			RemoveEntity();
		}
		if (position.y < AEGfxGetWinMinY()) {
			//destroy the entity
			RemoveEntity();
		}
	}
};


/*___________________________________________________________________________________________________________________________
	Components - Weapons
	System - Sys_Weapons
______________________________________________________________________________________________________________________*/
struct Com_WeaponAttack
{
	enum Weapons {
		sword,
		pistol
	};
	int currentweapon{ 0 };
};

struct Sys_WeaponAttack : public System {
	void UpdateComponent() override {
		if (AEInputCheckCurr(VK_SPACE)) {
			//if character holding to sword 
			if (get<Com_WeaponAttack>().currentweapon == Com_WeaponAttack::sword) {
				//attack the grid infront 

			}
			//if character holding to pistol 
			if (get<Com_WeaponAttack>().currentweapon == Com_WeaponAttack::pistol) {
				//shoot out projectile 
				//Sys_Projectile::CreateProjectile(Com_Position & position, Com_Velocity & velocity, Com_Direction & direction);
			}
		}
	}

	void sword_attack() {

	}
};

/*
struct Sys_ArrowKeys : public System {
	void UpdateComponent() override {
		if (AEInputCheckCurr(VK_LEFT)) {
			//if already facing this direction, move 
			if (get<Com_Direction>().currdir == Com_Direction::left) {
				get<Com_Position>().x -= get<Com_Tilemap>()._scale_x;
			}
			//if not facing this direction, change it to face there
			else {
				get<Com_Direction>().currdir = Com_Direction::left;
			}
		}
		if (AEInputCheckCurr(VK_RIGHT)) {
			//if already facing this direction, move 
			if (get<Com_Direction>().currdir == Com_Direction::right) {
				get<Com_Position>().x += get<Com_Tilemap>()._scale_x;
			}
			//if not facing this direction, change it to face there
			else {
				get<Com_Direction>().currdir = Com_Direction::right;
			}
		}
		if (AEInputCheckCurr(VK_UP)) {
			if (get<Com_Direction>().currdir == Com_Direction::up) {
				get<Com_Position>().y += get<Com_Tilemap>()._scale_y;
			}
			//if not facing this direction, change it to face there
			else {
				get<Com_Direction>().currdir = Com_Direction::up;
			}
		}
		if (AEInputCheckCurr(VK_DOWN)) {
			if (get<Com_Direction>().currdir == Com_Direction::down) {
				get<Com_Position>().y -= get<Com_Tilemap>()._scale_y;
			}
			//if not facing this direction, change it to face there
			else {
				get<Com_Direction>().currdir = Com_Direction::down;
			}
		}
	}
};
*/