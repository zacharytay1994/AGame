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

struct LevelEditor : public Scene {
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
	Sys_Pathfinding_v2 _pathfinding;

	//test
	bool inputting{ false };
	std::vector<char> result;
	std::string input;
	int col;
	int row;
	std::string mapdata;
	std::string bcdata;
	eid tilemap = -1;


	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: You're now entering the level editor." << std::endl;
		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.25f, 0.2f, 0.04f, 0.04f, "Textbox1", "courier");									// settings button
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.75f, 0.2f, 0.04f, 0.04f, "Textbox2", "courier");
		eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.6f, 0.3f, 0.4f);												// non clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.25f, 0.75f, 0.2f, SettingsButton, "Load", "courier");			// clickable child surface
		//Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.5f, 0.75f, 0.2f, SettingsButton, "Credits", "courier");		// clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.75f, 0.75f, 0.2f, QuitGame, "Save", "courier");				// clickable child surface

		//render text box
		
		//render 
		tilemap = Factory::Instance().FF_Tilemap("tilemap", "C_WilfTile.txt", "T_WilfTile.txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;

		//level editor
		//	1. pass in col
		//	2. pass in row
		//	3. Render grid based
		//	4. click and click
		//	options :
		//enemy spawn
		//	player spawn
		//	wall
		//	bomb
		//	5. name, save

		// initialize gui settings
		GUISettingsInitialize();

		std::vector<int> flags = { 0,0,0,0,1,1,0,0,0 };
		Pathfinding::Grid grid{ 3,3,flags };
		std::vector<Vec2i> path;
		_pathfinding.SolveAStar({ 0,0 }, { 2,2 }, grid, path);
	}
	void Update(const float& dt) override {
		GUISettingsUpdate();
	}
};

