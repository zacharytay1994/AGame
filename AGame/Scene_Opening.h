#pragma once
#include "SceneDeclarations.h"

struct Opening : public Scene {
	eid i{ -1 };
	eid main{ -1 };
	eid _change_scene{ -1 };
	eid load{ -1 };
	Factory::SpriteData data1{ "logo" };
	bool last = false;
	

	void Initialize() override {
		last = false;
		std::cout << "SYSTEM MESSAGE: Now entering Opening Scene." << std::endl;
		//opening

		main = Factory::Instance().FF_CreateGUISurface(data1, 0.5f, 0.5f, 1.0f, 1.0f, 100);															// surface
		Factory::Instance()[main].AddComponent<Com_GUIDelay>();
		SystemDatabase::Instance().GetSystem<Sys_GUIDelay>()->last = &last;
		

		// initialize gui settings
		GUISettingsInitialize();
		

	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();


		if (last == true || AEInputCheckTriggered(AEVK_SPACE) || AEInputCheckTriggered(AEVK_LBUTTON)) {
			SceneManager::Instance().ChangeScene("Main Menu");
		}
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
	}
};