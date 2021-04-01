#pragma once
#include "SceneDeclarations.h"

struct InventoryMenu : public Scene {
	eid i{ -1 };
	eid main{ -1 };
	eid _settings{ -1 };
	eid _change_scene{ -1 };
	eid _buttons_surface{ -1 };
	eid _title{ -1 };
	Factory::SpriteData data1{ "menubackground" };
	Factory::SpriteData data2{ "buttonsurface" };
	Factory::SpriteData data3{ "button1" };
	Factory::SpriteData data4{ "button2" };
	Factory::SpriteData data5{ "button3" };
	Factory::SpriteData data6{ "transparent" };
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin };
	Factory::SpriteData buttonbg{ "buttonsbg.png", 1.0f, 1.0f, 1, 1, 1, 1.0f, 0 };
	Factory::SpriteData title{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Sys_PathFinding _pathfinding;
	float offset_rad = 0.0f;
	float offset_y = 0.0f;
	float original_y = 0.5f;
	float original_dim_x = 0.8f;
	float original_dim_y = 0.3f;
	//Sys_PathFinding _pathfinding;
	eid current_weapon{ -1 };

	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: Now entering main menu." << std::endl;

		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
		//Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.5f, 0.2f, 0.04f, 0.04f, "Inventory", "courier");
		_title = Factory::Instance().FF_CreateGUISurface(title, 0.5f, 0.2f, original_dim_x, original_dim_y, 140);
		Factory::Instance().FF_CreateGUIChildSurfaceText(_title, { "transparent" }, 0.35f, 0.5f, 0.8f, 0.4f, "Inventory", "courier");
		std::stringstream current_coins;
		current_coins << "[Your Coins: " << _playerInv.coins << ']';
		Factory::Instance().FF_CreateGUIChildSurfaceText(_title, { "transparent" }, 0.65f, 0.5f, 0.8f, 0.4f, current_coins.str().c_str(), "courier");
		std::string current_weapon_text = _playerInv.Inventory_GetCurrentWeapon().GetWeapon_Name();
		current_weapon = Factory::Instance().FF_CreateGUIChildSurfaceText(_title, { "transparent" }, 0.5f, 0.6f, 0.8f, 0.4f, current_weapon_text, "courier");
		
		_buttons_surface = Factory::Instance().FF_CreateGUISurface(buttonbg, 0.5f, original_y, 0.9f, 0.6f, 120);
		eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.5f, 0.35f, 0.4f, 0.2f, ChangeTestScenePF, "Start", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();

		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.30f, 0.5f, 0.5f, 0.2f, EquipPistol, "Pistol", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.70f, 0.5f, 0.5f, 0.2f, EquipTrickPistol, "Trick Pistol", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.30f, 0.65f, 0.5f, 0.2f, EquipDualPistol, "Dual Pistol", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.70f, 0.65f, 0.5f, 0.2f, EquipDualDiagPistol, "DD Pistol", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.30f, 0.8f, 0.5f, 0.2f, EquipDagger, "Dagger", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();

		// initialize gui settings
		GUISettingsInitialize();
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();
		Factory::Instance()[current_weapon].Get<Com_Text>()._data._text = _playerInv.Inventory_GetCurrentWeapon().GetWeapon_Name();
		offset_rad = offset_rad + dt > 2.0f * PI ? 0.0f : offset_rad + dt;
		offset_y = sin(offset_rad);
		Factory::Instance()[_buttons_surface].Get<Com_GUISurface>()._position.y = original_y + offset_y * 0.03f;
		Factory::Instance()[_title].Get<Com_GUISurface>()._dimensions = { original_dim_x + (offset_y + 1.0f) * 0.03f, original_dim_y + (offset_y + 1.0f) * 0.03f };
	}
};