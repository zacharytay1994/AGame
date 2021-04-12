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

//row text box dummy button 
void rowtextbox(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
}

//column text box dummy button 
void coltextbox(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
}

//name text box dummy button
void nametextbox(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
}

//error message triggered 
void errormessage(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().RestartScene();
}

struct LevelEditor : public Scene {
	//member variables 
	eid main{ -1 };
	eid load{ -1 };
	eid tilemap = -1;
	eid mapname = -1;
	eid col = -1;
	eid row = -1;

	//sprite texture data 
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin };
	Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
	Factory::SpriteData buttonload{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };

	//static map information
	static inline std::string* nameofmap;
	static inline std::string* nameofcol;
	static inline std::string* nameofrow;

	void Initialize() override {
		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface

		//text 
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.36f, 0.26f, 0.04f, 0.04f, "Column", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.36f, 0.41f, 0.04f, 0.04f, "Row", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.23f, 0.12f, 0.04f, 0.04f, "Map Name", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.5f, 0.8f, 0.04f, 0.04f, "Load", "courier");

		//textbox
		col = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBox(main, button, 0.5f, 0.25f, 0.25f, 0.2f, coltextbox, "", "courier");			// clickable child surface
		row = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBox(main, button, 0.5f, 0.4f, 0.25f, 0.2f, rowtextbox, "", "courier");				// clickable child surface
		mapname = Factory::Instance().FF_CreateGUIChildClickableSurfaceWordsTextBox(main, button, 0.5f, 0.1f, 0.75f, 0.2f, nametextbox, "", "courier");				// clickable child surface

		//Load 
		load = Factory::Instance().FF_CreateGUIChildClickableSurfaceTextLoadTileMap(main, buttonload, 0.5f, 0.8f, 0.2f, 0.2f, ChangeTestSceneLevelEditor, "Load", "courier");		// clickable child surface
		//assigning load information
		Factory::Instance()[load].Get<Com_Writetofile>().col = &Factory::Instance()[col].Get<Com_Text>()._data._text;
		Factory::Instance()[load].Get<Com_Writetofile>().row = &Factory::Instance()[row].Get<Com_Text>()._data._text;
		Factory::Instance()[load].Get<Com_Writetofile>().name = &Factory::Instance()[mapname].Get<Com_Text>()._data._text;
		//assinging map information 
		nameofmap = &Factory::Instance()[mapname].Get<Com_Text>()._data._text;
		nameofcol = &Factory::Instance()[col].Get<Com_Text>()._data._text;
		nameofrow = &Factory::Instance()[row].Get<Com_Text>()._data._text;

		// initialize gui settings
		GUISettingsInitialize();

	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		//GUI update
		GUISettingsUpdate();
	}
};

void ChangeTestSceneLevelEditor(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	//main surface 
	eid main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);

	//sprite information
	Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };

	//check if all the user inputs are there
	if ((*LevelEditor::nameofmap).empty()) {
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(main, button, 0.5f, 0.6f, 0.75f, 0.2f, errormessage, "Hey Man! Input the map name!", "courier");
		return;
	}
	if ((*LevelEditor::nameofcol).empty()) {
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(main, button, 0.5f, 0.6f, 0.75f, 0.2f, errormessage, "Hey Man! the column in empty!", "courier");
		return;
	}
	if ((*LevelEditor::nameofrow).empty()) {
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(main, button, 0.5f, 0.6f, 0.75f, 0.2f, errormessage, "Hey Man! the row is empty!!", "courier");
		return;
	}
	//check if the row is bigger than the maximum
	if (std::stoi(*LevelEditor::nameofrow) > 10) {
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(main, button, 0.5f, 0.6f, 0.75f, 0.2f, errormessage, "Hey Man! the biggest is 10 for the row!", "courier");
		return;
	}
	//check if the column is bigger than the maximum
	if (std::stoi(*LevelEditor::nameofcol) > 10) {
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(main, button, 0.5f, 0.6f, 0.75f, 0.2f, errormessage, "Hey Man! the biggest is 10 for the column!", "courier");
		return;
	}
	//if it's a duplicate name 
	if (*LevelEditor::nameofmap == "duplicate name") {
		Factory::Instance().FF_CreateGUIChildClickableSurfaceTextBoxwitherrormsg(main, button, 0.5f, 0.6f, 0.75f, 0.2f, errormessage, "Hey Man! The name already exist!", "courier");
		return;
	}
	//if everything is fine 
	LevelEditor2::mapname = *LevelEditor::nameofmap;
	SceneManager::Instance().ChangeScene("Leveleditor2");
}