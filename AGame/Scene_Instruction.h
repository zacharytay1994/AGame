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
#include "SceneDeclarations.h"




void instructions(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
	//SceneManager::Instance().RestartScene();
}


struct Scene_Instructions : public Scene
{
	/*
	Member Variables
	________________________________*/
	std::string test = "hello";
	eid player = -1;
	//Com_Tilemap tile;
	//eid tilemap = -1;
	eid enemytest = -1;
	eid tilemap = -1;
	eid lives{ -1 };
	eid waves{ -1 };
	eid spawner{ -1 };
	eid menu{ -1 };
	eid wall{ -1 };
	eid bomb{ -1 };
	eid _WinOrLose{ -1 };
	Inventory playerInv;
	Factory::SpriteData box{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };
	Factory::SpriteData boom{ "kaboom", 40.0f, 40.0f, 1, 1, 1, 0.15f };
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData man{ "hero.png", 200.0f, 320.0f, 4, 3, 12, 0.1f, 0, passin };
	Factory::SpriteData data{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	Factory::SpriteData data1{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.25f };
	Factory::SpriteData data2{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.15f };
	Factory::SpriteData data22{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.25f };
	Factory::SpriteData underline{ "underline.png", 80.0f, 200.0f, 4, 1, 4, 0.25f };
	Factory::SpriteData clock{ "clock.png", 80.0f, 200.0f, 3, 2, 5, 0.20f };
	Factory::SpriteData title{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Vec2i passin2[5] = { {0,1},{2,3},{4,5},{6,7},{0,0} };
	Factory::SpriteData arrows{ "arrows.png", 50.0f, 50.0f, 3, 3, 8, 0.1f, -900, passin2 };
	eid arrow = -1;
	Com_Sprite* arrow_sprite{ nullptr };
	//Factory::SpriteData data{ 0,"test2", 1, 8, 8, 0.1f, 100.0f, 200.0f };
	size_t currentinstructions{ 0 };
	bool messageseen{ false };
	bool up{ false };
	bool down{ false };
	bool left{ false };
	bool right{ false };
	bool last{ false };
	size_t spacetriggered{ 0 };
	size_t daggertriggered{ 0 };
	bool once = false;
	Factory::SpriteData buttonsurface{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Vec2i passin3[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin3 };
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		std::cout << sizeof(Com_Tilemap) << std::endl;
		once = false;

		//reset 
		last = false;
		if (currentinstructions == 4) {
			//reset
			currentinstructions = 0;
		}
		messageseen = false;
		//init tilemap 
		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_INSTRUCTION.txt", "t_INSTRUCTION.txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -3;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_tilemap = tilemap;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_tilemap = tilemap;
		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();


		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		pf2._initialized = true;
		SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_AABB>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_ArrowKeysTilemap>()->_grid = &pf2._grid;


		//first 
		if (currentinstructions == 0) {
			//only spawn player 
			for (int y = 0; y < com_tilemap._height; ++y) {
				for (int x = 0; x < com_tilemap._width; ++x) {
					//if it's a player spawn location 
					if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 2) {
						player = Factory::Instance().FF_SpriteTile(man, tilemap, x, y);
						Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState, Com_type>();
						Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
						Factory::Instance()[player].Get<Com_type>().type = 0; // set player type
						//SystemDatabase::Instance().GetSystem<Sys_AABB>()->player_id = player;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->tile = tilemap;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
						SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;

						SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->playerpos = player;

					}
				}
			}
		}
		//second
		if (currentinstructions == 1) {
			for (int y = 0; y < com_tilemap._height; ++y) {
				for (int x = 0; x < com_tilemap._width; ++x) {
					//if it's a player spawn location 
					if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 2) {
						player = Factory::Instance().FF_SpriteTile(man, tilemap, x, y);
						Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState, Com_type>();
						Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
						Factory::Instance()[player].Get<Com_type>().type = 0; // set player type
						//SystemDatabase::Instance().GetSystem<Sys_AABB>()->player_id = player;
						SystemDatabase::Instance().GetSystem<Sys_AABB>()->_grid = &pf2._grid;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->tile = tilemap;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
						SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;
						SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->playerpos = player;

					}
				}
			}
		}
		//third
		if (currentinstructions == 2) {
			for (int y = 0; y < com_tilemap._height; ++y) {
				for (int x = 0; x < com_tilemap._width; ++x) {
					//if it's a player spawn location 
					if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 2) {
						player = Factory::Instance().FF_SpriteTile(man, tilemap, x, y);
						Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState, Com_type>();
						Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
						Factory::Instance()[player].Get<Com_type>().type = 0; // set player type
						//SystemDatabase::Instance().GetSystem<Sys_AABB>()->player_id = player;
						SystemDatabase::Instance().GetSystem<Sys_AABB>()->_grid = &pf2._grid;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->tile = tilemap;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
						SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;
						SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->playerpos = player;

					}
				}
			}
		}
		//fourth
		if (currentinstructions == 3) {
			//spawn player and monster
			for (int y = 0; y < com_tilemap._height; ++y) {
				for (int x = 0; x < com_tilemap._width; ++x) {
					//if it's a player spawn location 
					if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 2) {
						player = Factory::Instance().FF_SpriteTile(man, tilemap, x, y);
						Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState, Com_type>();
						Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
						Factory::Instance()[player].Get<Com_type>().type = 0; // set player type
						//SystemDatabase::Instance().GetSystem<Sys_AABB>()->player_id = player;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->tile = tilemap;
						SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
						SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;

						SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->playerpos = player;

					}
					//if it's a enemy spawn location 
					if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 3) {
						spawner = Factory::Instance().FF_CreateSpawner();

						//Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
						//Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
						pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
						pf2._initialized = true;
						SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
						SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_grid = &pf2._grid;
						SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_grid = &pf2._grid;
						SystemDatabase::Instance().GetSystem<Sys_AABB>()->_grid = &pf2._grid;
						SystemDatabase::Instance().GetSystem<Sys_ArrowKeysTilemap>()->_grid = &pf2._grid;
						SystemDatabase::Instance().GetSystem<Sys_AABB>()->_spawner = &Factory::Instance()[spawner].Get<Com_EnemySpawn>();
						Factory::Instance()[spawner].Get<Com_Boss>().BossHealth = 20;
						SystemDatabase::Instance().GetSystem<Sys_AABB>()->Boss = &Factory::Instance()[spawner].Get<Com_Boss>();
						SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->boss = &Factory::Instance()[spawner].Get<Com_Boss>();
						SystemDatabase::Instance().GetSystem<Sys_EnemyStateBoss>()->boss = &Factory::Instance()[spawner].Get<Com_Boss>();
						Factory::Instance()[spawner].Get<Com_Wave>().numberofwaves = 2;
					}
				}
			}
		}

		arrow = Factory::Instance().FF_Sprite(arrows, 0.0f, 0.0f);
		Entity& a = Factory::Instance()[arrow];
		a.AddComponent<Com_ParentPosition>();
		a.Get<Com_ParentPosition>()._parent_id = player;
		arrow_sprite = &a.Get<Com_Sprite>();

		//menu = Factory::Instance().FF_CreateGUISurface(buttonsurface, 0.5f, 0.5f, 0.9f, 0.6f, 120);
		//eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.35f, 0.4f, 0.2f, ChangeTestScenePF, "Restart", "courier");
		//Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		//start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.60f, 0.4f, 0.2f, ChangeMainMenu, "Main Menu", "courier");
		//Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		//Factory::Instance()[menu].Get<Com_GUISurface>()._active = false;

		menu = Factory::Instance().FF_CreateGUISurface(buttonsurface, 0.5f, 0.5f, 0.9f, 0.6f, 120);
		_WinOrLose = Factory::Instance().FF_CreateGUISurface(title, 0.5f, 0.2f, 0.8f, 0.3f, 140);
		//eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.35f, 0.4f, 0.2f, ChangeTestScenePF, "Restart", "courier");
		//Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.50f, 0.4f, 0.2f, ChangeMainMenu, "Main Menu", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		Factory::Instance()[menu].Get<Com_GUISurface>()._active = false;
		Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = false;

		GUISettingsInitialize();
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);

		std::stringstream ss1;

		if (AEInputCheckCurr('L')) {
			ResourceManager::Instance()._screen_shake = 1.0f;
		}
		if (AEInputCheckTriggered(AEVK_SPACE)) {
			_playerInv.Inventory_GetCurrentWeapon().Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);
		}
		if (AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_A)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 0;
		}
		else if (AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_W)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 1;
		}
		else if (AEInputCheckCurr(AEVK_RIGHT) || AEInputCheckCurr(AEVK_D)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 2;
		}
		else if (AEInputCheckCurr(AEVK_DOWN) || AEInputCheckCurr(AEVK_S)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 3;
		}
		else if (AEInputCheckTriggered(AEVK_Z) && !SceneManager::Instance()._pause) {
			_playerInv.Inventory_GetCurrentSecondaryWeapon().Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);
		}
		else {
			arrow_sprite->_visible = false;
		}
		//message to WASD 
		//press WASD / Up Down Left Right to move
		if (currentinstructions == 0 && messageseen == false) {
			eid main2 = Factory::Instance().FF_CreateGUISurface({ "transparent" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
			Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
			Factory::SpriteData button2{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
			Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwithinstructions(main2, button2, 0.5f, 0.1f, 0.75f, 0.2f, instructions, "WASD", "courier");
			SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->error = false;
			messageseen = true;
		}
		if (currentinstructions == 0 && messageseen == true) {
			if (AEInputCheckTriggered(AEVK_UP) || AEInputCheckTriggered(AEVK_W)) {
				up = true;
			}
			if (AEInputCheckTriggered(AEVK_DOWN) || AEInputCheckTriggered(AEVK_S)) {
				down = true;
			}
			if (AEInputCheckTriggered(AEVK_LEFT) || AEInputCheckTriggered(AEVK_A)) {
				left = true;
			}
			if (AEInputCheckTriggered(AEVK_RIGHT) || AEInputCheckTriggered(AEVK_S)) {
				right = true;
			}
			if (up == true && down == true && right == true && left == true) {
				++currentinstructions;
				messageseen = false;
				SceneManager::Instance().RestartScene();
			}
		}
		//Message to shoot 
		if (currentinstructions == 1 && messageseen == false) {
			eid main2 = Factory::Instance().FF_CreateGUISurface({ "transparent" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
			Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
			Factory::SpriteData button2{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
			Factory::Instance().FF_CreateGUIChildSurfaceText(main2, button2, 0.5f, 0.1f, 0.75f, 0.2f, "PRESS SPACE x5", "courier");
			SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->error = false;
			messageseen = true;
		}
		//Press space to shoot 
		if (currentinstructions == 1 && messageseen == true) {
			_playerInv.Inventory_SetWeaponUnlocked("Pistol");
			_playerInv.Inventory_EquipWeapon("Pistol");
			if (AEInputCheckTriggered(AEVK_SPACE)) {
				++spacetriggered;
			}
			if (spacetriggered >= 5) {
				currentinstructions = 2;
				messageseen = false;
				SceneManager::Instance().RestartScene();
			}
		}

		//Message to use dagger 
		if (currentinstructions == 2 && messageseen == false) {
			eid main2 = Factory::Instance().FF_CreateGUISurface({ "transparent" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
			Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
			Factory::SpriteData button2{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
			Factory::Instance().FF_CreateGUIChildSurfaceText(main2, button2, 0.5f, 0.1f, 0.75f, 0.2f, "PRESS Z x5!", "courier");
			SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->error = false;
			messageseen = true;
		}
		//Press space to shoot 
		if (currentinstructions == 2 && messageseen == true) {
			_playerInv.Inventory_SetWeaponUnlocked("Pistol");
			_playerInv.Inventory_EquipWeapon("Pistol");
			if (AEInputCheckTriggered(AEVK_Z)) {
				++daggertriggered;
			}
			if (daggertriggered >= 5) {
				++currentinstructions;
				messageseen = false;
				SceneManager::Instance().RestartScene();
			}
		}

		if (currentinstructions >= 3 && messageseen == false) {	
			eid main2 = Factory::Instance().FF_CreateGUISurface({ "transparent" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
			Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
			Factory::SpriteData button2{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
			Factory::Instance().FF_CreateGUIChildSurfaceText(main2, button2, 0.5f, 0.1f, 0.75f, 0.2f, "KILL!", "courier");
			messageseen = true;
		}
		if (currentinstructions == 3 && messageseen == true) {
			currentinstructions = 4;
			messageseen = false;
			last = true;
			spacetriggered = 0;
			daggertriggered = 0;
			up = false;
			down = false;
			left = false;
			right = false;
			SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->spawnBoss = false;
		}
		if (last == true) {
			Com_Wave& com_wave = Factory::Instance()[spawner].Get<Com_Wave>();
			Com_EnemySpawn& em = Factory::Instance()[spawner].Get<Com_EnemySpawn>();
			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 && once == false)
			{
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Lose :(", "courier");
				once = true;
			}
			else if (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && once == false)
			{
				SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->spawnBoss = false;
				Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "GOOD!", "courier");
				once = true;
			}

			if (Factory::Instance()[player].Get<Com_Health>().health <= 0 || (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0)) {
				Factory::Instance()[menu].Get<Com_GUISurface>()._active = true;
				Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = true;
			}

		}

		//if all monster die change scene 


		GUISettingsUpdate();
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		std::cout << "woo switching to scene 2!" << std::endl;

	}
};
