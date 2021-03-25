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
	//Sys_Pathfinding_v2 _pathfinding;

	//test
	bool inputting{ false };
	std::vector<char> result;
	std::string input;
	std::string mapdata;
	std::string bcdata;
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
		//render 
		std::cout << "name of map " << mapname << "this is what i want" << std::endl;
		//tilemap = Factory::Instance().FF_Tilemap("tilemap", "tilehello.txt", "tilehello.txt");
		tilemap = Factory::Instance().FF_Tilemap("tilemap", mapname, mapname);
		//Factory::Instance()[tilemap].Get <Com_Writetofile>().name;
		Factory::Instance()[tilemap].Get<Com_Position>().x = -8;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 5;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;

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
};

