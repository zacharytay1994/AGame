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
#include"Scene_LevelEditor2.h"

void ChangeTestSceneLevelEditor(Com_GUISurface* surface);

void rowtextbox(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered col" << std::endl;
}


void coltextbox(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
}


void nametextbox(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered mapname" << std::endl;
}


void errormessage(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "entered row" << std::endl;
	SceneManager::Instance().RestartScene();
}


struct LevelEditor : public Scene {
	eid i{ -1 };
	eid main{ -1 };
	eid _settings{ -1 };
	eid _change_scene{ -1 };
	eid load{ -1 };
	Factory::SpriteData data1{ "menubackground" };
	Factory::SpriteData data2{ "buttonsurface" };
	Factory::SpriteData data3{ "button1" };
	Factory::SpriteData data4{ "button2" };
	Factory::SpriteData data5{ "button3" };
	Factory::SpriteData data6{ "transparent" };
	Factory::SpriteData buttonbg{ "buttonsbg.png", 1.0f, 1.0f, 1, 1, 1, 1.0f, 0 };
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin };
	//Sys_Pathfinding_v2 _pathfinding;

	//test
	bool inputting{ false };
	std::vector<char> result;
	std::string input;
	std::string mapdata;
	std::string bcdata;
	eid tilemap = -1;
	eid mapname = -1;
	eid col = -1;
	eid row = -1;
	static inline std::string* nameofmap;
	static inline std::string* nameofcol;
	static inline std::string* nameofrow;

	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: You're now entering the level editor." << std::endl;
		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface
		eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.4f, 0.3f, 0.4f);												// non clickable child surface

		//text 
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.25f, 0.25f, 0.04f, 0.04f, "Column", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.25f, 0.45f, 0.04f, 0.04f, "Row", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.2f, 0.15f, 0.04f, 0.04f, "Map Name", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.5f, 0.8f, 0.04f, 0.04f, "Load", "courier");
		//textbox
		col = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBox(buttons, button, 0.5f, 0.25f, 0.75f, 0.2f, coltextbox, "", "courier");			// clickable child surface
		row = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBox(buttons, button, 0.5f, 0.75f, 0.75f, 0.2f, rowtextbox, "", "courier");				// clickable child surface
		mapname = Factory::Instance().FF_CreateGUIChildClickableSurfaceWordsTextBox(main, button, 0.5f, 0.1f, 0.75f, 0.2f, nametextbox, "", "courier");				// clickable child surface
		//Load 
		load = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLoadTileMap(main, { "background1" }, 0.5f, 0.8f, 0.2f, 0.2f, ChangeTestSceneLevelEditor, "Load", "courier");		// clickable child surface
		Factory::Instance()[load].Get<Com_Writetofile>().col = &Factory::Instance()[col].Get<Com_Text>()._data._text;
		Factory::Instance()[load].Get<Com_Writetofile>().row = &Factory::Instance()[row].Get<Com_Text>()._data._text;
		Factory::Instance()[load].Get<Com_Writetofile>().name = &Factory::Instance()[mapname].Get<Com_Text>()._data._text;
		nameofmap = &Factory::Instance()[mapname].Get<Com_Text>()._data._text;
		nameofcol = &Factory::Instance()[col].Get<Com_Text>()._data._text;
		nameofrow = &Factory::Instance()[row].Get<Com_Text>()._data._text;

		// initialize gui settings
		GUISettingsInitialize();

	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();
	}
};

void ChangeTestSceneLevelEditor(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	eid main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
	eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.5f, 0.5f, 0.7f);												// non clickable child surface
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin };
	//check if all the user inputs are there
	if ((*LevelEditor::nameofmap).empty()) {
		std::cout << "input your name!" << std::endl;
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(buttons, button, 0.5f, 0.5f, 0.75f, 0.2f, errormessage, "Hey Man! Input the map name!", "courier");
		return;
	}
	if ((*LevelEditor::nameofcol).empty()) {
		std::cout << "input your column!" << std::endl;
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(buttons, button, 0.5f, 0.5f, 0.75f, 0.2f, errormessage, "Hey Man! the column in empty!", "courier");
		//SceneManager::Instance().RestartScene();
		return;
	}
	if ((*LevelEditor::nameofrow).empty()) {
		std::cout << "input your row!" << std::endl;
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(buttons, button, 0.5f, 0.5f, 0.75f, 0.2f, errormessage, "Hey Man! the row is empty!!", "courier");
		//SceneManager::Instance().RestartScene();
		return;
	}
	if (std::stoi(*LevelEditor::nameofrow) > 10) {
		std::cout << " too big row" << std::endl;
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(buttons, button, 0.5f, 0.5f, 0.75f, 0.2f, errormessage, "Hey Man! the biggest is 10 for the row!", "courier");
		//SceneManager::Instance().RestartScene();
		return;
	}
	if (std::stoi(*LevelEditor::nameofcol) > 10) {
		std::cout << "too big col" << std::endl;
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(buttons, button, 0.5f, 0.5f, 0.75f, 0.2f, errormessage, "Hey Man! the biggest is 10 for the column!", "courier");
		//SceneManager::Instance().RestartScene();
		return;
	}
	LevelEditor2::mapname = *LevelEditor::nameofmap;
	SceneManager::Instance().ChangeScene("Leveleditor2");
}