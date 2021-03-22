#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <memory>
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
	Com_BoundingBox* aabb{ nullptr };
	Com_Velocity* vel{ nullptr };
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
		AlienMelee, //melee
		AlienRange  //range
	};
	size_t Alientype{ 0 };
};


/*																Component::PATH FINDING
____________________________________________________________________________________________________*/
struct PathFinding
{
	bool bObstacle = false;					// Is the node an obstruction?
	bool bVisited = false;					// Have we searched this node before?
	float fGlobalGoal = 0.0f;				// Distance to goal so far
	float fLocalGoal = 0.0f;				// Distance to goal if we took the alternative route
	Vec2i gridPos{0,0};
	vector<PathFinding*> vecNeighbours;	// Connections to neighbours
	PathFinding* parent = nullptr;		// Node connecting to this node that offers shortest parent

	~PathFinding()
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
	//PathFinding* nodeStart = nullptr;
	//PathFinding* nodeEnd = nullptr;
	////vector<Com_PathFinding> nodes;
	//int MapWidth = 0;
	//int MapHeight = 0;
	//PathFinding* nodes = nullptr;
	//~Com_Node()
	//{
	//	delete nodes;
	//	delete nodeStart;
	//	delete nodeEnd;
	//}
	Com_Node()
	{}
	Com_Node(const Vec2i& gridPos, bool obstacle = false, const Vec2f& worldPos = { 0.0f,0.0f })
		:
		_grid_pos(gridPos),
		_world_pos(worldPos),
		_obstacle(obstacle)
	{}
	bool operator>(Com_Node& rhs) {
		return rhs.FCost() < FCost();
	}
	int operator-(const Com_Node& rhs) const {
		int distance_x = abs(_grid_pos.x - rhs._grid_pos.x);
		int distance_y = abs(_grid_pos.y - rhs._grid_pos.y);
		if (distance_x > distance_y) {
			return distance_y * 14 + (distance_x - distance_y) * 10;
		}
		return distance_x * 14 + (distance_y - distance_x) * 10;
	}

	int		_g_cost = 0, _h_cost = 0;
	Vec2i	_grid_pos{ 0,0 };
	Vec2f	_world_pos{ 0.0f,0.0f };
	bool	_obstacle{ false };
	Com_Node* _parent{ nullptr };
	bool	_closed{ false };
	bool	_open{ false };

	int FCost() { return _g_cost + _h_cost; }
};

struct Grid {
	Grid() = default;
	Grid(int width, int height, const std::vector<int> grid)
		:
		_width(width),
		_height(height)
	{
		for (size_t y = 0; y < height; ++y) {
			for (size_t x = 0; x < width; ++x)
				_grid.emplace_back(Vec2i((int)x, (int)y), !grid[x * height + y]); {
			}
		}
	}
	size_t _width{ 0 };
	size_t _height{ 0 };
	vector<Com_Node> _grid;
	Com_Node& Get(const Vec2i& pos) {
		return _grid[pos.y * _width + pos.x];
	}
	void GetNeighbours(Com_Node*& node, std::vector<Com_Node*>& neighbours) {
		neighbours.clear();
		if (node->_grid_pos.x - 1 >= 0) {
			neighbours.push_back(&Get({ node->_grid_pos.x - 1, node->_grid_pos.y }));
		}
		if (node->_grid_pos.x + 1 < _width) {
			neighbours.push_back(&Get({ node->_grid_pos.x + 1, node->_grid_pos.y }));
		}
		if (node->_grid_pos.y - 1 >= 0) {
			neighbours.push_back(&Get({ node->_grid_pos.x, node->_grid_pos.y - 1 }));
		}
		if (node->_grid_pos.y + 1 < _height) {
			neighbours.push_back(&Get({ node->_grid_pos.x, node->_grid_pos.y + 1 }));
		}
	}
};

struct Com_FindPath {
	bool	_find	{ false };
	bool	_found	{ false };
	Vec2i	_start	{ 0,0 };
	Vec2i	_end	{ 0,0 };
	Vec2i	_next	{ 100, 100 }; // initailized to make sure is out of game board
};

struct Com_Health {
	size_t health{ 3 };
};

struct Sys_HealthUpdate : public System {
	void UpdateComponent() override {
		Com_Health& health = get<Com_Health>();
		
		//if no more health remove entity 
		if (health.health <= 0) {
			RemoveEntity();
			std::cout << "U die" << std::endl;
		}
	}
};

/*																				system::ENEMY STATES
____________________________________________________________________________________________________*/
struct Com_EnemyStateOne {
	//
	enum class STATES {
		IDLE,
		MOVE,
		ATTACK,
		EVILWIN
	} _current_state{ STATES::IDLE };
	int _speed{ 2 };
	int _counter{ _speed };
	Com_TilePosition* _player;
	Com_Health* playerHealth; 
};

struct Sys_EnemyStateOne : public System {
	float _turn_step{ 0.5f };
	float _turn_step_counter{ _turn_step };
	bool  _turn{ false };
	void OncePerFrame() override {
		_turn_step_counter -= _dt;
		if (_turn_step_counter > 0.0f) {
			_turn = false;
		}
		else {
			_turn = true;
			_turn_step_counter = _turn_step;
		}
	}
	void UpdateComponent() override {
		Com_EnemyStateOne& state = get<Com_EnemyStateOne>();
		if (_turn) {
			--state._counter;
		}
		(this->*_fp_states[static_cast<int>(state._current_state) * 3 + 1])();
		if (!state._counter) {
			state._counter = state._speed;
		}
	}
	void ChangeState(Com_EnemyStateOne::STATES newState) {
		Com_EnemyStateOne& state = get<Com_EnemyStateOne>();
		// exit current state
		(this->*_fp_states[static_cast<int>(state._current_state) * 3 + 2])();
		// enter new state
		(this->*_fp_states[static_cast<int>(newState) * 3])();
		state._current_state = newState;
	}
	// idle
	void IDLE_ENTER() {
		std::cout << "IDLE_ENTER" << std::endl;
	}
	void IDLE_UPDATE() {
		Com_EnemyStateOne& state = get<Com_EnemyStateOne>();
		Com_FindPath& fp = get<Com_FindPath>();
		Com_TilePosition& pos = get<Com_TilePosition>();
		if (!state._counter) {
			std::cout << "IDLE_UPDATE" << std::endl;
			// see if can find path to player
			fp._start = Vec2i(pos._grid_x, pos._grid_y);
			fp._end = Vec2i(state._player->_grid_x, state._player->_grid_y);
			fp._find = true;
			// if path found
			if (fp._found) {
				ChangeState(Com_EnemyStateOne::STATES::MOVE);
			}
		}
	}
	void IDLE_EXIT() {
		std::cout << "IDLE_EXIT" << std::endl;
	}
	// move
	void MOVE_ENTER() {
		std::cout << "MOVE_ENTER" << std::endl;
	}
	void MOVE_UPDATE() {
		Com_EnemyStateOne& state = get<Com_EnemyStateOne>();
		Com_FindPath& fp = get<Com_FindPath>();
		Com_TilePosition& pos = get<Com_TilePosition>();
		if (!state._counter) {
			std::cout << "MOVE_UPDATE" << std::endl;
			// see if can find path to player
			fp._start = Vec2i(pos._grid_x, pos._grid_y);
			fp._end = Vec2i(state._player->_grid_x, state._player->_grid_y);
			fp._find = true;

			//if next path is the player
			//std::cout << "x: " << fp._next.x << "y: " << fp._next.y << std::endl;
			if (fp._next.x == fp._end.x && fp._next.y == fp._end.y) 
			{
				ChangeState(Com_EnemyStateOne::STATES::ATTACK);
			}
			//std::cout << "x: " << pos._grid_x << " & y: " << pos._grid_y << std::endl;
			// if path found
			/*if (fp._next.x != -1 && fp._next.y != -1) {
				pos._grid_x = fp._next.x;
				pos._grid_y = fp._next.y;
				std::cout << "x: " << fp._next.x << "y: " << fp._next.y << std::endl;
			}*/
		}
		
	}
	void MOVE_EXIT() {
		std::cout << "MOVE_EXIT" << std::endl;
	}
	// attack
	void ATTACK_ENTER() {
		std::cout << "ATTACK_ENTER" << std::endl;
	}
	void ATTACK_UPDATE() {
		Com_EnemyStateOne& state = get<Com_EnemyStateOne>();
		Com_FindPath& fp = get<Com_FindPath>();
		Com_TilePosition& pos = get<Com_TilePosition>();
		if (!state._counter) 
		{
			std::cout << "ATTACK_UPDATE" << std::endl;
			//std::cout << "x: " << fp._next.x << "y: " << fp._next.y << std::endl;
			// see if can find path to player
			fp._start = Vec2i(pos._grid_x, pos._grid_y);
			fp._end = Vec2i(state._player->_grid_x, state._player->_grid_y);
			fp._find = true;

			if (fp._next.x != fp._end.x && fp._next.y != fp._end.y)
			{
				ChangeState(Com_EnemyStateOne::STATES::MOVE);
			}
			else if (fp._next.x == fp._end.x && fp._next.y == fp._end.y)
			{
				// to decrease health
				if (state.playerHealth != nullptr) 
				{
					--state.playerHealth->health;
					if (state.playerHealth->health == 0) 
					{
						std::cout << "お前もう死んで " << std::endl;
						std::cout << "何？" << std::endl;
						ChangeState(Com_EnemyStateOne::STATES::EVILWIN);
					}
				}

			}
			
		}
	}
	void ATTACK_EXIT() {
		std::cout << "ATTACK_EXIT" << std::endl;
	}
	void EVILWIN_ENTER() {
		std::cout << "EVIL_ENTER" << std::endl;
	}
	void EVILWIN_UPDATE() {
		std::cout << "EVIL_UPDATE" << std::endl;
		std::cout << "MUHAHAHAHAHAHAHAH!!!!" << std::endl;
	}
	void EVILWIN_EXIT() {
		std::cout << "EVIL_EXIT" << std::endl;
	}

	using FP_STATES = void(Sys_EnemyStateOne::*)();
	FP_STATES _fp_states[12] = { &Sys_EnemyStateOne::IDLE_ENTER, &Sys_EnemyStateOne::IDLE_UPDATE, &Sys_EnemyStateOne::IDLE_EXIT,
								&Sys_EnemyStateOne::MOVE_ENTER, &Sys_EnemyStateOne::MOVE_UPDATE, &Sys_EnemyStateOne::MOVE_EXIT,
								&Sys_EnemyStateOne::ATTACK_ENTER, &Sys_EnemyStateOne::ATTACK_UPDATE, &Sys_EnemyStateOne::ATTACK_EXIT,
								&Sys_EnemyStateOne::EVILWIN_ENTER, &Sys_EnemyStateOne::EVILWIN_UPDATE, &Sys_EnemyStateOne::EVILWIN_EXIT };
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
		get<Com_Sprite>()._render_pack._layer = static_cast<int>(-get<Com_Position>().y);
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
	Factory::SpriteData data = { "test", 20, 20, 1, static_cast<int>(100.0f), static_cast<int>(100.0f), static_cast<int>(100.0f) };
	//passing in of player's data 
	virtual void CreateProjectile(Com_Direction& direction,Com_Position& position) {
		//calling the factory fnc
		Factory::Instance().FF_Createproj(data, static_cast<int>(position.x), static_cast<int>(position.y),direction);
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
					//sword_attack(direction, position);

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
					//sword_attack(direction, position);

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
					//sword_attack(direction, position);

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
					//sword_attack(direction, position);

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
	//void sword_attack(Com_Direction& direction, Com_Position& position) {
	//	//pending 
	//
	//}
	void Plant_Bomb(Com_Position& position) {
		//setting the sprite data to pass in 
		Factory::SpriteData data{ "kaboom", 40.0f, 40.0f, 1, 1, 1, 0.15f };
		//creating the bomb 
		Factory::Instance().FF_CreateBomb(data, static_cast<int>(position.x),static_cast<int>(position.y));
	}
};

struct Sys_Projectile2 : public System {
	void UpdateComponent() override {
		Com_Projectile& proj = get<Com_Projectile>();
		if (AEGetTime(nullptr) - proj.time > AEFrameRateControllerGetFrameTime() * 10)
		{
			proj.time = static_cast<float>(AEGetTime(nullptr));
			Com_TilePosition& tileposition = get<Com_TilePosition>();
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
	}
};

 /////////Edits  

/*-------------------------------------
//for spawning of enemies 
-------------------------------------------*/
struct Com_EnemySpawn{
	size_t numberofenemies{ 2 }; //number of enemies to spawn
	size_t CurrNoOfEnemies{ 0 }; //keep track of enemies on map
	size_t DEATHEnemiespawncounter{ 0 };
};

struct Com_Wave{
	size_t timerforwave{ 3 }; //if timer hits 0 in secsm spawn new wave 
	size_t numberofwaves{ 2 }; //if number of wave hit 0, level unlocked 
};

//logic for spawning of enemies 
struct Sys_EnemySpawning : public System {
	// Initialization
	eid _tilemap = { -1 };
	eid playerpos = -1;
	float timer = 3;
	void OncePerFrame() override
	{
		timer -= _dt;
	}
	void UpdateComponent() override {
		static Com_EnemySpawn& Enemyspawn = get<Com_EnemySpawn>();
		Com_Wave& wave = get<Com_Wave>();
		int i = 0;

		//if the timer hits for set time 
		//if timer hit 0 spawn wave/ number of enemies hit 0 

		if (timer < 0 || Enemyspawn.DEATHEnemiespawncounter > 1)
		{
			if (Enemyspawn.CurrNoOfEnemies < 5) 
			{
				while (i < Enemyspawn.numberofenemies)
				{
					int randomx = rand() % 9;
					int randomy = rand() % 5;
					Factory::SpriteData data1{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.25f };
					eid enemy = Factory::Instance().FF_CreateEnemy(data1, _tilemap, randomx, randomy);
					Factory::Instance()[enemy].Get<Com_EnemyStateOne>()._player = &Factory::Instance()[playerpos].Get<Com_TilePosition>();
					Factory::Instance()[enemy].Get<Com_EnemyStateOne>().playerHealth = &Factory::Instance()[playerpos].Get<Com_Health>();
					++Enemyspawn.CurrNoOfEnemies;
					++i;
					timer = 5;
					--wave.numberofwaves; //decrease the number of waves left 
				}
					
			}


		}
		else
		{
			i = 0;
			
		}
	}


};


/*-------------------------------------
			//for attack of enemies 
-------------------------------------------*/

//logic for attack of enemies 
struct Sys_EnemyAttack : public System {
	//void UpdateComponent() override {
	//	bool _initialized{false};
	//	if (_initialized) {
	//		Com_FindPath& fp = get<Com_FindPath>();
	//		Com_TilePosition& tpos = get<Com_TilePosition>();
	//		//if enemy is melee
	//		if (get<Com_type>().type == 1) // 1 == enemy
	//		{
	//			if (tpos._grid_x == fp._end.x && tpos._grid_y == fp._end.y)
	//			{
	//				tpos._grid_x
	//			}
	//			//check 4 sides if player is 1 tile away
	//			//if () {
	//			//	//shoot invisible that direction 
	//			//	//create projectile system
	//			//}
	//		}
	//	}
	//	////if enemy is range
	//	//if (get<Com_TypeEnemy>().Alientype == Com_TypeEnemy::AlienRange) {
	//	//	//check 4 sides if player is x/y aligned 
	//	//	//if () {
	//	//	//	//shoot that direction
	//	//	//	//createprojectile system
	//	//	//}
	//	//}
	//}
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
		if (_initialized) {
			Com_FindPath& fp = get<Com_FindPath>();
			Com_TilePosition& tpos = get<Com_TilePosition>();
			static int i = 0;
			if (fp._find) {
				fp._found = SolveAStar(fp._start, fp._end, _grid, _path);
				if (fp._found && _path.size() >= 1) {
					
						if (fp._next.x != fp._end.x && fp._next.y != fp._end.y) 
						{
							tpos._grid_x = _path[0].x;
							tpos._grid_y = _path[0].y;
							//std::cout << "Enemy: " << tpos._grid_x << " & " << tpos._grid_y << std::endl;
							//std::cout << "player: " << fp._end.x << " & " << fp._end.y << std::endl;
							if (i < _path.size())
							{
							fp._next.x = _path[0].x;
							fp._next.y = _path[0].y;
							//std::cout << "からの" << std::endl;
							//std::cout << "fp.next: " << fp._next.x << " + " << fp._next.y << std::endl;
							}
						}
						else 
						{
							fp._next.x = _path[0].x;
							fp._next.y = _path[0].y;
						}
					
					
					
				}
				fp._find = false;
			}
		}

	}
	Grid _grid;
	vector<Com_Node*> _nodes_to_reset;		// rmb to reserve, PESSIMISM! or something like that
	vector<Com_Node*> _neighbours;			// rmb to reserve
	vector<Vec2i> _path;
	bool _initialized{ false };

	bool SolveAStar(const Vec2i& start, const Vec2i& end, Grid& grid, std::vector<Vec2i>& path) {
		path.clear();
		// custom comparator
		auto cmp = [](Com_Node*& node1, Com_Node*& node2) {return *node1 > * node2; };
		// create min heap
		std::priority_queue<Com_Node*, vector<Com_Node*>, decltype(cmp)> min_heap(cmp);

		// create start and end temp nodes
		Com_Node* start_node = &grid.Get(start);
		Com_Node* end_node = &grid.Get(end);

		// add start node to the open set
		min_heap.push(start_node);
		start_node->_open = true;
		_nodes_to_reset.push_back(start_node);

		// loop
		while (min_heap.size() > 0) {
			Com_Node* current_node = min_heap.top();

			// erase current node from open set and add to closed set
			min_heap.pop();
			current_node->_open = false;
			current_node->_closed = true;
			_nodes_to_reset.push_back(current_node);

			// if current == end, reached
			if (current_node == end_node) {
				RetracePath(start_node, end_node, path);
				return true;
			}

			// find/update fcost of neighbours and add them to open set
			grid.GetNeighbours(current_node, _neighbours);
			for (auto& n : _neighbours) {
				// if obstacle or closed skip
				if (n->_obstacle || n->_closed) {
					continue;
				}
				// if new g cost < g cost (need updating) || or if not in open, calculate f cost, add to open
				int new_g_cost = current_node->_g_cost + (*current_node - *n);
				// check
				bool in_open = n->_open;
				if (new_g_cost < n->_g_cost || !in_open) {
					// set fcost
					n->_g_cost = new_g_cost;
					n->_h_cost = (*n - *end_node);
					// set parent node
					n->_parent = current_node;
					if (!in_open) {
						min_heap.push(n);
						n->_open = true;
					}
					_nodes_to_reset.push_back(n);
				}
			}
		}
		for (auto& n : _nodes_to_reset) {
			ResetNode(n);
		}
		_nodes_to_reset.clear();
		return false;
	}
	void RetracePath(const Com_Node* start, const Com_Node* end, vector<Vec2i>& path) {
		path.clear();
		Com_Node const* current = end;
		while (start != current) {
			path.push_back(current->_grid_pos);
			current = current->_parent;
		}
		std::reverse(path.begin(), path.end());
		for (auto& n : _nodes_to_reset) {
			ResetNode(n);
		}
		_nodes_to_reset.clear();
	}
	void ResetNode(Com_Node*& node) {
		node->_g_cost = 0;
		node->_h_cost = 0;
		node->_closed = false;
		node->_open = false;
	}

};


//// AUSTEN SEE START
//namespace Pathfinding {
//	struct Node {
//		Node(const Vec2i& gridPos, int obstacle = 0, const Vec2f& worldPos = { 0.0f,0.0f })
//			:
//			_grid_pos(gridPos),
//			_world_pos(worldPos),
//			_obstacle(obstacle)
//		{}
//		bool operator>(Node& rhs) {
//			return rhs.FCost() < FCost();
//		}
//		int operator-(const Node& rhs) const {
//			int distance_x = abs(_grid_pos.x - rhs._grid_pos.x);
//			int distance_y = abs(_grid_pos.y - rhs._grid_pos.y);
//			if (distance_x > distance_y) {
//				return distance_y * 14 + (distance_x - distance_y) * 10;
//			}
//			return distance_x * 14 + (distance_y - distance_x) * 10;
//		}
//
//		int		_g_cost = 0, _h_cost = 0;
//		Vec2i	_grid_pos{ 0,0 };
//		Vec2f	_world_pos{ 0.0f,0.0f };
//		bool	_obstacle{ false };
//		Node* _parent{ nullptr };
//		bool	_closed{ false };
//		bool	_open{ false };
//
//		int FCost() { return _g_cost + _h_cost; }
//	};
//	struct Grid {
//		Grid() = default;
//		Grid(int width, int height, const std::vector<int> grid)
//			:
//			_width(width),
//			_height(height)
//		{
//			for (size_t y = 0; y < height; ++y) {
//				for (size_t x = 0; x < width; ++x)
//					_grid.emplace_back(Vec2i((int)x, (int)y), !grid[x * height + y]); {
//				}
//			}
//		}
//		size_t _width{ 0 };
//		size_t _height{ 0 };
//		std::vector<Node> _grid;
//		Node& Get(const Vec2i& pos) {
//			return _grid[pos.y * _width + pos.x];
//		}
//		void GetNeighbours(Node*& node, std::vector<Node*>& neighbours) {
//			neighbours.clear();
//			if (node->_grid_pos.x - 1 >= 0) {
//				neighbours.push_back(&Get({ node->_grid_pos.x - 1, node->_grid_pos.y }));
//			}
//			if (node->_grid_pos.x + 1 < _width) {
//				neighbours.push_back(&Get({ node->_grid_pos.x + 1, node->_grid_pos.y }));
//			}
//			if (node->_grid_pos.y - 1 >= 0) {
//				neighbours.push_back(&Get({ node->_grid_pos.x, node->_grid_pos.y - 1 }));
//			}
//			if (node->_grid_pos.y + 1 < _height) {
//				neighbours.push_back(&Get({ node->_grid_pos.x, node->_grid_pos.y + 1 }));
//			}
//		}
//	};
//}
//
//struct Sys_Pathfinding_v2 : public System {
//	//
//	void UpdateComponent() override {
//		if (_initialized) {
//			Com_FindPath& fp = get<Com_FindPath>();
//			Com_TilePosition& tpos = get<Com_TilePosition>();
//			if (fp._find) {
//				fp._found = SolveAStar(fp._start, fp._end, _grid, _path);
//				if (fp._found && _path.size() >= 1) {
//					tpos._grid_x = _path[0].x;
//					tpos._grid_y = _path[0].y;
//				}
//				fp._find = false;
//			}
//		}
//	}
//	Pathfinding::Grid _grid;
//	std::vector<Pathfinding::Node*> _nodes_to_reset;		// rmb to reserve, PESSIMISM! or something like that
//	std::vector<Pathfinding::Node*> _neighbours;			// rmb to reserve
//	std::vector<Vec2i> _path;
//	bool _initialized{ false };
//
//	bool SolveAStar(const Vec2i& start, const Vec2i& end, Pathfinding::Grid& grid, std::vector<Vec2i>& path) {
//		path.clear();
//		// custom comparator
//		auto cmp = [](Pathfinding::Node*& node1, Pathfinding::Node*& node2) {return *node1 > *node2; };
//		// create min heap
//		std::priority_queue<Pathfinding::Node*, std::vector<Pathfinding::Node*>, decltype(cmp)> min_heap(cmp);
//
//		// create start and end temp nodes
//		Pathfinding::Node* start_node = &grid.Get(start);
//		Pathfinding::Node* end_node = &grid.Get(end);
//
//		// add start node to the open set
//		min_heap.push(start_node);
//		start_node->_open = true;
//		_nodes_to_reset.push_back(start_node);
//
//		// loop
//		while (min_heap.size() > 0) {
//			Pathfinding::Node* current_node = min_heap.top();
//
//			// erase current node from open set and add to closed set
//			min_heap.pop();
//			current_node->_open = false;
//			current_node->_closed = true;
//			_nodes_to_reset.push_back(current_node);
//
//			// if current == end, reached
//			if (current_node == end_node) {
//				RetracePath(start_node, end_node, path);
//				return true;
//			}
//
//			// find/update fcost of neighbours and add them to open set
//			grid.GetNeighbours(current_node, _neighbours);
//			for (auto& n : _neighbours) {
//				// if obstacle or closed skip
//				if (n->_obstacle || n->_closed) {
//					continue;
//				}
//				// if new g cost < g cost (need updating) || or if not in open, calculate f cost, add to open
//				int new_g_cost = current_node->_g_cost + (*current_node - *n);
//				// check
//				bool in_open = n->_open;
//				if (new_g_cost < n->_g_cost || !in_open) {
//					// set fcost
//					n->_g_cost = new_g_cost;
//					n->_h_cost = (*n - *end_node);
//					// set parent node
//					n->_parent = current_node;
//					if (!in_open) {
//						min_heap.push(n);
//						n->_open = true;
//					}
//					_nodes_to_reset.push_back(n);
//				}
//			}
//		}
//		for (auto& n : _nodes_to_reset) {
//			ResetNode(n);
//		}
//		_nodes_to_reset.clear();
//		return false;
//	}
//	void RetracePath(const Pathfinding::Node* start, const Pathfinding::Node* end, std::vector<Vec2i>& path) {
//		path.clear();
//		Pathfinding::Node const* current = end;
//		while (start != current) {
//			path.push_back(current->_grid_pos);
//			current = current->_parent;
//		}
//		std::reverse(path.begin(), path.end());
//		for (auto& n : _nodes_to_reset) {
//			ResetNode(n);
//		}
//		_nodes_to_reset.clear();
//	}
//	void ResetNode(Pathfinding::Node*& node) {
//		node->_g_cost = 0;
//		node->_h_cost = 0;
//		node->_closed = false;
//		node->_open = false;
//	}
//};
//// AUSTEN SEE END

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
		Factory::Instance().FF_CreateParticle(data, static_cast<int>(get<Com_Position>().x), static_cast<int>(get<Com_Position>().y), rand_velocityx ,rand_velocityy);
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
		_mouse_position.x -= (int)_screen_dimensions.x;
		_mouse_position.y -= (int)_screen_dimensions.y;
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


//edits by wilfred
struct Com_Obstacle {
	enum obst {
		bombbarrel,
		breakablewall,
	};
	size_t obstacletype{ 0 };
	size_t numofhitstodestroy{ 1 };
};

struct Sys_Obstacle : public System {

	void UpdateComponent() override {
		Com_Obstacle& obstacle = get<Com_Obstacle>();
		//if it's a bomb barrel 
		if (obstacle.obstacletype == obstacle.bombbarrel) {
			//if hit, explode 
			if (obstacle.numofhitstodestroy == 0) {
				//explode 
			}
		}
		//if it's a breakable wall 
		if (obstacle.obstacletype == obstacle.breakablewall) {
			if (obstacle.numofhitstodestroy == 0) {
				//destroy wall, free space to walk on 
			}
		}
	}
};

struct Com_Camera {
	char _filler = 0;
	bool updated{ false };
	float boundariesxmax = 0.0f;
	float boundariesymax = 0.0f;
	float boundariesxmin = 0.0f;
	float boundariesymin = 0.0f;
	float buffer = 150.0f;
};

//camera 
struct Sys_Camera : public System {
	void UpdateComponent() override {
		//fix the camera to the player 
		Com_Position& pos = get<Com_Position>();
		Com_Camera& cam = get<Com_Camera>();
		if (cam.updated == false) {
			//seet camera postion
			AEGfxSetCamPosition(pos.x, pos.y);
			//recomput boundaries
			cam.boundariesxmax = pos.x + AEGetWindowWidth() / 2;
			cam.boundariesxmin = pos.x - AEGetWindowWidth() / 2;
			cam.boundariesymax = pos.y + AEGetWindowHeight() / 2;
			cam.boundariesymin = pos.y - AEGetWindowHeight() / 2;
			cam.updated = true;
		}
		if (pos.x + cam.buffer > cam.boundariesxmax || pos.y - cam.buffer < cam.boundariesymin || pos.x - cam.buffer < cam.boundariesxmin || pos.y + cam.buffer > cam.boundariesymax) {
			cam.updated = false;
		}
	}
};

//edits
struct Com_type {
	size_t type{ 0 };
	enum type {
		player,
		enemy,
		bullet,
		wall,
		bombbarrel,
		enemyrange
	};
};

struct Com_GridColData {
	Com_TilePosition* tilepos{ nullptr };
	Com_type* type{ nullptr };
	bool emplacedvec{ false };
};

//grid collision
struct Sys_GridCollision : public System {
	std::vector<Com_GridColData> GridCol; //to store all collision data of player
	void UpdateComponent() override {
		Com_type* type = &get<Com_type>();
		Com_TilePosition* tilepos = &get<Com_TilePosition>();
		Com_GridColData& gridcoldata = get<Com_GridColData>();
		Com_EnemySpawn& gridspaen = get<Com_EnemySpawn>();
		if (gridcoldata.emplacedvec == false) {
			GridCol.emplace_back(Com_GridColData{ tilepos,type });
			gridcoldata.emplacedvec = true;
		}
		for (size_t i{ 0 }; i < GridCol.size(); ++i) {
			if (gridcollisioncheck(*tilepos, *GridCol[i].tilepos)) {
				//range attack with enemy 
				if (type->type == type->enemy && GridCol[i].type->type == type->bullet) {
					std::cout << "Collided" << std::endl;
					RemoveEntity();
					++gridspaen.DEATHEnemiespawncounter;
					--gridspaen.CurrNoOfEnemies;
					std::cout << gridspaen.DEATHEnemiespawncounter << std::endl;

				}
				//range attack with enemy 
				if (type->type == type->enemy && GridCol[i].type->type == type->bullet) {
					std::cout << "Collided" << std::endl;
					RemoveEntity();
				}
				//range attack with enemy 
				if (type->type == type->enemy && GridCol[i].type->type == type->bullet) {
					std::cout << "Collided" << std::endl;
					RemoveEntity();
				}
				//testing
				//if player with enemy
				//if (type->type == type->player && GridCol[i].type->type == type->enemy) {
				//	RemoveEntity();
				//}
				//if enemy with player 
				if (type->type == type->enemy && GridCol[i].type->type == type->player) {
					std::cout << "Collided" << std::endl;
					RemoveEntity();
				}
				//enemy with bullet 
				if (type->type == type->enemy && GridCol[i].type->type == type->bullet) {
					std::cout << "Collided" << std::endl;
					RemoveEntity();
				}
			}
		}
	}
	bool gridcollisioncheck(const Com_TilePosition& tilepos1,const Com_TilePosition& tilepos2){
		if (tilepos1._grid_x == tilepos2._grid_x && tilepos1._grid_y == tilepos2._grid_y) {
			return true;
		}
		else {
			return false;
		}
	
	}
};
