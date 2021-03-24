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
#include <sstream>

// GUI CODE
eid _settings, _change_scene;

// INVENTORY CODE
static Inventory _playerInv;

//void TestFunction(Com_GUISurface* surface) {
//	std::cout << "button1" << std::endl;
//}

static bool _change_scene_toggle{ false };
void ToggleChangeSceneButton(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_change_scene_toggle = !_change_scene_toggle;
}

static bool _settings_toggle{ false };
void SettingsButton(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_settings_toggle = !_settings_toggle;
}

void QuitGame(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "button3" << std::endl;
	SceneManager::Instance().StopGame();
}

void ChangeTestScene(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().ChangeScene("Test Scene");
}

void ChangeTestScenePF(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().ChangeScene("Test PathFinding");
}

void ChangeShootingRangeScene(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().ChangeScene("ShootingRange");
}

void ChangeWilf(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().ChangeScene("TestScenewilfred");
}

void ChangeMainMenu(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	SceneManager::Instance().ChangeScene("Main Menu");
}

void GUISettingsInitialize() {
	_settings_toggle = false;
	_change_scene_toggle = false;
	Vec2i passin[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
	eid settings = Factory::Instance().FF_CreateGUIClickableSurface({ "settingsbutton.png", 1.0f, 1.0f, 2, 1, 2, 0.1f, 0, passin }, 0.96f, 0.04f, 0.04f, 0.04f, SettingsButton, 150);
	Factory::Instance()[settings].AddComponent<Com_GUISurfaceHoverShadow>();
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
	Factory::Instance()[_settings].Get<Com_GUISurface>()._active = _settings_toggle;
	Factory::Instance()[_change_scene].Get<Com_GUISurface>()._active = _change_scene_toggle;
}

/*!___________________________________________________________________
	TEST SCENE - Created By : Zac
_____________________________________________________________________*/

/*___________________________________________________________________
	TEST SCENE 2 - Created By : Zac
_____________________________________________________________________*/
struct TestScene2 : public Scene {
	std::string test = "hi";
	eid e;
	void Initialize() override {
		std::cout << test << " i came from test scene 1" << std::endl;
		e = Factory::Instance().FF_Sprite({ "test2", 50.0f, 100.0f, 1, 8, 8 }, 0, 0);
	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		//std::cout << "hehe just keep printing" << std::endl;
		if (AEInputCheckCurr('C')) {
			SceneManager::Instance().ChangeScene("Test Scene");
		}
	}
	// overriding initialize/update/exit is optional
};

// Example Code
struct ExampleScene : public Scene {
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
		UNREFERENCED_PARAMETER(dt);
		++(*scene_variable);
		if (AEInputCheckTriggered('R')) {
			SceneManager::Instance().RestartScene();
		}
		if (AEInputCheckTriggered('C')) {
			SceneManager::Instance().ChangeScene("Test Scene");
		}
	}
	void Draw(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
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
};

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
	eid enemytest = -1;
	eid tilemap = -1;
	eid lives{ -1 };
	eid waves{ -1 };
	eid spawner{ -1 };
	eid menu{ -1 };
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData man{ "hero.png", 100.0f, 160.0f, 3, 3, 8, 0.1f, 0, passin };
	Factory::SpriteData data{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	Factory::SpriteData data1{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.25f };
	Factory::SpriteData data2{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.15f };
	Factory::SpriteData data22{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.25f };
	Factory::SpriteData underline{ "underline.png", 80.0f, 200.0f, 4, 1, 4, 0.25f };
	Factory::SpriteData clock{ "clock.png", 80.0f, 200.0f, 3, 2, 5, 0.20f };
	Vec2i passin2[5] = { {0,1},{2,3},{4,5},{6,7},{0,0} };
	Factory::SpriteData arrows{ "arrows.png", 50.0f, 50.0f, 3, 3, 8, 0.1f, -900, passin2 };
	eid arrow = -1;
	Com_Sprite* arrow_sprite{ nullptr };
	//Factory::SpriteData data{ 0,"test2", 1, 8, 8, 0.1f, 100.0f, 200.0f };

	Factory::SpriteData buttonsurface{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Vec2i passin3[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin3 };
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		std::cout << sizeof(Com_Tilemap) << std::endl;

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test2.txt", "t_test2.txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 2;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_tilemap = tilemap;

		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		pf2._initialized = true;
		SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_grid = &pf2._grid;

		player = Factory::Instance().FF_SpriteTile(man, tilemap, 0, 0);
		Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState>();
		Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
		SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;

		arrow = Factory::Instance().FF_Sprite(arrows, 0.0f, 0.0f);
		Entity& a = Factory::Instance()[arrow];
		a.AddComponent<Com_ParentPosition>();
		a.Get<Com_ParentPosition>()._parent_id = player;
		arrow_sprite = &a.Get<Com_Sprite>();

		spawner = Factory::Instance().FF_CreateSpawner();
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->playerpos = player;

		lives = Factory::Instance().FF_CreateGUISurface(underline, 0.2f, 0.1f, 0.4f, 0.1f, 100);
		Factory::Instance().FF_CreateGUIChildSurfaceText(lives, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Lives: ", "courier");
		std::stringstream ss;
		ss << Factory::Instance()[player].Get<Com_Health>().health;
		lives = Factory::Instance().FF_CreateGUIChildSurfaceText(lives, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss.str().c_str(), "courier");

		waves = Factory::Instance().FF_CreateGUISurface(underline, 0.8f, 0.1f, 0.4f, 0.1f, 100);
		Factory::Instance().FF_CreateGUIChildSurfaceText(waves, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Waves: ", "courier");
		std::stringstream ss1;
		ss1 << Factory::Instance()[spawner].Get<Com_Wave>().numberofwaves;
		waves = Factory::Instance().FF_CreateGUIChildSurfaceText(waves, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss1.str().c_str(), "courier");

		menu = Factory::Instance().FF_CreateGUISurface(buttonsurface, 0.5f, 0.5f, 0.9f, 0.6f, 120);
		eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.35f, 0.4f, 0.2f, ChangeTestScenePF, "Restart", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.60f, 0.4f, 0.2f, ChangeMainMenu, "Main Menu", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		Factory::Instance()[menu].Get<Com_GUISurface>()._active = false;

		//Factory::Instance().FF_CreateGUISurface(clock, 0.5f, 0.05f, 0.1f, 0.1f, 100);

		GUISettingsInitialize();
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		//Entity& testing = Factory::Instance()[tilemap];
		//if (AEInputCheckTriggered('E')) {
		//}
		std::stringstream ss;
		ss << Factory::Instance()[player].Get<Com_Health>().health;
		Factory::Instance()[lives].Get<Com_Text>()._data._text = ss.str();
		std::stringstream ss1;
		ss1 << Factory::Instance()[spawner].Get<Com_Wave>().numberofwaves;
		Factory::Instance()[waves].Get<Com_Text>()._data._text = ss1.str();

		if (AEInputCheckCurr('L')) {
			ResourceManager::Instance()._screen_shake = 1.0f;
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
		if (AEInputCheckCurr(AEVK_LEFT)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 0;
		}
		else if (AEInputCheckCurr(AEVK_UP)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 1;
		}
		else if (AEInputCheckCurr(AEVK_RIGHT)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 2;
		}
		else if (AEInputCheckCurr(AEVK_DOWN)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 3;
		}
		else {
			arrow_sprite->_visible = false;
		}

		if (Factory::Instance()[player].Get<Com_Health>().health <= 0) {
			Factory::Instance()[menu].Get<Com_GUISurface>()._active = true;
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
