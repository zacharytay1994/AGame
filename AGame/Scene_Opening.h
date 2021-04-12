#pragma once
#include "SceneDeclarations.h"

struct Opening : public Scene {
	//member variables
	eid main{ -1 };
	//sprite data
	Factory::SpriteData data1{ "logo" };
	bool last = false; //tracking if time is up 
	

	void Initialize() override {
		//reset 
		last = false;
		//opening
		main = Factory::Instance().FF_CreateGUISurface(data1, 0.5f, 0.5f, 1.0f, 1.0f, 100);															// surface
		Factory::Instance()[main].AddComponent<Com_GUIDelay>();
		SystemDatabase::Instance().GetSystem<Sys_GUIDelay>()->last = &last; //delay the scene
	
		// initialize gui settings
		GUISettingsInitialize();
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		//gui update
		GUISettingsUpdate();

		//if time is up or early triggered
		if (last == true
			|| (AEInputCheckTriggered(AEVK_SPACE))
			|| (AEInputCheckTriggered(AEVK_LBUTTON))
			|| (AEInputCheckTriggered(AEVK_RETURN))
			|| (AEInputCheckTriggered(AEVK_ESCAPE))) {
			last = false;
			SceneManager::Instance().ChangeScene("Main Menu");
		}
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
	}
};