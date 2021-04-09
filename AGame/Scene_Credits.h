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


struct Scene_Credits : public Scene {
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
	Factory::SpriteData data7{ "gamelogo" };
	Factory::SpriteData buttonbg{ "buttonsbg.png", 1.0f, 1.0f, 1, 1, 1, 1.0f, 0 };
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin };
	Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
	Factory::SpriteData buttonload{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
	bool last = false;
	

	bool _gui_change_scene{ false };
	void Initialize() override {
		//reset 
		last = false;
		std::cout << "SYSTEM MESSAGE: You're now entering the level editor." << std::endl;
		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface
		//eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.4f, 0.3f, 0.4f);												// non clickable child surface
		SystemDatabase::Instance().GetSystem<Sys_TextMovingGUI>()->last = &last;
		//text 
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "gamelogo" }, 0.5f, 0.25f, 0.4f, 0.4f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "teamlogo" }, 0.5f, 0.4f, 0.3f, 0.3f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "zachary" }, 0.35f, 0.65f, 0.25f, 0.25f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "noel" }, 0.65f, 0.65f, 0.25f, 0.25f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "austen" }, 0.35f, 0.85f, 0.25f, 0.25f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "wilfred" }, 0.65f, 0.85f, 0.25f, 0.25f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "faculty" }, 0.5f, 1.1f, 0.35f, 0.35f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "instructorthomas" }, 0.5f, 1.25f, 0.25f, 0.25f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "instructordx" }, 0.5f, 1.33f, 0.25f, 0.25f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "digipensingapore" }, 0.5f, 1.55f, 0.35f, 0.35f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "president" }, 0.5f, 1.85f, 0.35f, 0.35f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "executives1" }, 0.5f, 2.05f, 0.48f, 0.48f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "executives2" }, 0.5f, 2.12f, 0.48f, 0.48f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "executives3" }, 0.5f, 2.17f, 0.48f, 0.48f, "", "courier");
		Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "ending" }, 0.5f, 2.5f, 0.3f, 0.3f, "", "courier");
		eid last = Factory::Instance().FF_CreateGUIChildSurfaceTextMoving(main, { "endingfmod" }, 0.5f, 2.6f, 0.3f, 0.3f, "", "courier");
		Factory::Instance()[last].Get<Com_TextMovingGUI>().lastmessage = true;
		// initialize gui settings
		GUISettingsInitialize();
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();

		if (last == true || AEInputCheckTriggered(AEVK_SPACE)) {
			SceneManager::Instance().ChangeScene("Main Menu");
		}
	}
};
