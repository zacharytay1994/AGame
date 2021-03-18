#pragma once
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include "ResourceManager.h"
#include "Inventory.h"
#include <string>

// GUI CODE
eid _settings, _change_scene;

void TestFunction(Com_GUISurface* surface) {
	std::cout << "button1" << std::endl;
}

void ToggleChangeSceneButton(Com_GUISurface* surface) {
	GlobalGameData::Instance()._gui._menu_ChangeScene = !GlobalGameData::Instance()._gui._menu_ChangeScene;
}

void SettingsButton(Com_GUISurface* surface) {
	GlobalGameData::Instance()._gui._menu_Settings = !GlobalGameData::Instance()._gui._menu_Settings;
}

void QuitGame(Com_GUISurface* surface) {
	std::cout << "button3" << std::endl;
	SceneManager::Instance().StopGame();
}

void ChangeTestScene(Com_GUISurface* surface) {
	SceneManager::Instance().ChangeScene("Test Scene");
}

void ChangeTestScenePF(Com_GUISurface* surface) {
	SceneManager::Instance().ChangeScene("Test PathFinding");
}

void ChangeShootingRangeScene(Com_GUISurface* surface) {
	SceneManager::Instance().ChangeScene("ShootingRange");
}

void ChangeWilf(Com_GUISurface* surface) {
	SceneManager::Instance().ChangeScene("TestScenewilfred");
}

void ChangeMainMenu(Com_GUISurface* surface) {
	SceneManager::Instance().ChangeScene("Main Menu");
}

void GUISettingsInitialize() {
	GlobalGameData::Instance()._gui._menu_ChangeScene = false;
	GlobalGameData::Instance()._gui._menu_Settings = false;
	Factory::Instance().FF_CreateGUIClickableSurface( { "settings" }, 0.96f, 0.04f, 0.04f, 0.04f, SettingsButton, 150);									// settings button
	// settings menu
	_settings = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.84f, 0.38f, 0.3f, 0.6f, 150);
	Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_settings, { "background1" }, 0.5f, 0.2f, 0.9f, 0.08f, ToggleChangeSceneButton, "Change Scene", "courier");	// clickable child surface text

	// change scene menu
	_change_scene = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.16f, 0.38f, 0.3f, 0.6f, 200);
	Factory::Instance()[_change_scene].AddComponent<Com_GUIDrag, Com_GUIMouseCheck>();
	Factory::Instance().FF_CreateGUIChildSurfaceText(_change_scene, { "transparent" }, 0.5f, 0.08f, 0.9f, 0.05f, "Select Scene", "courier");					// clickable child surface text
	Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.2f, 0.9f, 0.08f, ChangeMainMenu, "Main", "courier");	// clickable child surface text
	Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.35f, 0.9f, 0.08f, ChangeTestScenePF, "Aus", "courier");	// clickable child surface text
	Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.5f, 0.9f, 0.08f, ChangeShootingRangeScene, "Noel", "courier");	// clickable child surface text
	Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.65f, 0.9f, 0.08f, ChangeWilf, "Wilf", "courier");	// clickable child surface text
	Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, { "background1" }, 0.5f, 0.8f, 0.9f, 0.08f, ChangeTestScene, "Zac", "courier");	// clickable child surface text
	Factory::Instance().FF_CreateGUIChildClickableSurface(_change_scene, { "cross" }, 0.9f, 0.05f, 0.08f, 0.04f, ToggleChangeSceneButton);						// clickable child surface text
}

void GUISettingsUpdate() {
	Factory::Instance()[_settings].Get<Com_GUISurface>()._active = GlobalGameData::Instance()._gui._menu_Settings;
	Factory::Instance()[_change_scene].Get<Com_GUISurface>()._active = GlobalGameData::Instance()._gui._menu_ChangeScene;

}/* ___________________________________________________________________________________________________
  - EXAMPLE SCENE CREATION
  Step 1 : Create a new .h file prefixed with Scene_ (e.g. Scene_Example)
  Step 2 : In the newly created file, #include "SceneDeclarations.h"
  Step 2 : In Scene.cpp #include your newly created file, (e.g. #include "Scene_Example.h"
  Step 3 : Register your Scene as per normal.
 ___________________________________________________________________________________________________ */
// Example Scene
/*struct ExampleScene : public Scene {
	AEGfxTexture* scene_texture{ nullptr };	// scene persistent resource
	int* scene_variable;			// scene temporary resource
	void Load() override {
		scene_texture = AEGfxTextureLoad("Somerandomtexture.png");
		std::cout << "Example Scene Loaded." << std::endl;
	}
	void Initialize() override {
		// initializing
		scene_variable = new int{ 10 };
		std::cout << "Example Scene Initialized" << std::endl;
	}
	void Update(const float& dt) override {
		++(*scene_variable);
		if (AEInputCheckTriggered('R')) {
			SceneManager::Instance().RestartScene();
		}
		if (AEInputCheckTriggered('C')) {
			SceneManager::Instance().ChangeScene("Test Scene");
		}
	}
	void Draw(const float& dt) override {
		// draw smoething
	}
	void Exit() override {
		// free scene resources
		delete scene_variable;
		std::cout << "Example Scene Freed." << std::endl;
	}
	void Unload() override {
		//if (scene_texture) { AEGfxTextureUnload(scene_texture); }
		std::cout << "Example Scene Unloaded." << std::endl;
	}
};*/

//struct MainMenu;
//struct TestScene;

/*___________________________________________________________________
	TEST FOR PATHFINDING - Created By : Aus
_____________________________________________________________________*/
struct TestScenePF : public Scene
{
	/*
	Member Variables
	________________________________*/
	std::string test = "hello";
	eid player = -1;
	//Com_Tilemap tile;
	//eid tilemap = -1;
	eid tilemap = -1;
	Factory::SpriteData data{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	Factory::SpriteData data1{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.25f };
	Factory::SpriteData data2{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.15f };
	Factory::SpriteData data22{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.25f };
	Factory::SpriteData data3{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };
	//Factory::SpriteData data{ 0,"test2", 1, 8, 8, 0.1f, 100.0f, 200.0f };
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		std::cout << sizeof(Com_Tilemap) << std::endl;

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_tilemap = tilemap;
		//SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()._tilemap = tilemap;
		
		player = Factory::Instance().FF_SpriteTile(data1, tilemap, 9, 4);
		Factory::Instance()[player].AddComponent<Com_YLayering,Com_Node, Com_EnemySpawn, Com_Wave>();
		++Factory::Instance()[player].Get<Com_EnemySpawn>().CurrNoOfEnemies;

		player = Factory::Instance().FF_SpriteTile(data2, tilemap, 0, 0);
		Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap>();
		SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos =  player;
		//player = Factory::Instance().CreateEntity<Com_Position>();
		/*int* i = new int{ 0 };
		std::shared_ptr<int> a{ i };
		std::shared_ptr<int> b{ i };*/
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
		if (AEInputCheckTriggered('N')) {
			/*std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i++ << std::endl;
			std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i << std::endl;*/
		}
		if (AEInputCheckTriggered('P')) {
			player = Factory::Instance().FF_Sprite(data, 100.0f, 100.0f);
			//Factory::Instance().GetEntity(player).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = player->Get<Com_Sprite>();
		}
		if (AEInputCheckTriggered('O')) {
			Factory::Instance()[player].AddComponent<Com_ArrowKeys>();
			Factory::Instance()[player].AddComponent<Com_YLayering>();
		}
		
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
	Inventory playerInv; 
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
			playerInv.Inventory_PrintCurrentWeapon();
		}

		if (AEInputCheckTriggered(AEVK_H)) {
			playerInv.Inventory_SetWeaponUnlocked("Pistol");
			playerInv.Inventory_EquipWeapon("Pistol");
			std::cout << "EQUIPPED PISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_F)) {
			playerInv.Inventory_SetWeaponUnlocked("TrickPistol");
			playerInv.Inventory_EquipWeapon("TrickPistol");
			std::cout << "EQUIPPED TRICKPISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_S)) {
			playerInv.Inventory_SetWeaponUnlocked("DualPistol");
			playerInv.Inventory_EquipWeapon("DualPistol");
			std::cout << "EQUIPPED DUALPISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_A)) {
			playerInv.Inventory_SetWeaponUnlocked("DualDiagPistol");
			playerInv.Inventory_EquipWeapon("DualDiagPistol");
			std::cout << "EQUIPPED DUALDIAGPISTOL" << std::endl;
		}

		if (AEInputCheckTriggered(AEVK_D)) {
			playerInv.Inventory_GetCurrentWeapon().Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);
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