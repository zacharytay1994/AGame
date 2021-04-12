/******************************************************************************/
/*!
\File Name		: Scene_Levels
\Project Name	: AGame
\Authors 		:
				Primary - Wilfred Ng (100%)
				Secondary -
\brief		Scene for normal levels. 3 Levels which are unlockable

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/

#pragma once
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include "ResourceManager.h"
#include "Inventory.h"
#include <string>
#include <sstream>

/*___________________________________________________________________
	Normal Levels 
_____________________________________________________________________*/
struct Level : public Scene
{
	/*
	Member Variables
	________________________________*/
	eid player = -1;
	eid enemytest = -1;
	eid tilemap = -1;
	eid lives{ -1 };
	eid waves{ -1 };
	eid spawner{ -1 };
	eid menu{ -1 };
	eid _WinOrLose{ -1 };
	eid wall{ -1 };
	eid bomb{ -1 };
	eid arrow = -1;
	eid bossy{ -1 };

	//sprite data 
	Factory::SpriteData box{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };
	Factory::SpriteData boom{ "kaboom", 40.0f, 40.0f, 1, 1, 1, 0.15f };
	Vec2i passin[5] = { {0,3},{4,7},{8,11},{0,0},{0,0} };
	Factory::SpriteData man{ "hero.png", 200.0f, 320.0f, 4, 3, 12, 0.1f, 0, passin };
	Factory::SpriteData underline{ "underline.png", 80.0f, 200.0f, 4, 1, 4, 0.25f };
	Vec2i passin2[5] = { {0,1},{2,3},{4,5},{6,7},{0,0} };
	Factory::SpriteData arrows{ "arrows.png", 50.0f, 50.0f, 3, 3, 8, 0.1f, -900, passin2 };
	Factory::SpriteData title{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Factory::SpriteData buttonsurface{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Vec2i passin3[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin3 };
	Com_Sprite* arrow_sprite{ nullptr };

	//current levels tracking 
	size_t levels{ 0 };
	bool once = false, checkBoss = false;
	bool skip = false;
	Inventory playerInv;

	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		//reset 
		once = false;
		checkBoss = false;
		//for cursor no display 
		SceneManager::Instance()._inlevel = true;
		SceneManager::Instance()._currentlyplaying = true;
		//init tilemap 
		tilemap = Factory::Instance().FF_Tilemap("tilemap", ResourceManager::Instance()._tilemap_names2[ResourceManager::Instance()._tilemap_id2]._binary + ".txt",
			ResourceManager::Instance()._tilemap_names2[ResourceManager::Instance()._tilemap_id2]._map + ".txt");
		//if it's level 1 
		if (ResourceManager::Instance()._tilemap_names2[ResourceManager::Instance()._tilemap_id2]._binary + ".txt" == "c_level1.txt") {
			levels = 1;
			SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->yammonsterenable = false; //disable purple monsters 
		}
		//if it's level 2 
		if (ResourceManager::Instance()._tilemap_names2[ResourceManager::Instance()._tilemap_id2]._binary + ".txt" == "c_level2.txt") {
			levels = 2;
		}
		//if it's level 3 
		if (ResourceManager::Instance()._tilemap_names2[ResourceManager::Instance()._tilemap_id2]._binary + ".txt" == "c_level3.txt") {
			levels = 3;
		}
		//if either level 1 or 2 
		if (levels == 1 || levels == 2) {
			SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->spawnBoss = false; //disable
		}
		//if either level 2 or 3 
		if (levels == 2 || levels == 3) {
			SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->yammonsterenable = true; //enable 
		}

		//continue init the correct tilemap 
		Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 3;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_tilemap = tilemap;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_tilemap = tilemap;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateBoss>()->_tilemap = tilemap;

		//create spawner
		spawner = Factory::Instance().FF_CreateSpawner();
		//init grid 
		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		pf2._initialized = true;
		SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_AABB>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_ArrowKeysTilemap>()->_grid = &pf2._grid;
		//init spawner 
		SystemDatabase::Instance().GetSystem<Sys_AABB>()->_spawner = &Factory::Instance()[spawner].Get<Com_EnemySpawn>();
		Factory::Instance()[spawner].Get<Com_Boss>().BossHealth = 20;
		SystemDatabase::Instance().GetSystem<Sys_AABB>()->Boss = &Factory::Instance()[spawner].Get<Com_Boss>();
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->boss = &Factory::Instance()[spawner].Get<Com_Boss>();
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateBoss>()->boss = &Factory::Instance()[spawner].Get<Com_Boss>();

		//testting for level editor 
		for (int y = 0; y < com_tilemap._height; ++y) {
			for (int x = 0; x < com_tilemap._width; ++x) {
				//if it's a player spawn location 
				if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 2) {
					//init player 
					player = Factory::Instance().FF_SpriteTile(man, tilemap, x, y);
					Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState, Com_type, Com_BoundingBox, Com_Velocity, Com_CollisionData>();
					Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
					Factory::Instance()[player].Get<Com_type>().type = 0; // set player type
					SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->tile = tilemap;
					SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
					SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;
					SystemDatabase::Instance().GetSystem<Sys_EnemyStateBoss>()->player = player;
					SystemDatabase::Instance().GetSystem<Sys_AABB>()->_PLayerHealth = &Factory::Instance()[player].Get<Com_Health>();
					SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->playerpos = player;

				}
				//if its' a destructible wall 
				if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 4) {
					wall = Factory::Instance().FF_SpriteTile(box, tilemap, x, y);
					Factory::Instance()[wall].AddComponent<Com_YLayering, Com_Health, Com_type, Com_BoundingBox, Com_Velocity, Com_CollisionData, Com_ParticleEmitter>();
					SystemDatabase::Instance().GetSystem<Sys_Obstacle>()->_grid = &pf2._grid;
					Entity& e = Factory::Instance()[wall];
					e.Get<Com_Health>().health = 3;
					e.Get<Com_type>().type = 3;
					continue;
				}
				//if it's a explosive barrel 
				if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 5) {
					bomb = Factory::Instance().FF_SpriteTile(boom, tilemap, x, y);
					Factory::Instance()[bomb].AddComponent<Com_YLayering, Com_type, Com_Health, Com_BoundingBox, Com_Velocity, Com_CollisionData, Com_ParticleEmitter, Com_GameTimer>();
					SystemDatabase::Instance().GetSystem<Sys_ParticleEmitter>()->tilemap = tilemap;
					SystemDatabase::Instance().GetSystem<Sys_Obstacle>()->_grid = &pf2._grid;
					Entity& e = Factory::Instance()[bomb];
					e.Get<Com_Health>().health = 1;
					e.Get<Com_type>().type = 4;
					continue;
				}
			}
		}


		//for arrow 
		arrow = Factory::Instance().FF_Sprite(arrows, 0.0f, 0.0f);
		Entity& a = Factory::Instance()[arrow];
		a.AddComponent<Com_ParentPosition>();
		a.Get<Com_ParentPosition>()._parent_id = player;
		arrow_sprite = &a.Get<Com_Sprite>();

		//for the number of lives of player left 
		lives = Factory::Instance().FF_CreateGUISurface(underline, 0.2f, 0.1f, 0.4f, 0.1f, 100);
		Factory::Instance().FF_CreateGUIChildSurfaceText(lives, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Lives: ", "courier");
		std::stringstream ss;
		ss << Factory::Instance()[player].Get<Com_Health>().health;
		lives = Factory::Instance().FF_CreateGUIChildSurfaceText(lives, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss.str().c_str(), "courier");
		//for the number of waves left 
		waves = Factory::Instance().FF_CreateGUISurface(underline, 0.8f, 0.1f, 0.4f, 0.1f, 100);
		std::stringstream ss1;
		ss1 << Factory::Instance()[spawner].Get<Com_Wave>().numberofwaves;
		waves = Factory::Instance().FF_CreateGUIChildSurfaceText(waves, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss1.str().c_str(), "courier");
		//for the boss health 
		bossy = Factory::Instance().FF_CreateGUISurface(underline, 0.8f, 0.1f, 0.4f, 0.1f, 100);
		std::stringstream ss2;
		ss2 << Factory::Instance()[spawner].Get<Com_Boss>().BossHealth;
		bossy = Factory::Instance().FF_CreateGUIChildSurfaceText(bossy, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss2.str().c_str(), "courier");
		//menu
		menu = Factory::Instance().FF_CreateGUISurface(buttonsurface, 0.5f, 0.5f, 0.9f, 0.6f, 120);
		//win or lose menu
		_WinOrLose = Factory::Instance().FF_CreateGUISurface(title, 0.5f, 0.2f, 0.8f, 0.3f, 140);
		eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.35f, 0.4f, 0.2f, ChangeTestScenePF, "Restart", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.60f, 0.4f, 0.2f, ChangeMainMenu, "Main Menu", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		Factory::Instance()[menu].Get<Com_GUISurface>()._active = false;
		Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = false;
		Factory::Instance()[bossy].Get<Com_GUISurface>()._active = false;

		GUISettingsInitialize();
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);


		//if pause 
		if (AEInputCheckTriggered(AEVK_P)) {
			SceneManager::Instance()._pause = !SceneManager::Instance()._pause;
		}

		//get the boss and wave info
		Com_Boss& bs = Factory::Instance()[spawner].Get<Com_Boss>();
		Com_Wave& com_wave = Factory::Instance()[spawner].Get<Com_Wave>();
		Com_EnemySpawn& em = Factory::Instance()[spawner].Get<Com_EnemySpawn>();
		std::stringstream ss;
		ss << Factory::Instance()[player].Get<Com_Health>().health;
		Factory::Instance()[lives].Get<Com_Text>()._data._text = ss.str();

		//write waves left 
		if (com_wave.numberofwaves > 0 && em.CurrNoOfEnemies > 0 && checkBoss == false)
		{
			Factory::Instance().FF_CreateGUIChildSurfaceText(waves, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Waves: ", "courier");
			checkBoss = true;
		}
		// for wave
		std::stringstream ss1;
		ss1 << Factory::Instance()[spawner].Get<Com_Wave>().numberofwaves;
		Factory::Instance()[waves].Get<Com_Text>()._data._text = ss1.str();

		//shake screen
		if (AEInputCheckCurr('L')) {
			ResourceManager::Instance()._screen_shake = 1.0f;
		}
		//#endif
		if (AEInputCheckTriggered('R')) {
			bs.disable = 0;
			bs.bossdefeat = false;
			bs.BossHealth = 20;
			SceneManager::Instance().RestartScene();
		}
		Com_Sprite& sprite = Factory::Instance()[player].Get<Com_Sprite>();
		//movement
		if (AEInputCheckTriggered(AEVK_SPACE) && !SceneManager::Instance()._pause) {
			// Shotos the primary weapon if not paused
			_playerInv.Inventory_GetCurrentWeapon().Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);

			sprite._lock = true;
			sprite._current_frame = 0;
			sprite._frame_interval_counter = 0.0f;
			sprite._current_frame_segment = 2;
		}
		if (AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_A)) {
			sprite._flip = true;
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 0;
		}
		else if (AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_W)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 1;
		}
		else if (AEInputCheckCurr(AEVK_RIGHT) || AEInputCheckCurr(AEVK_D)) {
			sprite._flip = false;
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 2;
		}
		else if (AEInputCheckCurr(AEVK_DOWN) || AEInputCheckCurr(AEVK_S)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 3;
		}
		else if (AEInputCheckTriggered(AEVK_Z) && !SceneManager::Instance()._pause) {
			// Shoots the secondary weapon if not moving or paused
			_playerInv.Inventory_GetCurrentSecondaryWeapon().Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);
		}
		else {
			arrow_sprite->_visible = false;
		}
		//if level 1 
		if (levels == 1) {
			//if lose 
			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 && once == false)
			{
				std::cout << "YOU LOSE" << std::endl;
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Lose :(", "courier");
				_playerInv.Inventory_AddCoins(25);
				once = true;
				SceneManager::Instance()._currentlyplaying = false;
			}
			//if win
			else if (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && once == false)
			{
				SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->spawnBoss = false;
				//++levels;
				levelsunlocked = 2;
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Won! Level 2 Unlocked!", "courier"); //unlock level 2 
				_playerInv.Inventory_AddCoins(50);
				once = true;
				SceneManager::Instance()._currentlyplaying = false;
			}
			//if lose or win
			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 || (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0)) {
				Factory::Instance()[menu].Get<Com_GUISurface>()._active = true;
				Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = true;
				
			}
		}
		//if level 2 
		if (levels == 2) {
			//if lose 
			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 && once == false)
			{
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Lose :(", "courier");
				_playerInv.Inventory_AddCoins(25);
				once = true;
				SceneManager::Instance()._currentlyplaying = false;
			}
			//if won
			else if (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && once == false)
			{
				SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->spawnBoss = false;
				//++levels;
				levelsunlocked = 3;
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Won! Level 3 Unlocked!", "courier"); //unlock level 3
				_playerInv.Inventory_AddCoins(50);
				once = true;
				SceneManager::Instance()._currentlyplaying = false;
			}
			//if lose or win 
			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 || (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0)) {
				Factory::Instance()[menu].Get<Com_GUISurface>()._active = true;
				Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = true;
			}
		}
		//if level 3
		if (levels == 3) {
			//if lose 
			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 && once == false)
			{
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Lose :(", "courier");
				_playerInv.Inventory_AddCoins(25);
				once = true;
				SceneManager::Instance()._currentlyplaying = false;
			}
			//if own 
			else if (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && bs.bossdefeat == true && once == false)
			{
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Win :D", "courier");
				_playerInv.Inventory_AddCoins(50);
				once = true;
				SceneManager::Instance()._currentlyplaying = false;
			}
			//if boss present 
			if (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && checkBoss == true)
			{
				Factory::Instance().FF_CreateGUIChildSurfaceText(bossy, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Boss HP:  ", "courier");
				Factory::Instance()[waves].Get<Com_GUISurface>()._active = false;
				Factory::Instance()[bossy].Get<Com_GUISurface>()._active = true;
				checkBoss = false;
			}
			std::stringstream ss2;
			ss2 << Factory::Instance()[spawner].Get<Com_Boss>().BossHealth;
			Factory::Instance()[bossy].Get<Com_Text>()._data._text = ss2.str();
			//if lose or win 
			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 || (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && bs.bossdefeat == true)) {
				Factory::Instance()[menu].Get<Com_GUISurface>()._active = true;
				Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = true;
			}
		}

		GUISettingsUpdate();
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		//for cursor reactivate
		SceneManager::Instance()._currentlyplaying = false;
		SceneManager::Instance()._inlevel = false;
	}
};

