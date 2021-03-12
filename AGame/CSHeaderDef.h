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
#include "GlobalGameData.h"

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
struct Com_GUIMouseCheck;
struct Com_GUIOnClick;
struct Com_GUIDrag;
struct Com_Text;
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
	bool				_visible{ true };
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
	float minx;
	float miny;
	float maxx;
	float maxy;
};


// testing for wilfred ////////////////////////////
struct Com_objecttype {
	enum type
	{
		playert,
		enemyt,
		bullett,
		obstaclest
	};
	eid objtype{ playert };
	bool updated{ false };
	//to store id of all player 
	//static std::vector<eid> player;
	//static std::vector<eid> enemy;
	//static std::vector<eid> bullet;
	//static std::vector<eid> obstacle;
};

//global 
static std::vector<eid> player;
static std::vector<eid> enemy;
static std::vector<eid> bullet;
static std::vector<eid> obstacle;

struct Sys_RegisteringEntity :public System{
	void UpdateComponent() override {
		Com_objecttype& obtype = get<Com_objecttype>();
		if (obtype.updated == false) {
			//if it's a player 
			if (obtype.objtype == obtype.playert) {
				player.emplace_back(player.size()+1);
				std::cout << "assigning player" << std::endl;
			}
			//if it's a enemy 
			if (obtype.objtype == obtype.enemyt) {
				enemy.emplace_back(enemy.size() + 1);
				std::cout << "assigning enemy" << std::endl;
			}
			//if it's a bullet
			if (obtype.objtype == obtype.bullett) {
				bullet.emplace_back(bullet.size() + 1);
				std::cout << "assigning bullet" << std::endl;
			}
			//if it's a obstacle
			if (obtype.objtype == obtype.obstaclest) {
				obstacle.emplace_back(obstacle.size() + 1);
				std::cout << "assigning obstacle" << std::endl;
			}
			obtype.updated = true;
		}
	}
};


// testing for wilfred ////////////////////////////

struct Com_CollisionData {
	Com_BoundingBox* aabb;
	Com_Velocity* vel;
	bool emplacedvec{ false };
};

/*																				Component::ATTACK
____________________________________________________________________________________________________*/
struct Com_WeaponAttack
{
	enum Weapons {
		sword,
		pistol,
		bomb
	};
	int currentweapon{ 1 };
};;

/*																				Component::ENEMY
____________________________________________________________________________________________________*/

struct Com_TypeEnemy {
	enum EnemyType
	{
		Alien1, //melee
		Alien2  //range
	};
	size_t Alientype{ 0 };
};


/*																Component::PATH FINDING
____________________________________________________________________________________________________*/
struct Com_PathFinding
{
	bool bObstacle = false;					// Is the node an obstruction?
	bool bVisited = false;					// Have we searched this node before?
	float fGlobalGoal = 0.0f;				// Distance to goal so far
	float fLocalGoal = 0.0f;				// Distance to goal if we took the alternative route
	int x = 0;								// Nodes position in 2D space
	int y = 0;
	vector<Com_PathFinding*> vecNeighbours;	// Connections to neighbours
	Com_PathFinding* parent = nullptr;		// Node connecting to this node that offers shortest parent
	
	~Com_PathFinding()
	{
		delete parent;
		while (!vecNeighbours.empty()) 
		{
			delete vecNeighbours.back();
			vecNeighbours.pop_back();
		}
	}
};

struct Com_Node
{
	Com_PathFinding* nodeStart = nullptr;
	Com_PathFinding* nodeEnd = nullptr;
	//vector<Com_PathFinding> nodes;
	int MapWidth = 0; 
	int MapHeight = 0;
	Com_PathFinding* nodes = nullptr;
	~Com_Node()
	{
		delete nodes;
		delete nodeStart;
		delete nodeEnd;
	}
};

/*																				Component::GUI
____________________________________________________________________________________________________*/
struct Com_GUISurface {
	Vec2f			_position{ 0.0f, 0.0f };
	Vec2f			_n_position{ 0.0f,0.0f };
	Vec2f			_dimensions{ 1.0f, 1.0f };
	Vec2f			_ph_dimensions{ 1.0f, 1.0f };
	Com_GUISurface* _parent_surface{ nullptr };
	Com_Position*	_parent_position{ nullptr };
	bool			_active{ true };
	bool			_parent_active{ true };
	int				_layer{ 0 };
};

struct Com_GUIMouseCheck {
	bool _over{ false };
};

using OnClick = void(*)(Com_GUISurface* surface);
struct Com_GUIOnClick {
	OnClick _click_event{ nullptr };
};

struct Com_GUIDrag {
	bool _held{ false };
	Vec2f _click_position{ 0.0f,0.0f };
};

struct Com_Text {
	TextPack _data;
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
		- Sys_GUIFontRender
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
struct Sys_GUISurfaceMouseCheck;
struct Sys_GUISurfaceOnClick;
struct Sys_GUITextRender;

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
		Com_Sprite& sprite = get<Com_Sprite>();
		if (!sprite._visible) {
			return;
		}
		//	// form the matrix
		AEMtx33 trans{ 0 }, scale{ 0 }, rot{ 0 };
		Draw(sprite, get<Com_Position>());
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
		ResourceManager::Instance().DrawQueue(&sprite._render_pack);
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
		AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
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
					//ResourceManager::Instance().DrawQueue(&tilemap._render_pack);
					AEGfxSetTransform(tilemap._render_pack._transform.m);
					AEGfxTextureSet(tilemap._render_pack._texture, tilemap._render_pack._offset_x, tilemap._render_pack._offset_y);
					AEGfxMeshDraw(tilemap._render_pack._mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
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
		calculateAABB(get<Com_BoundingBox>(), get<Com_Position>(), get<Com_Sprite>());

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
	void calculateAABB(Com_BoundingBox& boundingbox, Com_Position& position, Com_Sprite& sprite)
	{
		//calculate min max
		boundingbox.maxx = 0.5f * sprite._x_scale + position.x;
		boundingbox.minx = -0.5f * sprite._x_scale + position.x;
		boundingbox.miny = -0.5f * sprite._y_scale + position.y;
		boundingbox.maxy = 0.5f * sprite._y_scale + position.y;
	}
};




struct Sys_AABB : public System {
	std::vector<Com_CollisionData> AABBTestplayer; //to store all collision data of player
	std::vector<Com_CollisionData> AABBTestEnemy; //to store all collision data of player
	std::vector<Com_CollisionData> AABBTestBullet; //to store all collision data of player
	void UpdateComponent() override {
		//calculate AABB detection
		bool collisionflag = false;
		Com_BoundingBox* AABB = &get<Com_BoundingBox>();
		Com_Velocity* vel = &get<Com_Velocity>();
		Com_CollisionData& coldata = get<Com_CollisionData>();
		//for (int i{ 0 }; i < AABBTest.size(); ++i) {
		//	collisionflag = CollisionAABB(*AABB, *vel, *AABBTest[i].aabb, *AABBTest[i].vel);
		//}
		//AABBTest.emplace_back(Com_CollisionData{ AABB,vel });
		//edits for testing 
		//collision testing for player
		Com_objecttype& objtype = get<Com_objecttype>();
		if (objtype.objtype == objtype.playert && coldata.emplacedvec == false) {
			AABBTestplayer.emplace_back(Com_CollisionData{ AABB,vel });
			coldata.emplacedvec = true;
		}
		if (objtype.objtype == objtype.playert && coldata.emplacedvec == true) {
			//test with enemy
			for (int i{ 0 }; i < AABBTestEnemy.size(); ++i) {
				//test with enemy
				collisionflag = CollisionAABB(*AABB, *vel, *AABBTestEnemy[i].aabb, *AABBTestEnemy[i].vel);
			}
		}

		//for enemy 
		if (objtype.objtype == objtype.enemyt && coldata.emplacedvec == false) {
			AABBTestEnemy.emplace_back(Com_CollisionData{ AABB,vel });
			coldata.emplacedvec = true;
		}
		if (objtype.objtype == objtype.enemyt && coldata.emplacedvec == true) {
			//tbc
		}

		//for bullet 
		if (objtype.objtype == objtype.bullett && coldata.emplacedvec == false) {
			AABBTestBullet.emplace_back(Com_CollisionData{ AABB,vel });
			coldata.emplacedvec = true;
		}
		if (objtype.objtype == objtype.bullett && coldata.emplacedvec == true) {
			//test with enemy
			for (int i{ 0 }; i < AABBTestEnemy.size(); ++i) {
				//test with enemy
				collisionflag = CollisionAABB(*AABB, *vel, *AABBTestEnemy[i].aabb, *AABBTestEnemy[i].vel);
				//if collide 
				if (collisionflag == true) {
					RemoveEntity();
					//std::cout << "collidde" << std::endl;
				}
			}
		}

		//check with diff type objects 
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
		if ((object1.maxx < object2.minx) || (object1.minx > object2.maxx) || (object1.maxy < object2.miny) || (object1.miny > object2.maxy)) // check static collision
		{


			// initialzing time of first and last contact 
			float tFirst = 0.0f; //init tfirst 
			float tLast = (float)AEFrameRateControllerGetFrameTime(); //inti tlast 
			AEVec2 Rvel;
			Rvel.x = objvel2.x - objvel1.x;
			Rvel.y = objvel2.y - objvel1.y;

			if (Rvel.x < 0.0) // if relative velocity x < 0
			{
				if (object2.maxx < object1.minx)
				{
					return 0;//no collision
				}
				if (object1.maxx < object2.minx)
				{
					tFirst = max((object1.maxx - object2.minx) / Rvel.x, tFirst); //calculate tfirst touch
				}
				if (object2.maxx > object1.minx)
				{
					tLast = min((object1.minx - object2.maxx) / Rvel.x, tLast); //calculate tlast touch
				}
			}

			if (Rvel.x > 0.0) //if relative velocity x > 0
			{
				if (object2.minx > object1.maxx)
				{
					return 0;//no collision
				}
				if (object2.maxx < object1.minx)
				{
					tFirst = max((object1.minx - object2.maxx) / Rvel.x, tFirst);  // calculate tfirst touch
				}
				if (object1.maxx > object2.minx)
				{
					tLast = min((object1.maxx - object2.minx) / Rvel.x, tLast); //calculate tlast touch 
				}

			}

			if (tFirst > tLast)
			{
				return 0; //no collision
			}

			if (Rvel.y < 0.0) //if relative velocity y < 0
			{
				if (object2.maxy < object1.miny)
				{
					return 0; // not collison moving apart 
				}
				if (object1.maxy < object2.miny)
				{
					tFirst = max((object1.maxy - object2.miny) / Rvel.y, tFirst); //calculate tfirst touch
				}
				if (object2.maxy > object1.miny)
				{
					tLast = min((object1.miny - object2.maxy) / Rvel.y, tLast); //calculate tlast touch
				}
			}

			if (Rvel.y > 0.0)
			{
				if (object2.miny > object1.maxy)
				{
					return 0; // not collison moving apart 

				}
				if (object2.maxy < object1.miny)
				{
					tFirst = max((object1.miny - object2.maxy) / Rvel.y, tFirst);  // cacluate tfirst touch

				}
				if (object1.maxy > object2.miny)
				{
					tLast = min((object1.maxy - object2.miny) / Rvel.y, tLast);  //calculate tlast touch
				}

			}

			if (tFirst > tLast || tFirst == 0)
			{
				return 0; //no collision
			}

			else
			{
				return 1;  //dynamic collision
			}
		}

		return 1;  //static collision 
	}

};

/*																				system::ATTACK
____________________________________________________________________________________________________*/

struct Com_Projectile {
	char _filler = 0; //filler
	float time = static_cast<float>(AEGetTime(nullptr));
	int grid_vel_x = 0;
	int grid_vel_y = 0;
};


struct Sys_Projectile : public System {
	Factory::SpriteData data = { "test", 20, 20, 1, 100.0f, 100.0f, 100.0f };
	//passing in of player's data 
	virtual void CreateProjectile(Com_Direction& direction,Com_Position& position) {
		//calling the factory fnc
		Factory::Instance().FF_Createproj(data, position.x, position.y,direction);
	}
};


struct Sys_PlayerAttack : public Sys_Projectile {
	void UpdateComponent() override {

		Com_Direction& direction = get<Com_Direction>();
		Com_WeaponAttack& weapon = get<Com_WeaponAttack>();
		Com_Position& position = get<Com_Position>();
		if (AEInputCheckTriggered(VK_SPACE)) {
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
			//if it's a bomb
			if (weapon.currentweapon == weapon.bomb) {
				//create a bomb on current tile 
				Plant_Bomb(position);
			}
		}
		//change weapon 
		if (AEInputCheckTriggered(AEVK_0)) {
			weapon.currentweapon = weapon.sword;
		}
		if (AEInputCheckTriggered(AEVK_1)) {
			weapon.currentweapon = weapon.pistol;
		}
		if (AEInputCheckTriggered(AEVK_2)) {
			weapon.currentweapon = weapon.bomb;
		}
	}
	void sword_attack(Com_Direction& direction, Com_Position& position) {
		//pending 
	}
	void Plant_Bomb(Com_Position& position) {
		//setting the sprite data to pass in 
		Factory::SpriteData data{ "kaboom", 40.0f, 40.0f, 1, 1, 1, 0.15f };
		//creating the bomb 
		Factory::Instance().FF_CreateBomb(data, position.x,position.y);
	}
};

struct Sys_Projectile2 : public System {
	void UpdateComponent() override {
		Com_Projectile& proj = get<Com_Projectile>();
		if (AEGetTime(nullptr) - proj.time > AEFrameRateControllerGetFrameTime() * 10)
		{
			Com_TilemapRef& tilemapref = get<Com_TilemapRef>();
			Com_Tilemap* tilemap = tilemapref._tilemap;

			proj.time = static_cast<float>(AEGetTime(nullptr));
			Com_TilePosition& tileposition = get<Com_TilePosition>();

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
			
			if (proj.grid_vel_x > 0)
			{
				tileposition._grid_x++;
			}
			else if (proj.grid_vel_x < 0)
			{
				tileposition._grid_x--;
			}
			if (proj.grid_vel_y > 0)
			{
				tileposition._grid_y--;
			}
			else if (proj.grid_vel_y < 0)
			{
				tileposition._grid_y++;
			}
			
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
	}
};

 /////////Edits  

/*-------------------------------------
//for spawning of enemies 
-------------------------------------------*/
struct Com_EnemySpawn{
	size_t numberofenemies{ 2 }; //number of enemies to spawn
	size_t CurrNoOfEnemies{ 0 }; //keep track of enemies on map
};

struct Com_Wave{
	size_t timerforwave{ 10 }; //if timer hits 0 in secsm spawn new wave 
	size_t numberofwaves{ 3 }; //if number of wave hit 0, level unlocked 
};

//logic for spawning of enemies 
struct Sys_EnemySpawning : public System {
	// Initialization
	eid _tilemap = {-1};


	void UpdateComponent() override {
		Com_EnemySpawn& Enemyspawn = get<Com_EnemySpawn>();
		Com_Wave& wave = get<Com_Wave>();
		//Com_GameTimer& timer = get<Com_GameTimer>();
		static float alarm = 0;
		alarm += _dt;
		//if the timer hits for set time 
		//if timer hit 0 spawn wave/ number of enemies hit 0 
		if (alarm > wave.timerforwave || Enemyspawn.numberofenemies == 0) 
		{
			//spawning of enemies 
			if (Enemyspawn.CurrNoOfEnemies < 5) 
			{
				spawn_enemies(Enemyspawn);
				--wave.numberofwaves; //decrease the number of waves left 
				alarm = 0;
			}
			
		}
	}
	void spawn_enemies(Com_EnemySpawn& enem) {
		//spawn enemy at a certain location
		//create enemy entity 
		/*Factory::Instance().CreateEntity<Com_Sprite, Com_Position, Com_BoundingBox, Com_Direction, 
			Com_TilePosition, Com_Tilemap,Com_TypeEnemy,Com_EnemySpawn,Com_Wave>();*/
		
		int i = 0;
		while (i < enem.numberofenemies) 
		{
			Factory::SpriteData data1{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.25f };
			eid enemy = Factory::Instance().FF_CreateEnemy(data1, _tilemap, 5,2);
			Factory::Instance()[enemy].AddComponent<Com_YLayering, Com_Node, Com_PathFinding>();
			++enem.CurrNoOfEnemies;
			++i;
		}
	}

};

/*-------------------------------------
			//for attack of enemies 
-------------------------------------------*/

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
		Com_GameTimer& gametimer = get<Com_GameTimer>();
		++gametimer.incrementer;
		if (AEFrameRateControllerGetFrameRate() < gametimer.incrementer) {
			//std::cout << gametimer.timerinseconds << "\n";
			gametimer.incrementer = 0; //reset incrementer 
			++gametimer.timerinseconds; //add 1 sec
		}
	}
};


/*																			system::PATH FINDING
____________________________________________________________________________________________________*/

struct Sys_PathFinding : public System
{
		eid playerPos{ -1 };
	void UpdateComponent() override {
		Com_Node& ode = get<Com_Node>();
		Com_TilemapRef& tilemapref = get<Com_TilemapRef>();
		Com_Tilemap* tile = tilemapref._tilemap;
		Com_TilePosition& EnemyPos = get<Com_TilePosition>();
		
		MapCreate(ode, tile, EnemyPos, playerPos);
		Solve_AStar(ode, EnemyPos);
	
	}
	

void MapCreate(Com_Node& ode, const Com_Tilemap* tile, Com_TilePosition& enemyPos, eid& player)
{
	// Create a 2D array of nodes - this is for convenience of rendering and construction
	// and is not required for the algorithm to work - the nodes could be placed anywhere
	// in any space, in multiple dimension
	ode.MapHeight = tile->_height;
	ode.MapWidth = tile->_width;
	int MapArea = ode.MapHeight * ode.MapWidth;
	ode.nodes = new Com_PathFinding[MapArea];
	for (int y = 0; y < ode.MapHeight; y++)
	{
		for (int x = 0; x < ode.MapWidth; x++)
		{
			ode.nodes[x * ode.MapHeight + y].x = x; // to give each node its own coordinates
			ode.nodes[x * ode.MapHeight + y].y = y;
			// set everything to default value 1st
			ode.nodes[x * ode.MapHeight + y].bObstacle = false;
			ode.nodes[x * ode.MapHeight + y].parent = nullptr;
			ode.nodes[x * ode.MapHeight + y].bVisited = false;
		}
	}
	// Create connections - in this case nodes are on a regular grid
	for (int y = 0; y < ode.MapHeight; y++)
		for (int x = 0; x < ode.MapWidth; x++)
		{
			if (y > 0)
				ode.nodes[x * ode.MapHeight + y].vecNeighbours.push_back(&ode.nodes[(x + 0) * ode.MapHeight + (y - 1)]);
			if (y < ode.MapHeight - 1)
				ode.nodes[x * ode.MapHeight + y].vecNeighbours.push_back(&ode.nodes[(x + 0) * ode.MapHeight + (y + 1)]);
			if (x > 0)
				ode.nodes[x * ode.MapHeight + y].vecNeighbours.push_back(&ode.nodes[(x - 1) * ode.MapHeight + (y + 0)]);
			if (x < ode.MapWidth - 1)
				ode.nodes[x * ode.MapHeight + y].vecNeighbours.push_back(&ode.nodes[(x + 1) * ode.MapHeight + (y + 0)]);

		}

	// Manually positio the start and end markers so they are not nullptr
	ode.nodeStart = &ode.nodes[(enemyPos._grid_x * ode.MapHeight) + enemyPos._grid_y];
	ode.nodeEnd = &ode.nodes[(Factory::Instance()[player].Get<Com_TilePosition>()._grid_x * ode.MapHeight) + (Factory::Instance()[player].Get<Com_TilePosition>()._grid_y)];
	/*ode.nodeStart->x = enemyPos._grid_x;
	ode.nodeStart->y = enemyPos._grid_y;
	ode.nodeEnd->x = 0;
	ode.nodeEnd->y = 0;*/

}

void Solve_AStar(Com_Node& ode, Com_TilePosition& enemyPos)
{
	static float alarm = 0;
	
	// Reset Navigation Graph - default all node states
	for (int y = 0; y < ode.MapHeight; y++)
		for (int x = 0; x < ode.MapWidth; x++)
		{
			ode.nodes[x * ode.MapHeight + y].bVisited = false;
			ode.nodes[x * ode.MapHeight + y].fGlobalGoal = INFINITY;
			ode.nodes[x * ode.MapHeight + y].fLocalGoal = INFINITY;
			ode.nodes[x * ode.MapHeight + y].parent = nullptr;	// No parents
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
	// which have not yet been explored. However, we will also stop  oo
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
			alarm += _dt;
			if(nodeNeighbour->parent != nullptr && alarm > 20.0f)
			{
				enemyPos._grid_x = nodeNeighbour->parent->x; // with this code is just teleporting but at least following 
				enemyPos._grid_y = nodeNeighbour->parent->y; // only some block it go diagonal, maybe due to the empty hole in the tilemap
				
				enemyPos._grid_x = nodeNeighbour->x; // to make sure it can reach the player
				enemyPos._grid_y = nodeNeighbour->y; 

				alarm = 0;
				/*std::cout << "Checked" << std::endl;
				std::cout << "nodeNeighbour->parent->x & y " << nodeNeighbour->parent->x << " + ";
				std::cout << nodeNeighbour->parent->y << std::endl;
				std::cout << "enemypos x & y " << enemyPos._grid_x << " + ";
				std::cout << enemyPos._grid_y << std::endl;
				std::cout << std::endl;*/

			}
			else if (nodeNeighbour->parent == nullptr && alarm > 20.0f)
			{
				enemyPos._grid_x = nodeNeighbour->x; // to make sure it can reach the player
				enemyPos._grid_y = nodeNeighbour->y;
				alarm = 0;
				/*std::cout << "Not Checked" << std::endl;
				std::cout << "enemypos x & y " << enemyPos._grid_x << " + ";
				std::cout << enemyPos._grid_y << std::endl;
				std::cout << std::endl;*/
			}
		}
	}
}

};


struct Com_Particle {
	size_t lifetime{ 2 };
};

struct Sys_ParticleSys : public System {
	void UpdateComponent() override {
		Com_Particle& particle = get<Com_Particle>();
		Com_GameTimer& timer = get<Com_GameTimer>();
		//if the particle reaches the end of it's short life 
		if (timer.timerinseconds == particle.lifetime)
		{
			RemoveEntity();
		}
	}
};


struct Com_ParticleEmitter {
	size_t timeforemitter{ 5 };
	size_t numberofparticle{ 20 };
	bool active{true};
};



struct Sys_ParticleEmitter : public System {
	void UpdateComponent() override {
		Com_GameTimer& timer = get<Com_GameTimer>();
		Com_ParticleEmitter& emitter = get<Com_ParticleEmitter>();
		Com_Position& position = get<Com_Position>();
		//if timer reaches 0 emit particles 
		if (emitter.active == true) {
			if (timer.timerinseconds == emitter.timeforemitter)
			{
				for (int i{ 0 }; i < emitter.numberofparticle; ++i) {
					//create particles 
					emitparticle();
				}
				timer.timerinseconds = 0;
				RemoveEntity();
			}
		}
	}

	void emitparticle() {
		//create particle sprite 
		float min{-50.0f };
		float max{ 50.0f };
		//create random sprite data 
		float rand_sizex = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - ((min)))));
		float rand_sizey = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - ((min)))));
		float rand_velocityx = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - ((min)))));
		float rand_velocityy = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - ((min)))));
		Factory::SpriteData data{ "test", rand_sizex, rand_sizey, 2, 3, 8, 0.15f };
		//Factory::SpriteData data = { "test3", 1,8, 8, 0.1f, rand_sizex, rand_sizey };
		//create particle 
		Factory::Instance().FF_CreateParticle(data, get<Com_Position>().x, get<Com_Position>().y, rand_velocityx ,rand_velocityy);
	}
};



struct Com_Health {
	size_t health{ 3 };
};

struct Sys_HealthUpdate : public System {
	void UpdateComponent() override {
		Com_Health& health = get<Com_Health>();
		//if no more health remove entity 
		if (health.health == 0) {
			RemoveEntity();
		}
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
		Com_Sprite& sprite = get<Com_Sprite>();
		if (surface._parent_surface) {
			// set self active to parent active
			if (surface._parent_active != surface._parent_surface->_active) {
				surface._active = surface._parent_surface->_active;
				surface._parent_active = surface._parent_surface->_active;
			}
			//surface._active = surface._parent_surface->_active ? surface._active : surface._parent_surface->_active;
			surface._n_position.x = surface._parent_surface->_n_position.x + surface._parent_surface->_dimensions.x * (surface._position.x - 0.5f);
			surface._n_position.y = surface._parent_surface->_n_position.y + surface._parent_surface->_dimensions.y * (surface._position.y - 0.5f);
			// offset with parent
			position.x = surface._parent_position->x - surface._parent_surface->_ph_dimensions.x + surface._position.x*surface._parent_surface->_ph_dimensions.x*2.0f;
			position.y = surface._parent_position->y + surface._parent_surface->_ph_dimensions.y - surface._position.y*surface._parent_surface->_ph_dimensions.y*2.0f;
		}
		else {
			// update sprite position with button
			surface._n_position = surface._position;
			position.x = (surface._position.x - 0.5f) * _screen_width;
			position.y = -((surface._position.y - 0.5f) * _screen_height);
		}
		sprite._visible = surface._active;
	}
};

struct Sys_GUISurfaceMouseCheck : public System {
	Vec2i _mouse_position{ 0,0 };
	Vec2f _screen_dimensions{ AEGetWindowWidth() / 2.0f,AEGetWindowHeight() / 2.0f };
	void OncePerFrame() override {
		AEInputGetCursorPosition(&_mouse_position.x, &_mouse_position.y);
		_mouse_position.x -= _screen_dimensions.x;
		_mouse_position.y -= _screen_dimensions.y;
		_mouse_position.y *= -1;
	}
	void UpdateComponent() override {
		Com_GUIMouseCheck& mouse = get<Com_GUIMouseCheck>();
		Com_GUISurface& surface = get<Com_GUISurface>();
		if (!surface._active) { return; }
		Com_Position& position = get<Com_Position>();
		// do bounding box check
		mouse._over = !(_mouse_position.x < position.x - surface._ph_dimensions.x || _mouse_position.x > position.x + surface._ph_dimensions.x ||
			_mouse_position.y < position.y - surface._ph_dimensions.y || _mouse_position.y > position.y + surface._ph_dimensions.y);
	}
};

struct Sys_GUISurfaceOnClick : public System {
	bool _left_mouse{ false };
	void OncePerFrame() override {
		_left_mouse = AEInputCheckTriggered(AEVK_LBUTTON);
	}
	void UpdateComponent() override {
		Com_GUIOnClick& on_click = get<Com_GUIOnClick>();
		Com_GUIMouseCheck& mouse = get<Com_GUIMouseCheck>();
		Com_GUISurface& surface = get<Com_GUISurface>();
		if (!surface._active) { return; }
		// do bounding box check
		if (mouse._over && _left_mouse) { 
			on_click._click_event(&surface);
		}
	}
};

struct Sys_GUITextRender : public System {
	char str_buffer[100];
	void UpdateComponent() override {
		Com_Position& position = get<Com_Position>();
		Com_GUISurface& surface = get<Com_GUISurface>();
		Com_Text& text = get<Com_Text>();
		if (!surface._active) {
			return;
		}
		Draw(position, text, surface);
		text._data._position.x = surface._n_position.x * 2.0f - 1.0f;
		text._data._position.y = -(surface._n_position.y * 2.0f - 1.0f);
		ResourceManager::Instance().DrawStackText(text._data);
	}
	void Draw(const Com_Position& position, Com_Text& text, const Com_GUISurface& surface) {
		//sprintf_s(str_buffer, text._text.c_str());
		//AEGfxGetPrintSize(text._font, str_buffer, 1.0f, text._width, text._height);
		//AEGfxPrint(text._font, const_cast<s8*>(text._text.c_str()), surface._position.x*2.0f-1.0f, -(surface._position.y*2.0f-1.0f), text._scale, 1.0f, text._g, text._b);
	}
};

struct Sys_GUIDrag : public System {
	bool _left_mouse{ false };
	bool _left_mouse_triggered{ false };
	Vec2f _n_mouse_position{ 0.0f,0.0f };
	Vec2f _click_position{ 0.0f,0.0f };
	Vec2f _offset{ 0.0f,0.0f };
	void OncePerFrame() override {
		_left_mouse = AEInputCheckCurr(AEVK_LBUTTON);
		_left_mouse_triggered = AEInputCheckTriggered(AEVK_LBUTTON);
		int x, y;
		AEInputGetCursorPosition(&x, &y);
		_n_mouse_position = { (float)x/(float)AEGetWindowWidth(), (float)y/(float)AEGetWindowHeight() };
		if (_left_mouse_triggered) {
			_click_position = _n_mouse_position;
		}
		if (_left_mouse) {
			_offset = _n_mouse_position - _click_position;
		}
		else {
			_offset = { 0.0f,0.0f };
		}
	}
	void UpdateComponent() override {
		Com_GUIMouseCheck& check = get<Com_GUIMouseCheck>();
		Com_GUIDrag& drag = get<Com_GUIDrag>();
		Com_GUISurface& surface = get<Com_GUISurface>();
		if (check._over) {
			if (_left_mouse_triggered) {
				drag._held = true;
				drag._click_position = surface._position;
			}
		}
		// if released free all drag
		if (!_left_mouse) {
			drag._held = false;
			drag._click_position = { 0.0f,0.0f };
		}
		if (drag._held && !surface._parent_surface) {
			surface._position = drag._click_position + _offset;
		}
	}
};
