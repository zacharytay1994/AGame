#pragma once
#include "SceneDeclarations.h"

struct LevelSelect : public Scene {
	eid _title{ -1 };
	Factory::SpriteData title{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };

	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: Now entering main menu." << std::endl;


		// main background
		_title = Factory::Instance().FF_CreateGUISurface(title, 0.5f, 0.5f, 0.6f, 0.6f, 140);
		Factory::Instance().FF_CreateGUIChildSurface(_title, { "tilemaps/test.png" }, 0.5f, 0.5f, 0.5f, 0.5f);

		// initialize gui settings
		GUISettingsInitialize();

	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {

	}
};