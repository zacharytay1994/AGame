#pragma once
#include "SceneDeclarations.h"
 
void TestFunction(Com_GUISurface* surface) {
	std::cout << "button1" << std::endl;
}

static bool _change_scene_toggle{ false };
void ToggleChangeSceneButton(Com_GUISurface* surface) {
	_change_scene_toggle = !_change_scene_toggle;
}

static bool _settings_toggle{ false };
void SettingsButton(Com_GUISurface* surface) {
	_settings_toggle = !_settings_toggle;
}

void QuitGame(Com_GUISurface* surface) {
	std::cout << "button3" << std::endl;
	SceneManager::Instance().StopGame();
}

void ChangeTestScene(Com_GUISurface* surface) {
	SceneManager::Instance().ChangeScene("Test Scene");
}

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

	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: Now entering main menu." << std::endl;

		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface
		Factory::Instance().FF_CreateGUIChildClickableSurface(main, { "settings" }, 0.96f, 0.04f, 0.04f, 0.04f, SettingsButton);									// settings button
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.5f, 0.2f, 0.04f, 0.04f, "AGame", "courier");									// settings button
		eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.6f, 0.3f, 0.4f);															// non clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.25f, 0.75f, 0.2f, SettingsButton, "Start", "courier");		// clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.5f, 0.75f, 0.2f, SettingsButton, "Credits", "courier");		// clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.75f, 0.75f, 0.2f, QuitGame, "Exit", "courier");				// clickable child surface

		// settings menu
		_settings = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.84f, 0.38f, 0.3f, 0.6f);
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_settings, { "background1" }, 0.5f, 0.2f, 0.9f, 0.08f, ToggleChangeSceneButton, "Change Scene", "courier");	// clickable child surface text

		// change scene menu
		_change_scene = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 0.3f, 0.6f, 200);
		Factory::Instance()[_change_scene].AddComponent<Com_GUIDrag, Com_GUIMouseCheck>();
		Factory::Instance().FF_CreateGUIChildSurfaceText(_change_scene, { "transparent" }, 0.5f, 0.08f, 0.9f, 0.05f, "Select Scene", "courier");					// clickable child surface text
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.2f, 0.9f, 0.08f, ChangeTestScene, "Aus", "courier");	// clickable child surface text
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.4f, 0.9f, 0.08f, ChangeTestScene, "Noel", "courier");	// clickable child surface text
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.6f, 0.9f, 0.08f, ChangeTestScene, "Wilf", "courier");	// clickable child surface text
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.8f, 0.9f, 0.08f, ChangeTestScene, "Zac", "courier");	// clickable child surface text
		Factory::Instance().FF_CreateGUIChildClickableSurface(_change_scene, { "cross" }, 0.9f, 0.05f, 0.08f, 0.04f, ToggleChangeSceneButton);						// clickable child surface text

		//Factory::Instance().FF_CreateGUISurface(data6, 0.5f, 0.5f, 0.2f, 0.2f);
	}
	void Update(const float& dt) override {
		Factory::Instance()[_settings].Get<Com_GUISurface>()._active = _settings_toggle;
		Factory::Instance()[_change_scene].Get<Com_GUISurface>()._active = _change_scene_toggle;
	}
};