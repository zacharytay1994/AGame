#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "AEEngine.h"
#include "Factory.h"

#include "ResourceManager.h"
#include "zComponent.h"
#include "zSystem.h"

#include "zMath.h"

using namespace std;


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
// Nodes
struct Com_PathFinding;
struct Com_Node;
// GUI
struct Com_GUISurface;
struct Com_GUIOnClick;
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
	RenderPack			_render_pack;
	float				_x_scale = 1.0f;
	float				_y_scale = 1.0f;
	float				_rotation = 0.0f;
	int					_frames = 1;
	int					_current_frame = 0;
	float				_frame_interval = 1;
	float				_frame_interval_counter = 0.0f;
	int					_row = 1;
	int					_col = 1;
};

struct Com_Direction {
	enum Direction { up, down, left, right };
	int currdir = up;
};

struct Com_Boundary {
	char _filler = 0; //filler 
};

struct Com_YLayering {
	char filler = 0;
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
	RenderPack		_render_pack;
	std::vector<int> _map;
	std::vector<int> _floor_mask;
	float _offset_x{ 0.0f };
	float _offset_y{ 0.0f };
	int _width = 0;
	int _height = 0;
	float _scale_x = 1.0f;
	float _scale_y = 1.0f;
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

/*																Component::PATH FINDING
____________________________________________________________________________________________________*/
struct Com_PathFinding
{
	bool bObstacle = false;			// Is the node an obstruction?
	bool bVisited = false;			// Have we searched this node before?
	float fGlobalGoal = 0.0f;				// Distance to goal so far
	float fLocalGoal = 0.0f;				// Distance to goal if we took the alternative route
	int x = 0;							// Nodes position in 2D space
	int y = 0;
	vector<Com_PathFinding*> vecNeighbours;	// Connections to neighbours
	Com_PathFinding* parent = nullptr;					// Node connecting to this node that offers shortest parent
};

struct Com_Node
{
	Com_PathFinding* nodeStart = nullptr;
	Com_PathFinding* nodeEnd = nullptr;
	Com_PathFinding* nodes = nullptr;
	/*int nMapWidth = 0; 
	int nMapHeight = 0;*/

};

/*																				Component::GUI
____________________________________________________________________________________________________*/
struct Com_GUISurface {
	Vec2f			_position		{ 0.0f, 0.0f };
	Vec2f			_dimensions		{ 1.0f, 1.0f };
	Vec2f			_ph_dimensions	{ 1.0f, 1.0f };
	Com_GUISurface*	_parent_surface	{ nullptr };
	Com_Position*	_parent_position{ nullptr };
};

using OnClick = void(*)(Com_GUISurface* surface);
struct Com_GUIOnClick {
	OnClick _click_event{ nullptr };
};

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
	>> GUI
		- Sys_GUISurfaceRender
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

/*																				system::PATHFINDING
____________________________________________________________________________________________________*/
struct Sys_PathFinding;

/*																				system::GUI
____________________________________________________________________________________________________*/
struct Sys_GUISurfaceRender;

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
			sprite._render_pack._offset_x = (sprite._current_frame % sprite._col) * 1.0f / (float)sprite._col;
			sprite._render_pack._offset_y = (sprite._current_frame / sprite._col) * 1.0f / (float)sprite._row;
			sprite._frame_interval_counter = 0.0f;
		}
		else {
			sprite._frame_interval_counter += _dt;
		}
		AEMtx33Scale(&scale, sprite._x_scale, sprite._y_scale);
		AEMtx33Rot(&rot, sprite._rotation);
		AEMtx33Trans(&trans, position.x, position.y);
		AEMtx33Concat(&sprite._render_pack._transform, &rot, &scale);
		AEMtx33Concat(&sprite._render_pack._transform, &trans, &sprite._render_pack._transform);
		// set aegfx variables
		ResourceManager::Instance().DrawQueue(sprite._render_pack);
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

struct Sys_YLayering : public System {
	void UpdateComponent() override {
		// sets the layer to the position
		get<Com_Sprite>()._render_pack._layer = -get<Com_Position>().y;
	}
};

/*																				system::INPUT
____________________________________________________________________________________________________*/

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
	void UpdateComponent() override {
		Com_Tilemap& tilemap = get<Com_Tilemap>();
		if (tilemap._initialized) {
			Com_Position& position = get<Com_Position>();
			DrawTilemap(tilemap, position);
		}
	}
	void DrawTilemap(Com_Tilemap& tilemap, const Com_Position& position) {
		AEMtx33 trans, scale, transform;
		AEMtx33Scale(&scale, tilemap._scale_x, tilemap._scale_y);
		for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
			for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
				if (tilemap._floor_mask[x * (size_t)tilemap._height + y] == -1) { continue; }
				AEMtx33Trans(&trans, (float)x + tilemap._offset_x, -(float)y + tilemap._offset_y);
				AEMtx33Concat(&transform, &scale, &trans);
				tilemap._render_pack._transform = transform;
				if (tilemap._floor_mask[x * (size_t)tilemap._height + y]) {
					// sample texture according to collision mask
					float _offset_x = (tilemap._floor_mask[x * (size_t)tilemap._height + y] % 4) * 1.0f / (float)4;
					float _offset_y = (tilemap._floor_mask[x * (size_t)tilemap._height + y] / 4) * 1.0f / (float)4;
					tilemap._render_pack._offset_x = (tilemap._floor_mask[x * (size_t)tilemap._height + y] % 4) * 1.0f / (float)4;
					tilemap._render_pack._offset_y = (tilemap._floor_mask[x * (size_t)tilemap._height + y] / 4) * 1.0f / (float)4;
					ResourceManager::Instance().DrawQueue(tilemap._render_pack);
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
	float time = AEGetTime(nullptr);
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

struct Sys_Projectile2 : public System {
	void UpdateComponent() override {
		Com_Projectile& proj = get<Com_Projectile>();
		if (AEGetTime(nullptr) - proj.time > AEFrameRateControllerGetFrameTime() * 10)
		{
			proj.time = AEGetTime(nullptr);
			Com_Direction& direction = get<Com_Direction>();
			Com_TilePosition& tileposition = get<Com_TilePosition>();
			if (direction.currdir == direction.left)
			{
				tileposition._grid_x--;
			}
			else if (direction.currdir == direction.right)
			{
				tileposition._grid_x++;
			}
			else if (direction.currdir == direction.up)
			{
				tileposition._grid_y--;
			}
			else if (direction.currdir == direction.down)
			{
				tileposition._grid_y++;
			}

			Com_TilemapRef& tilemapref = get<Com_TilemapRef>();
			Com_Tilemap* tilemap = tilemapref._tilemap;
			if (tilemap) {
				// check if new tile position is within grid - would be checked with collision_mask after
				if (tileposition._grid_x >= 0 && tileposition._grid_x < tilemap->_width && tileposition._grid_y >= 0 && tileposition._grid_y < tilemap->_height &&
					tilemap->_floor_mask[(size_t)tileposition._grid_x * (size_t)tilemap->_height + (size_t)tileposition._grid_y] >= 0) {
					// Do nothing
				}
				else {
					RemoveEntity();
				}
			}
		}
		else
		{
			//AEGetTime(nullptr) - proj.time > AEFrameRateControllerGetFrameRate() * 3
			std::cout << std::endl << AEGetTime(nullptr) << " - " << proj.time << " is not more than " << AEFrameRateControllerGetFrameTime() << " * 3" << std::endl;
		}
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


/*																			system::PATH FINDING
____________________________________________________________________________________________________*/

struct Sys_PathFinding : public System
{
	void UpdateComponent() override {
		Com_Node* ode = &get<Com_Node>();
		Com_Tilemap& tile = get<Com_Tilemap>();
		Com_Position& PlayerPos = get<Com_Position>();
	}
	

void MapCreate(Com_Node& ode, const Com_Tilemap& tile)
{
	// Create a 2D array of nodes - this is for convenience of rendering and construction
	// and is not required for the algorithm to work - the nodes could be placed anywhere
	// in any space, in multiple dimension
	int height = tile._height;
	int width = tile._width;
	int MapArea = width * height;
	//vector<Com_PathFinding> nodes(MapArea); // create vector with size MapArea
	ode.nodes = new Com_PathFinding[MapArea];
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			ode.nodes[x * height + y].x = x; // to give each node its own coordinates
			ode.nodes[x * height + y].y = y;
			// set everything to default value 1st
			ode.nodes[x * height + y].bObstacle = false;
			ode.nodes[x * height + y].parent = nullptr;
			ode.nodes[x * height + y].bVisited = false;
		}

	// Create connections - in this case nodes are on a regular grid
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			if (x > 0)
				ode.nodes[x * height + y].vecNeighbours.push_back(&ode.nodes[(x - 1) * height + (y + 0)]);
			if (x < width - 1)
				ode.nodes[x * height + y].vecNeighbours.push_back(&ode.nodes[(x + 1) * height + (y + 0)]);
			if (y > 0)
				ode.nodes[x * height + y].vecNeighbours.push_back(&ode.nodes[(x + 0) * height + (y - 1)]);
			if (y < height - 1)
				ode.nodes[x * height + y].vecNeighbours.push_back(&ode.nodes[(x + 0) * height + (y + 1)]);

		}

	// Manually positio the start and end markers so they are not nullptr
	ode.nodeStart = &ode.nodes[(height / 2) * width + 1];
	ode.nodeEnd = &ode.nodes[(height / 2) * width + width - 2];
}

bool Solve_AStar(Com_Node& ode, Com_Tilemap& tile)
{
	int height = tile._height;
	int width = tile._width;
	int MapArea = width * height;
	//vector<Com_PathFinding> nodes(MapArea); // create vector with size MapArea
	// Reset Navigation Graph - default all node states
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			ode.nodes[x * height + y].bVisited = false;
			ode.nodes[x * height + y].fGlobalGoal = INFINITY;
			ode.nodes[x * height + y].fLocalGoal = INFINITY;
			ode.nodes[x * height + y].parent = nullptr;	// No parents
		}

	auto distance = [](Com_PathFinding* a, Com_PathFinding* b) // For convenience
	{
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](Com_PathFinding* a, Com_PathFinding* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};

	// Setup starting conditions
	Com_PathFinding* nodeCurrent = ode.nodeStart;
	ode.nodeStart->fLocalGoal = 0.0f;
	ode.nodeStart->fGlobalGoal = heuristic(ode.nodeStart, ode.nodeEnd);

	// Add start node to not tested list - this will ensure it gets tested.
	// As the algorithm progresses, newly discovered nodes get added to this
	// list, and will themselves be tested later
	list<Com_PathFinding*> listNotTestedNodes;
	listNotTestedNodes.push_back(ode.nodeStart);

	// if the not tested list contains nodes, there may be better paths
	// which have not yet been explored. However, we will also stop 
	// searching when we reach the target - there may well be better
	// paths but this one will do - it wont be the longest.
	while (!listNotTestedNodes.empty() && nodeCurrent != ode.nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const Com_PathFinding* lhs, const Com_PathFinding* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		// Front of listNotTestedNodes is potentially the lowest distance node. Our
		// list may also contain nodes that have been visited, so ditch these...
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		// ...or abort because there are no valid nodes left to test
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true; // We only explore a node once


		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			// ... and only if the neighbour is not visited and is 
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			// Calculate the neighbours potential lowest parent distance
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, ode.nodeEnd);
			}
		}
	}

	return true;
}


};

/*																				system::GUI
____________________________________________________________________________________________________*/
struct Sys_GUISurfaceRender : public System {
	float _screen_width = (float)AEGetWindowWidth();
	float _screen_height = (float)AEGetWindowHeight();
	void UpdateComponent() override {
		Com_GUISurface& surface = get<Com_GUISurface>();
		Com_Position& position = get<Com_Position>();
		if (surface._parent_surface) {
			// offset with parent
			position.x = surface._parent_position->x - surface._parent_surface->_ph_dimensions.x + surface._position.x*surface._parent_surface->_ph_dimensions.x*2.0f;
			position.y = surface._parent_position->y + surface._parent_surface->_ph_dimensions.y - surface._position.y*surface._parent_surface->_ph_dimensions.y*2.0f;
		}
		else {
			// update sprite position with button
			position.x = (surface._position.x - 0.5f) * _screen_width;
			position.y = -((surface._position.y - 0.5f) * _screen_height);
		}
	}
};

struct Sys_GUISurfaceOnClick : public System {
	bool _left_mouse{ false };
	Vec2i _mouse_position{ 0,0 };
	Vec2f _screen_dimensions{ AEGetWindowWidth()/2.0f,AEGetWindowHeight()/2.0f };
	void OncePerFrame() override {
		_left_mouse = AEInputCheckTriggered(AEVK_LBUTTON);
		AEInputGetCursorPosition(&_mouse_position.x, &_mouse_position.y);
		_mouse_position.x -= _screen_dimensions.x;
		_mouse_position.y -= _screen_dimensions.y;
		_mouse_position.y *= -1;
	}
	void UpdateComponent() override {
		Com_GUIOnClick& on_click = get<Com_GUIOnClick>();
		Com_GUISurface& surface = get<Com_GUISurface>();
		Com_Position& position = get<Com_Position>();
		// do bounding box check
		if (_left_mouse && !(_mouse_position.x < position.x - surface._ph_dimensions.x || _mouse_position.x > position.x + surface._ph_dimensions.x ||
			_mouse_position.y < position.y - surface._ph_dimensions.y || _mouse_position.y > position.y + surface._ph_dimensions.y)) { 
			on_click._click_event(&surface);
		}
	}
};
