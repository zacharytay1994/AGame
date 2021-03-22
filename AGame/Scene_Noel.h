#pragma once
#include "SceneDeclarations.h"

/*!___________________________________________________________________
	SHOOTING RANGE - Created By : Noel
_____________________________________________________________________*/
struct ShootingRange : public Scene {
	/*
	Member Variables
	________________________________*/
	eid player = -1;
	//Com_Tilemap tile;
	eid tilemap = -1;
	Factory::SpriteData data{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.25f };
	Factory::SpriteData data1{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };
	//Inventory playerInv;
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << std::endl << "WELCOME TO THE SHOOTING RANGE (Please leave your shoes to the side)" << std::endl;
		std::cout << sizeof(Com_Tilemap) << std::endl;

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;

		//tilemap = Factory::Instance().FF_Sprite(data, 0.0f, 0.0f);

		//SystemDatabase::Instance().GetSystem<Sys_GUISurfaceOnClick>()->_left_mouse = true;

		player = Factory::Instance().FF_SpriteTile(data, tilemap, 5, 2);
		Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap>();
		//player = Factory::Instance().FF_SpriteTile(data1, tilemap, 0, 0);
		//Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap>();
		GUISettingsInitialize();
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		//Entity& testing = Factory::Instance()[tilemap];
		//if (AEInputCheckTriggered('E')) {
		//}

		if (AEInputCheckCurr('L')) {
			SceneManager::Instance().ChangeScene("Test Scene 2");
		}

#if defined(DEBUG) | defined(_DEBUG)
		if (AEInputCheckTriggered(AEVK_G)) {
			_playerInv.Inventory_PrintCurrentWeapon();
		}

		if (AEInputCheckTriggered(AEVK_F)) {
			_playerInv.Inventory_SetWeaponUnlocked("Pistol");
			_playerInv.Inventory_EquipWeapon("Pistol");
			std::cout << "EQUIPPED PISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_H)) {
			_playerInv.Inventory_SetWeaponUnlocked("TrickPistol");
			_playerInv.Inventory_EquipWeapon("TrickPistol");
			std::cout << "EQUIPPED TRICKPISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_S)) {
			_playerInv.Inventory_SetWeaponUnlocked("DualPistol");
			_playerInv.Inventory_EquipWeapon("DualPistol");
			std::cout << "EQUIPPED DUALPISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_A)) {
			_playerInv.Inventory_SetWeaponUnlocked("DualDiagPistol");
			_playerInv.Inventory_EquipWeapon("DualDiagPistol");
			std::cout << "EQUIPPED DUALDIAGPISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_B)) {
			_playerInv.Inventory_SetWeaponUnlocked("Dagger");
			_playerInv.Inventory_EquipWeapon("Dagger");
			std::cout << "EQUIPPED DAGGER" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_D)) {
			_playerInv.Inventory_GetCurrentWeapon().Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);
		}
#endif
		if (AEInputCheckTriggered('R')) {
			SceneManager::Instance().RestartScene();
		}
		GUISettingsUpdate();
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		std::cout << "woo switching to scene 2!" << std::endl;
	}
};