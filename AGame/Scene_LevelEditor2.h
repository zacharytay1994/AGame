#pragma once
#include "SceneDeclarations.h"
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include "ResourceManager.h"
#include "Inventory.h"
#include <string>
#include "Scene_LevelEditor.h"


void nocolbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
}
void playerspawnbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
}
void enemyspawnbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
}
void obstaclesbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
}


void ChangeTestSceneLevelEditor2(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().ChangeScene("Main Menu");
}

struct LevelEditor2 : public Scene {
	eid i{ -1 };
	eid main{ -1 };
	eid _settings{ -1 };
	eid _change_scene{ -1 };
	Factory::SpriteData data1{ "menubackground" };
	Factory::SpriteData data2{ "buttonsurface" };
	Factory::SpriteData data3{ "button1" };
	Factory::SpriteData data4{ "button2" };
	Factory::SpriteData data5{ "button3" };
	Factory::SpriteData data6{ "transparent" };
	Factory::SpriteData dataskeleton{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	//Sys_Pathfinding_v2 _pathfinding;


	eid tilemap = -1;
	static inline std::string mapname;
	eid test = -1;

	//for the buttons 
	eid noncol = -1;
	eid playerspawn = -1;
	eid enemyspawn = -1;
	eid obstacles = -1;

	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: You're now entering the level editor.2" << std::endl;
		// main background
		//main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface
		//eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.4f, 0.3f, 0.4f);												// non clickable child surface

		////text 
		//Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.25f, 0.25f, 0.04f, 0.04f, "Column", "courier");
		//Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.25f, 0.45f, 0.04f, 0.04f, "Row", "courier");
		////textbox
		//eid col = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBox(buttons, { "background1" }, 0.5f, 0.25f, 0.75f, 0.2f, SettingsButton, "", "courier");			// clickable child surface
		//eid row = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBox(buttons, { "background1" }, 0.5f, 0.75f, 0.75f, 0.2f, SettingsButton, "", "courier");				// clickable child surface

		////Load 
		//Factory::Instance().FF_CreateGUIChildClickableSurfaceText(main, { "background1" }, 0.5f, 0.8f, 0.2f, 0.2f, ChangeTestSceneLevelEditor2, "Load", "courier");		// clickable child surface

		//render text box
		//AEToogleFullScreen(true);
		//prev level editor 
		//eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.4f, 0.3f, 0.4f);
		//eid name = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBox(buttons, { "background1" }, 0.5f, 0.25f, 0.75f, 0.2f, SettingsButton, "", "courier");			// clickable child surface
		//Factory::Instance()[name].AddComponent<Com_Writetofile>();
		//eid test = Factory::Instance().FF_WriteTileMap();
		/*std::string namemap = *Factory::Instance()[LevelEditor::load].Get<Com_Writetofile>().name;*/
		//test = Factory::Instance()[LevelEditor::load].Get<LevelEditor>().load;
		//test = LevelEditor::load;
		//std::string namemap = *Factory::Instance()[LevelEditor::load].Get<Com_Writetofile>().name;
		//namemap = *LevelEditor::nameofmap;
		//std::string namemap = Factory::Instance()[LevelEditor::mapname].Get<Com_Text>()._data._text;
		// 
		// 
		// 
		// 
		//tilemap = Factory::Instance().FF_Tilemap("tilemap", "C_WilfTile.txt", "T_WilfTile.txt");
		//Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		//Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
		//Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		////passing in ref to tilemap for enemy
		//SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_tilemap = tilemap;
		////SystemDatabase::Instance().GetSystem<Sys_Projectile>().tilemap = tilemap;
		////init tile map 
		//Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		//Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		//pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		//pf2._initialized = true;

		//////player
		//player = Factory::Instance().FF_SpriteTile(data2, tilemap, 0, 0);
		//Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Projectile, Com_WeaponAttack, Com_Camera, Com_type, Com_GridColData, Com_Health>();
		//SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
		//SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;


		//render 
		std::cout << "name of map " << mapname << "this is what i want" << std::endl;
		//tilemap = Factory::Instance().FF_Tilemap("tilemap", "tilehello.txt", "tilehello.txt");
		tilemap = Factory::Instance().FF_TilemapGUI("tilemap", mapname, mapname);
		//tilemap = Factory::Instance().FF_TilemapGUI("tilemap", "C_WilfTile.txt", "C_WilfTile.txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -8;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 5;
		//Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		//Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->_tilemap = tilemap;

		//for some reason needs this*****
		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		pf2._initialized = true;
		SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
		//**** needed 

		//testing
		eid player = Factory::Instance().FF_SpriteTile(dataskeleton, tilemap, 0, 0);
		//Factory::Instance()[player].AddComponent<Com_YLayering>();


		/*eid tile = Factory::Instance().FF_CreateGUIChildClickableTileMap(main, { "transparent" }, 0.5f, 0.15f, 0.8f, 0.2f, nocolbut, mapname, "tilemap");*/

		main = Factory::Instance().FF_CreateGUISurface({ "menubackground" }, 0.85f, 0.5f, 0.25f, 1.0f, 100);																	// surface
		//non collidable - unlimited 
		eid noncol = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(main, { "background1" }, 0.5f, 0.15f, 0.8f, 0.2f, nocolbut, "non-collidable", "courier");
		UNREFERENCED_PARAMETER(noncol);
		//player spawn location - limit 1 
		eid playerspawn = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(main, { "background1" }, 0.5f, 0.4f, 0.8f, 0.2f, playerspawnbut, "playerspawn", "courier");
		UNREFERENCED_PARAMETER(playerspawn);
		//enemy spawn location - limit 2 
		eid enemyspawn = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(main, { "background1" }, 0.5f, 0.65f, 0.8f, 0.2f, enemyspawnbut, "enemyspawn", "courier");
		UNREFERENCED_PARAMETER(enemyspawn);
		//obstacle - unlimited
		eid obstacles = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(main, { "background1" }, 0.5f, 0.9f, 0.8f, 0.2f, obstaclesbut, "obstacles", "courier");
		UNREFERENCED_PARAMETER(obstacles);




		//eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.5f, 1.f, 1.f);												// non clickable child surface

		//render grid map 

		//render choosing options 

		//render naming and save file 
		//write to file 

		// initialize gui settings
		GUISettingsInitialize();

		//std::vector<int> flags = { 0,0,0,0,1,1,0,0,0 };
		//Pathfinding::Grid grid{ 3,3,flags };
		//std::vector<Vec2i> path;
		//_pathfinding.SolveAStar({ 0,0 }, { 2,2 }, grid, path);
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();
	}
	void Exit() override {

	}

};



//struct LevelEditor2 : public Scene
//{
//	/*
//	Member Variables
//	________________________________*/
//	eid player = -1;
//	//Com_Tilemap tile;
//	//eid tilemap = -1;
//	eid enemytest = -1;
//	eid tilemap = -1;
//	eid lives{ -1 };
//	eid waves{ -1 };
//	eid spawner{ -1 };
//	eid menu{ -1 };
//	Inventory playerInv;
//	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
//	Factory::SpriteData man{ "hero.png", 100.0f, 160.0f, 3, 3, 8, 0.1f, 0, passin };
//	Factory::SpriteData data{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
//	Factory::SpriteData data1{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.25f };
//	Factory::SpriteData data2{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.15f };
//	Factory::SpriteData data22{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.25f };
//	Factory::SpriteData underline{ "underline.png", 80.0f, 200.0f, 4, 1, 4, 0.25f };
//	Factory::SpriteData clock{ "clock.png", 80.0f, 200.0f, 3, 2, 5, 0.20f };
//	Vec2i passin2[5] = { {0,1},{2,3},{4,5},{6,7},{0,0} };
//	Factory::SpriteData arrows{ "arrows.png", 50.0f, 50.0f, 3, 3, 8, 0.1f, -900, passin2 };
//	eid arrow = -1;
//	Com_Sprite* arrow_sprite{ nullptr };
//	//Factory::SpriteData data{ 0,"test2", 1, 8, 8, 0.1f, 100.0f, 200.0f };
//
//	Factory::SpriteData buttonsurface{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
//	Vec2i passin3[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
//	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin3 };
//	/*
//	Initialize Override (optional)
//	________________________________*/
//	void Initialize() override {
//
//		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test2.txt", "t_test2.txt");
//		Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
//		Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
//		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
//		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_tilemap = tilemap;
//
//
//		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
//		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
//		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
//		pf2._initialized = true;
//		SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
//
//		player = Factory::Instance().FF_SpriteTile(man, tilemap, 0, 0);
//		Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState>();
//		//Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
//
//
//
//		//Factory::Instance().FF_CreateGUISurface(clock, 0.5f, 0.05f, 0.1f, 0.1f, 100);
//
//		GUISettingsInitialize();
//		_playerInv.Inventory_SetWeaponUnlocked("Pistol");
//		_playerInv.Inventory_EquipWeapon("Pistol");
//		std::cout << "EQUIPPED PISTOL" << std::endl;
//	}
//	/*
//	Update Override (optional)
//	________________________________*/
//	void Update(const float& dt) override {
//		UNREFERENCED_PARAMETER(dt);
//		GUISettingsUpdate();
//	}
//	/*
//	Exit Override (optional)
//	________________________________*/
//	void Exit() override {
//
//	}
//};