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
	std::cout << "activated non collidable" << std::endl;
	//Factory::Instance()[tilemap].Get<Com_Position>().y = 1;
	SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 1;
}
void playerspawnbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "activated player spawn" << std::endl;
	SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 2;
}
void enemyspawnbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "activated enemy" << std::endl;
	SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 3;
}
void obstaclesbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "Save map" << std::endl;
	SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 4;
	//SceneManager::Instance().ChangeScene("Main Menu");
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
		//render 
		std::cout << "name of map " << mapname << "this is what i want" << std::endl;
		//tilemap = Factory::Instance().FF_Tilemap("tilemap", "tilehello.txt", "tilehello.txt");
		tilemap = Factory::Instance().FF_TilemapGUI("tilemap", "c_"+mapname, "t_"+mapname);
		//tilemap = Factory::Instance().FF_TilemapGUI("tilemap", "C_WilfTile.txt", "C_WilfTile.txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -7;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 4;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->_tilemap = tilemap;
		SystemDatabase::Instance().GetSystem< Sys_GUIMapClick>()->nameofmap = mapname;

		//for some reason needs this*****
		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		pf2._initialized = true;
		SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
		//**** needed 
		AEToogleFullScreen(true);

		/*eid tile = Factory::Instance().FF_CreateGUIChildClickableTileMap(main, { "transparent" }, 0.5f, 0.15f, 0.8f, 0.2f, nocolbut, mapname, "tilemap");*/

		main = Factory::Instance().FF_CreateGUISurface({ "menubackground" }, 0.85f, 0.5f, 0.25f, 1.0f, 100);																	// surface
		//non collidable - unlimited 
		eid noncol = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, { "background1" }, 0.5f, 0.15f, 0.8f, 0.2f, nocolbut, "non-collidable", "courier");
		UNREFERENCED_PARAMETER(noncol);
		//player spawn location - limit 1 
		eid playerspawn = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, { "background1" }, 0.5f, 0.4f, 0.8f, 0.2f, playerspawnbut, "playerspawn", "courier");
		UNREFERENCED_PARAMETER(playerspawn);
		//enemy spawn location - limit 2 
		eid enemyspawn = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, { "background1" }, 0.5f, 0.65f, 0.8f, 0.2f, enemyspawnbut, "enemyspawn", "courier");
		UNREFERENCED_PARAMETER(enemyspawn);
		//obstacle - unlimited
		eid obstacles = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, { "background1" }, 0.5f, 0.9f, 0.8f, 0.2f, obstaclesbut, "Save Map", "courier");
		UNREFERENCED_PARAMETER(obstacles);

		GUISettingsInitialize();
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();
		//one more frame
		if (SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp == 4 && SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->savedmap == true){
			SceneManager::Instance().ChangeScene("Main Menu");
		}
		if (AEInputCheckTriggered('R')) {
			SceneManager::Instance().RestartScene();
		}
	}
	void Exit() override {

	}

};
