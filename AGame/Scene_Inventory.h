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
	Vec2i passin2[5] = { {0,3},{4,7},{9,12},{13,16},{0,0} };
	Factory::SpriteData button_dark{ "buttonsprite_dark.png", 1.0f, 1.0f, 6, 3, 17, 0.1f, 0, passin2 };
	Factory::SpriteData buttonbg{ "buttonsbg.png", 1.0f, 1.0f, 1, 1, 1, 1.0f, 0 };
	Factory::SpriteData title{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Sys_PathFinding _pathfinding;
	float offset_rad = 0.0f;
	float offset_y = 0.0f;
	float original_y = 0.5f;
	float original_dim_x = 0.8f;
	float original_dim_y = 0.3f;
	//Sys_PathFinding _pathfinding;
	eid coins_text{ -1 };
	eid current_weapon{ -1 };
	eid pistol_weapon{ -1 };
	bool pistol_weapon_unlocked{ false };
	eid trickpistol_weapon{ -1 };
	bool trickpistol_weapon_unlocked{ false };
	eid dualpistol_weapon{ -1 };
	bool dualpistol_weapon_unlocked{ false };
	eid dualdiagpistol_weapon{ -1 };
	bool dualdiagpistol_weapon_unlocked{ false };
	eid dagger_weapon{ -1 };
	std::string current_coins{};

	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: Now entering main menu." << std::endl;

		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);
		//Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.5f, 0.2f, 0.04f, 0.04f, "Inventory", "courier");
		_title = Factory::Instance().FF_CreateGUISurface(title, 0.5f, 0.2f, original_dim_x, original_dim_y, 140);
		Factory::Instance().FF_CreateGUIChildSurfaceText(_title, { "transparent" }, 0.35f, 0.5f, 0.8f, 0.4f, "Inventory", "courier");
		current_coins = "[Coins: ";
		current_coins += std::to_string(_playerInv.coins);
		current_coins += "]";
		
		coins_text = Factory::Instance().FF_CreateGUIChildSurfaceText(_title, { "transparent" }, 0.65f, 0.5f, 0.8f, 0.4f, current_coins.c_str(), "courier");
		std::string current_weapon_text = _playerInv.Inventory_GetCurrentWeapon().GetWeapon_Name();
		current_weapon = Factory::Instance().FF_CreateGUIChildSurfaceText(_title, { "transparent" }, 0.5f, 0.6f, 0.8f, 0.4f, current_weapon_text, "courier");
		
		_buttons_surface = Factory::Instance().FF_CreateGUISurface(buttonbg, 0.5f, original_y, 0.9f, 0.6f, 120);
		eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.5f, 0.35f, 0.4f, 0.2f, ChangeTestScenePF, "Start", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();

		pistol_weapon = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button_dark, 0.30f, 0.5f, 0.5f, 0.2f, EquipPistol, "Pistol", "courier");
		Factory::Instance()[pistol_weapon].AddComponent<Com_GUISurfaceHoverShadow_Inventory>();
		trickpistol_weapon = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button_dark, 0.70f, 0.5f, 0.5f, 0.2f, EquipTrickPistol, "Trick Pistol", "courier");
		Factory::Instance()[trickpistol_weapon].AddComponent<Com_GUISurfaceHoverShadow_Inventory>();
		dualpistol_weapon = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button_dark, 0.30f, 0.65f, 0.5f, 0.2f, EquipDualPistol, "Dual Pistol", "courier");
		Factory::Instance()[dualpistol_weapon].AddComponent<Com_GUISurfaceHoverShadow_Inventory>();
		dualdiagpistol_weapon = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button_dark, 0.70f, 0.65f, 0.5f, 0.2f, EquipDualDiagPistol, "DD Pistol", "courier");
		Factory::Instance()[dualdiagpistol_weapon].AddComponent<Com_GUISurfaceHoverShadow_Inventory>();
		dagger_weapon = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_buttons_surface, button, 0.30f, 0.8f, 0.5f, 0.2f, EquipDagger, "Dagger", "courier");
		Factory::Instance()[dagger_weapon].AddComponent<Com_GUISurfaceHoverShadow>();

		if (pistol_weapon_unlocked)
			Factory::Instance()[pistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;

		if (trickpistol_weapon_unlocked)
			Factory::Instance()[trickpistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;

		if (dualpistol_weapon_unlocked)
			Factory::Instance()[dualpistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;

		if (dualdiagpistol_weapon_unlocked)
			Factory::Instance()[dualdiagpistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;


		// initialize gui settings
		GUISettingsInitialize();
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();

		Factory::Instance()[current_weapon].Get<Com_Text>()._data._text = _playerInv.Inventory_GetCurrentWeapon().GetWeapon_Name();	

		if (!pistol_weapon_unlocked && _playerInv.Inventory_CheckWeaponUnlocked("Pistol"))
		{
			pistol_weapon_unlocked = true;
			Factory::Instance()[pistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;
		}
		if (!trickpistol_weapon_unlocked && _playerInv.Inventory_CheckWeaponUnlocked("TrickPistol"))
		{
			trickpistol_weapon_unlocked = true;
			Factory::Instance()[trickpistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;
		}
		if (!dualpistol_weapon_unlocked && _playerInv.Inventory_CheckWeaponUnlocked("DualPistol"))
		{
			dualpistol_weapon_unlocked = true;
			Factory::Instance()[dualpistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;
		}
		if (!dualdiagpistol_weapon_unlocked && _playerInv.Inventory_CheckWeaponUnlocked("DualDiagPistol"))
		{
			dualdiagpistol_weapon_unlocked = true;
			Factory::Instance()[dualdiagpistol_weapon].Get<Com_GUISurfaceHoverShadow_Inventory>().weapon_unlocked = true;
		}

		current_coins = "[Coins: ";
		current_coins += std::to_string(_playerInv.coins);
		current_coins += "]";

		Factory::Instance()[coins_text].Get<Com_Text>()._data._text = current_coins.c_str();


		offset_rad = offset_rad + dt > 2.0f * PI ? 0.0f : offset_rad + dt;
		offset_y = sin(offset_rad);
		Factory::Instance()[_buttons_surface].Get<Com_GUISurface>()._position.y = original_y + offset_y * 0.03f;
		Factory::Instance()[_title].Get<Com_GUISurface>()._dimensions = { original_dim_x + (offset_y + 1.0f) * 0.03f, original_dim_y + (offset_y + 1.0f) * 0.03f };
	}
	void Exit() 
	{

	}
};