#pragma once
#include <iostream>
#include <string>
#include "AEEngine.h"
#include "Factory.h"
#include "zComponent.h"
#include "zSystem.h"

/*___________________________________________________________________________________________________________________________________
	COMPONENT DECLARATIONS & DEFINITONS																	<<	COMPONENT DEFINITIONS  >>
	_________________________________________________________________________________________________________________________________*/
// basic data
struct Com_Position;
struct Com_Velocity;
struct Com_Sprite;
struct Com_Direction;
// input
struct Com_ArrowKeys;
struct Com_ArrowKeysTilemap;
// tilemap
struct Com_Tilemap;
struct Com_TilemapRef;
struct Com_TilePosition;
// collision
struct Com_BoundingBox;
struct Com_CollisionData;
// attack
struct Com_WeaponAttack;
/*__________________________________________________________________________________________________
																				Component::BASIC DATA
____________________________________________________________________________________________________*/

//timer
struct Com_GameTimer {
	size_t timerinseconds{ 0 };
	size_t incrementer{ 0 };
};

struct Com_Position {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Com_Velocity {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Com_Sprite {
	AEGfxTexture* _texture = nullptr;
	AEGfxVertexList* _mesh = nullptr;
	float				_x_scale = 1.0f;
	float				_y_scale = 1.0f;
	float				_rotation = 0.0f;
	int					_frames = 1;
	int					_current_frame = 0;
	float				_frame_interval = 1;
	float				_frame_interval_counter = 0.0f;
	int					_row = 1;
	int					_col = 1;
	float				_offset_x = 0.0f;
	float				_offset_y = 0.0f;
	AEMtx33				_transform{ 0 };
};

struct Com_Direction {
	enum Direction { up, down, left, right };
	int currdir = up;
};

struct Com_Boundary {
	char _filler = 0; //filler 
};

/*																				Component::INPUT
____________________________________________________________________________________________________*/

struct Com_ArrowKeys {
	char _filler = 0; // filler 
};

struct Com_ArrowKeysTilemap {
	char _filler = 0;
};
/*																				Component::TILEMAP
____________________________________________________________________________________________________*/

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

struct Com_TilemapRef {
	Com_Tilemap* _tilemap = nullptr;
};

struct Com_TilePosition {
	int _grid_x = 0;
	int _grid_y = 0;
	int _vgrid_x = 0;	// verified grid positions - do not set
	int _vgrid_y = 0;	// verified grid positions - do not set
};

/*																				Component::COLLISION
____________________________________________________________________________________________________*/
struct Com_BoundingBox
{
	Com_Position min;
	Com_Position max;
	float scale;
};

struct CollisionData {
	Com_BoundingBox* aabb;
	Com_Velocity* vel;
};

/*																				Component::ATTACK
____________________________________________________________________________________________________*/
struct Com_WeaponAttack
{
	enum Weapons {
		sword,
		pistol
	};
	int currentweapon{ 1 };
};;

/*																				Component::ENEMY
____________________________________________________________________________________________________*/

/*___________________________________________________________________________________________________________________________________
	SYSTEM DECLARATIONS				<< RIGHT CLICK ON DECLARATION TO NAVIGATE TO DEFINITION!! >>			<<	SYSTEM DECLARATIONS  >>
	_________________________________________________________________________________________________________________________________
	>> basic systems
		- Sys_DrawSprite
		- Sys_Boundary
	>> input
		- Sys_ArrowKeys
		- Sys_ArrowKeysTilemap
	>> tilemap
		- Sys_Tilemap
		- Sys_TilemapPosition
		- Sys_TilePosition
	>> collision
		- Sys_BoundingBox
		- Sys_AABB
	>> attack
		- Sys_Projectile
________________________________________________________________________*/
/*																				system::BASIC SYSTEMS
____________________________________________________________________________________________________*/
struct Sys_DrawSprite;
struct Sys_Boundary;

/*																				system::INPUT
____________________________________________________________________________________________________*/
struct Sys_ArrowKeys;
struct Sys_ArrowKeysTilemap;

/*																				system::TILEMAP
____________________________________________________________________________________________________*/
struct Sys_Tilemap;
struct Sys_TilemapPosition;
struct Sys_TilePosition;

/*																				system::COLLISION
____________________________________________________________________________________________________*/
struct Sys_BoundingBox;
struct Sys_AABB;

/*																				system::ATTACK
____________________________________________________________________________________________________*/
struct Sys_WeaponAttack;

/*___________________________________________________________________________________________________________________________________
	SYSTEM DEFINITIONS																						<<	SYSTEM DEFINITIONS  >>
_____________________________________________________________________________________________________________________________________*/
/*																				system::BASIC SYSTEMS
____________________________________________________________________________________________________*/

struct Sys_Velocity : public System {
	void UpdateComponent() override {
		Com_Velocity& velocity = get<Com_Velocity>();
		Com_Position& position = get<Com_Position>();
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
	}
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
			sprite._offset_x = (sprite._current_frame % sprite._col) * 1.0f / (float)sprite._col;
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

/*																				system::INPUT
____________________________________________________________________________________________________*/

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

struct Sys_ArrowKeysTilemap : public System {
	void UpdateComponent() override {
		Com_TilePosition& pos = get<Com_TilePosition>();
		Com_Direction& direction = get<Com_Direction>();
		if (AEInputCheckTriggered(VK_LEFT)) {
			//if already left 
			if (direction.currdir == direction.left) {
				pos._grid_x -= 1;
			}
			else {
				direction.currdir = direction.left;
			}
		}
		if (AEInputCheckTriggered(VK_RIGHT)) {
			if (direction.currdir == direction.right) {
				pos._grid_x += 1;
			}
			else {
				direction.currdir = direction.right;
			}
		}
		if (AEInputCheckTriggered(VK_UP)) {
			if (direction.currdir == direction.up) {
				pos._grid_y -= 1;
			}
			else {
				direction.currdir = direction.up;
			}
		}
		if (AEInputCheckTriggered(VK_DOWN)) {
			if (direction.currdir == direction.down) {
				pos._grid_y += 1;
			}
			else {
				direction.currdir = direction.down;
			}
		}
	}
};

/*																				system::TILEMAP
____________________________________________________________________________________________________*/

struct Sys_Tilemap : public System {
	int i = 0;
	void UpdateComponent() override {
		Com_Tilemap& tilemap = get<Com_Tilemap>();
		if (tilemap._initialized) {
			Com_Position& position = get<Com_Position>();
			DrawTilemap(tilemap, position);
		}
	}
	void DrawTilemap(const Com_Tilemap& tilemap, const Com_Position& position) {
		AEMtx33 trans, scale, transform;
		AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
		AEMtx33Scale(&scale, tilemap._scale_x, tilemap._scale_y);
		for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
			for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
				if (tilemap._floor_mask[x * (size_t)tilemap._height + y] == -1) { continue; }
				AEMtx33Trans(&trans, (float)x + tilemap._offset_x, -(float)y + tilemap._offset_y);
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
/*																				system::COLLISION
____________________________________________________________________________________________________*/

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

/*																				system::ATTACK
____________________________________________________________________________________________________*/

struct Com_Projectile {
	char _filler = 0; //filler
};


struct Sys_Projectile : public System {
	Factory::SpriteData data = { "test", 1, 1, 1, 100.0f, 50.0f, 50.0f };
	//passing in of player's data 
	virtual void CreateProjectile(Com_Direction& direction,Com_Position& position) {
		//calling the factory fnc
		Factory::Instance().FF_Createproj(data, position.x, position.y,direction);
	}
};


struct Sys_PlayerAttack : public Sys_Projectile {
	void UpdateComponent() override {

		if (AEInputCheckCurr(VK_SPACE)) {
			Com_Direction& direction = get<Com_Direction>();
			Com_WeaponAttack& weapon = get<Com_WeaponAttack>();
			Com_Position& position = get<Com_Position>();
			if (direction.currdir == direction.up) {
				//if character holding to sword 
				if (weapon.currentweapon == weapon.sword) {
					//attack the grid infront or shoort invisible bullet 
					sword_attack(direction, position);

				}
				//if character holding to pistol 
				if (weapon.currentweapon == weapon.pistol) {
					//shoot out projectile 
					CreateProjectile(direction, position);
				}

			}
			if (direction.currdir == direction.down) {
				//if character holding to sword 
				if (weapon.currentweapon == weapon.sword) {
					//attack the grid infront or shoort invisible bullet 
					sword_attack(direction, position);

				}
				//if character holding to pistol 
				if (weapon.currentweapon == weapon.pistol) {
					//shoot out projectile 
					CreateProjectile(direction, position);
				}
			}
			if (direction.currdir == direction.left) {
				//if character holding to sword 
				if (weapon.currentweapon == weapon.sword) {
					//attack the grid infront or shoort invisible bullet 
					sword_attack(direction, position);

				}
				//if character holding to pistol 
				if (weapon.currentweapon == weapon.pistol) {
					//shoot out projectile 
					CreateProjectile(direction, position);
				}
			}
			if (direction.currdir == direction.right) {
				//if character holding to sword 
				if (weapon.currentweapon == weapon.sword) {
					//attack the grid infront or shoort invisible bullet 
					sword_attack(direction, position);

				}
				//if character holding to pistol 
				if (weapon.currentweapon == weapon.pistol) {
					//shoot out projectile 
					CreateProjectile(direction, position);
				}
			}
		}
	}
	void sword_attack(Com_Direction& direction, Com_Position& position) {
		//pending 
	}
};


 /////////Edits  

/*-------------------------------------
//for spawning of enemies 
-------------------------------------------*/
struct Com_EnemySpawn{
	size_t numberofenemies{ 5 }; //number of enemies to spawn 
};

struct Com_Wave{
	size_t timerforwave{ 60 }; //if timer hits 0 in secsm spawn new wave 
	size_t numberofwaves{ 3 }; //if number of wave hit 0, level unlocked 
};

//logic for spawning of enemies 
struct Sys_EnemySpawning : public System {
	void UpdateComponent() override {
		Com_EnemySpawn& Enemyspawn = get<Com_EnemySpawn>();
		Com_Wave& wave = get<Com_Wave>();
		Com_GameTimer& timer = get<Com_GameTimer>();
		//if the timer hits for set time 
		//if timer hit 0 spawn wave/ number of enemies hit 0 
		if (timer.timerinseconds == wave.timerforwave || Enemyspawn.numberofenemies == 0) {
			//spawning of enemies 
			spawn_enemies();
			--wave.numberofwaves; //decrease the number of waves left 
			timer.timerinseconds = 0;
		}
	}
	void spawn_enemies() {
		//spawn enemy at a certain location
		//create enemy entity 
		/*Factory::Instance().CreateEntity<Com_Sprite, Com_Position, Com_BoundingBox, Com_Direction, 
			Com_TilePosition, Com_Tilemap,Com_TypeEnemy,Com_EnemySpawn,Com_Wave>();*/
	}
};


/*-------------------------------------
			//for attack of enemies 
-------------------------------------------*/

struct Com_TypeEnemy {
	enum EnemyType
	{
		Alien1, //melee
		Alien2  //range
	};
	size_t Alientype{ 0 };
};


//logic for attack of enemies 
struct Sys_EnemyAttack : public Sys_Projectile {
	void UpdateComponent() override {
		//if enemy is melee
		if (get<Com_TypeEnemy>().Alientype == Com_TypeEnemy::Alien1) {
			//check 4 sides if player is 1 tile away
			//if () {
			//	//shoot invisible that direction 
			//	//create projectile system
			//}
		}
		//if enemy is range
		if (get<Com_TypeEnemy>().Alientype == Com_TypeEnemy::Alien2) {
			//check 4 sides if player is x/y aligned 
			//if () {
			//	//shoot that direction
			//	//createprojectile system
			//}
		}
	}
};

/*-------------------------------------
			//timing for game/wave
-------------------------------------------*/

//frame rate non independent timer 
struct Sys_GameTimer : public System {
	void UpdateComponent() override {
		++get<Com_GameTimer>().incrementer;
		if (AEFrameRateControllerGetFrameRate() < get<Com_GameTimer>().incrementer) {
			get<Com_GameTimer>().incrementer = 0; //reset incrementer 
			++get<Com_GameTimer>().timerinseconds; //add 1 sec
		}
	}
};

