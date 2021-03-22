#pragma once
#include "SceneDeclarations.h"

struct MainMenu : public Scene {
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

	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: Now entering main menu." << std::endl;

		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.5f, 0.2f, 0.04f, 0.04f, "AGame", "courier");									// settings button
		eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.6f, 0.3f, 0.4f);															// non clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.25f, 0.75f, 0.2f, SettingsButton, "Start", "courier");		// clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.5f, 0.75f, 0.2f, SettingsButton, "Credits", "courier");		// clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.75f, 0.75f, 0.2f, QuitGame, "Exit", "courier");				// clickable child surface

		// initialize gui settings
		GUISettingsInitialize();

		std::vector<int> flags = { 0,0,0,0,1,1,0,0,0 };
		Pathfinding::Grid grid{ 3,3,flags };
		std::vector<Vec2i> path;
		_pathfinding.SolveAStar({ 0,0 }, { 2,2 }, grid, path);
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();
	}
};