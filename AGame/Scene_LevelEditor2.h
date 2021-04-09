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
void wallbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 4;
	//SceneManager::Instance().ChangeScene("Main Menu");
}
void bombbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 5;
	//SceneManager::Instance().ChangeScene("Main Menu");
}
void resetbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().RestartScene();
	//SceneManager::Instance().ChangeScene("Main Menu");
}
void savemapbut(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "Save map" << std::endl;
	SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 6;
	//SceneManager::Instance().ChangeScene("Main Menu");
}

void errormessage2(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
	SceneManager::Instance().RestartScene();
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
	bool frameskipped = false;


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
		//	AEToogleFullScreen(true);
		
		/*eid tile = Factory::Instance().FF_CreateGUIChildClickableTileMap(main, { "transparent" }, 0.5f, 0.15f, 0.8f, 0.2f, nocolbut, mapname, "tilemap");*/
		

		Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
		Factory::SpriteData button{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };


		main = Factory::Instance().FF_CreateGUISurface({ "transparent" }, 0.85f, 0.5f, 0.27f, 1.0f, 100);																	// surface
		//non collidable - unlimited 
		eid noncol = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, button, 0.5f, 0.1f, 0.8f, 0.1f, nocolbut, "Blank Space", "courier");
		UNREFERENCED_PARAMETER(noncol);
		//player spawn location - limit 1 
		eid playerspawn = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, button, 0.5f, 0.22f, 0.8f, 0.1f, playerspawnbut, "Player Spawn", "courier");
		UNREFERENCED_PARAMETER(playerspawn);
		//enemy spawn location - limit 2 
		eid enemyspawn = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, button, 0.5f, 0.34f, 0.8f, 0.1f, enemyspawnbut, "Enemy Spawn ", "courier");
		UNREFERENCED_PARAMETER(enemyspawn);
		//wall - unlimited 
		eid wall = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, button, 0.5f, 0.46f, 0.8f, 0.1f, wallbut, "Wall", "courier");
		UNREFERENCED_PARAMETER(wall);
		//bomb - unlimited 
		eid bomb = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, button, 0.5f, 0.58f, 0.8f, 0.1f, bombbut, "Bomb", "courier");
		UNREFERENCED_PARAMETER(bomb);
		//resetmap - 1
		eid resetmap = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, button, 0.5f, 0.7f, 0.8f, 0.1f, resetbut, "Reset Map", "courier");
		UNREFERENCED_PARAMETER(resetmap);
		//savemap - 1
		eid savemap = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLevelEditor(main, button, 0.5f, 0.82f, 0.8f, 0.1f, savemapbut, "Save Map", "courier");
		UNREFERENCED_PARAMETER(savemap);

		GUISettingsInitialize();
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();

		//one more frame
		if (SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp == 6 && SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->savedmap == true){
			//reset 
			SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 0;
			SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->savedmap = false;
			SceneManager::Instance().ChangeScene("Main Menu");
		}
		//error 
		if (SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp == 6 && SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->savedmap == false) {
			//skip one frame 
			if (frameskipped && SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->error == true) {

				eid main2 = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
				Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
				Factory::SpriteData button2{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
				Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(main2, button2, 0.5f, 0.5f, 0.75f, 0.2f, errormessage2, "You did not placed the player!", "courier");
				SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->Leveledittyp = 0;
				SystemDatabase::Instance().GetSystem<Sys_GUIMapClick>()->error = false;
				frameskipped = false;
			}
			frameskipped = true;
		}
		//if (AEInputCheckTriggered('R')) {
		//	SceneManager::Instance().RestartScene();
		//}
	}
	void Exit() override {

	}

};
