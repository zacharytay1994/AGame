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
	eid main{ -1 };
	//check if it's the last message
	bool last = false;
	void Initialize() override {
		//reset 
		last = false;
		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);	//surface 
		SystemDatabase::Instance().GetSystem<Sys_TextMovingGUI>()->last = &last; //assigning pointer to last message
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
		Factory::Instance()[last].Get<Com_TextMovingGUI>().lastmessage = true; //assining the last message 
		// initialize gui settings
		GUISettingsInitialize();
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();

		//if the last message/space triggered/left clicked end credits
		if (last == true || AEInputCheckTriggered(AEVK_SPACE) || AEInputCheckTriggered(AEVK_LBUTTON)) {
			SceneManager::Instance().ChangeScene("Main Menu");
			last = false; //reset
		}
	}
};
