/******************************************************************************/
/*!
\File Name		: SceneDeclaration
\Project Name	: AGame
\Authors 		:
				Primary - Noel Ho (25%)
				Secondary -  Wilfred Ng (25%), Austen Ang (50%) , Zachary Tay (25%)

\brief		For the GUI Map and for the custom map gameplay 

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/
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
static int levelsunlocked{ 1 };
static int levelselector{ 0 };
static bool _change_scene_toggle{ false };
static bool _quit_confirmation{ false };
static eid _quit_confirmation_window{ -1 };

/**************************************************************************/
	/*!
	  \brief
		Turns on the settings menu in options
	*/
/**************************************************************************/
void ToggleChangeSceneButton(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_change_scene_toggle = !_change_scene_toggle;
}

/**************************************************************************/
	/*!
	  \brief
		Turns on the options menu
	*/
/**************************************************************************/
void SettingsButton(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_change_scene_toggle = false;
	// Pauses or unpauses the game
	SceneManager::Instance()._pause = !SceneManager::Instance()._pause;

	// Toggles the settings menu depending on the pause state
	SceneManager::Instance()._settings_toggle = SceneManager::Instance()._pause;
	_change_scene_toggle = false;
	_quit_confirmation = false;
	if (SceneManager::Instance()._inlevel == true) {
		SceneManager::Instance()._currentlyplaying = true;
	}
}

/**************************************************************************/
	/*!
	  \brief
		Confirms if you want to quit the game
	*/
/**************************************************************************/
void QuitConfirmation(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	if (!_quit_confirmation)
		_quit_confirmation = true;
	else
		SceneManager::Instance().StopGame();
}

/**************************************************************************/
	/*!
	  \brief
		Quits the game
	*/
/**************************************************************************/
void QuitGame(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	std::cout << "button3" << std::endl;
	SceneManager::Instance().StopGame();
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to Test Scene
	*/
/**************************************************************************/
void ChangeTestScene(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("Test Scene");
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to Inventory Scene
	*/
/**************************************************************************/
void ChangeInventoryScene(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("InventoryMenu");
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to Level Select scene
	*/
/**************************************************************************/
void ChangeLevelSelect(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	levelselector = 1;
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("LevelSelect");
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to Normal Level Select scene
	*/
/**************************************************************************/
void ChangeLevelSelectNormal(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	levelselector = 2;
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("LevelSelectNormal");
}

/**************************************************************************/
	/*!
	  \brief
		Changes Scene to Game scene if a weapon is equipped
	*/
/**************************************************************************/
void ChangeTestScenePF(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	if (_playerInv.Inventory_GetCurrentWeapon().GetWeapon_Name() != "NoWeapon") {
		//if it's custom 
		if (levelselector == 1) {
			SceneManager::Instance().ChangeScene("Test PathFinding");
		}
		if (levelselector == 2) {
			SceneManager::Instance().ChangeScene("Level");
		}
	}
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to shooting range scene
	*/
/**************************************************************************/
void ChangeShootingRangeScene(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("ShootingRange");
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to tutorial scene
	*/
/**************************************************************************/
void ChangeWilf(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("Instructions");
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to main menu scene
	*/
/**************************************************************************/
void ChangeMainMenu(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("Main Menu");
}

/**************************************************************************/
	/*!
	  \brief
		Toggles fullscreen on and off
	*/
/**************************************************************************/
void ToggleFullScreen(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance()._fullscreen = !SceneManager::Instance()._fullscreen;
	AEToogleFullScreen(SceneManager::Instance()._fullscreen);
}

/**************************************************************************/
	/*!
	  \brief
		Mutes and unmutes teh music
	*/
/**************************************************************************/
void ToggleMute(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	//ResourceManager::Instance().ToggleMuteMusic();
	if (SceneManager::Instance()._musicmmute == true) {
		SceneManager::Instance()._musicmmute = false;
		return;
	}
	if (SceneManager::Instance()._musicmmute == false) {
		SceneManager::Instance()._musicmmute = true;
		return;
	}
}

void ResetConfirmationyes(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	levelsunlocked = 1;
	//rewrite both custom and normals
	//normals
	std::ofstream file;
	assert(file);
	file.open("../bin/Assets/Tilemaps/leveltilemaps.txt"); //overwrite
	file << "level1";
	file.close();
	//customs
	std::ofstream filecustoms;
	assert(filecustoms);
	filecustoms.open("../bin/Assets/Tilemaps/tilemaps.txt"); //overwrite
	filecustoms << "Custom1";
	filecustoms.close();
	SceneManager::Instance().ChangeScene("Main Menu");
}

void ResetConfirmationno(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_change_scene_toggle = false;
	SceneManager::Instance()._settings_toggle = !SceneManager::Instance()._settings_toggle;
	SceneManager::Instance().ChangeScene("Main Menu");
}

void ResetProgress(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	eid i{ -1 };
	Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
	eid main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, .5f, 0.5f, 200);
	Factory::Instance()[main].AddComponent<Com_GUIDrag, Com_GUIMouseCheck>();
	Factory::SpriteData button{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(main, button, 0.5f, 0.25f, 0.9f, 0.08f, ResetConfirmationyes, "Reset(Yes)", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(main, button, 0.5f, 0.75f, 0.9f, 0.08f, ResetConfirmationno, "Reset(No)", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
}


/**************************************************************************/
	/*!
	  \brief
		Opens the survey link (and adds in some bonus coins)
	*/
/**************************************************************************/
void OpenSurvey(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	#ifdef _WIN32 
		system("start https://forms.gle/KPbjkFks2SYmj9af8");
	#elif __APPLE__ 
		system("open https://forms.gle/KPbjkFks2SYmj9af8");
	#elif __linux__ 
		system("xdg-open https://forms.gle/KPbjkFks2SYmj9af8");
	#endif
		_playerInv.Inventory_AddCoins(300);
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to credits scene
	*/
/**************************************************************************/
void Credits(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	_playerInv.Inventory_AddCoins(300);
	SceneManager::Instance().ChangeScene("Credits");
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to tutorial scene
	*/
/**************************************************************************/
void Instructions(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("Instructions");
}

/**************************************************************************/
	/*!
	  \brief
		Changes scene to Level Editor scene
	*/
/**************************************************************************/
void ChangeLevelEditor(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	SceneManager::Instance().ChangeScene("Leveleditor");
}

/**************************************************************************/
	/*!
	  \brief
		Initializes the GUI Settings interface
	*/
/**************************************************************************/
void GUISettingsInitialize() {
	Vec2i passin4[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "background2.png", 2.0f, 1.0f, 2, 1, 2, 0.05f, 0, passin4 };
	SceneManager::Instance()._settings_toggle = false;
	_change_scene_toggle = false;
	Vec2i passin[5] = { {0,0},{1,1},{0,0},{0,0},{0,0} };
	eid settings = Factory::Instance().FF_CreateGUIClickableSurface({ "settingsbutton.png", 1.0f, 1.0f, 2, 1, 2, 0.1f, 0, passin }, 0.96f, 0.04f, 0.04f, 0.04f, SettingsButton, 150);
	Factory::Instance()[settings].AddComponent<Com_GUISurfaceHoverShadow>();

	// settings menu
	_settings = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.84f, 0.38f, 0.3f, 0.6f, 150);
	eid i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_settings, button, 0.5f, 0.1f, 0.9f, 0.08f, SettingsButton, "Resume", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_settings, button, 0.5f, 0.3f, 0.9f, 0.08f, ChangeMainMenu, "Main Menu", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_settings, button, 0.5f, 0.5f, 0.9f, 0.08f, ToggleChangeSceneButton, "Settings", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_settings, button, 0.5f, 0.7f, 0.9f, 0.08f, OpenSurvey, "Survey", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_settings, button, 0.5f, 0.9f, 0.9f, 0.08f, QuitConfirmation, "Quit", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();

	// change scene menu
	_change_scene = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.16f, 0.38f, 0.3f, 0.6f, 200);
	Factory::Instance()[_change_scene].AddComponent<Com_GUIDrag, Com_GUIMouseCheck>();
	Factory::Instance().FF_CreateGUIChildSurfaceText(_change_scene, { "transparent" }, 0.5f, 0.08f, 0.9f, 0.05f, "Select Option", "courier");		
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, button, 0.5f, 0.2f, 0.9f, 0.08f, ToggleFullScreen, "Fullscreen", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();// clickable child surface text
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, button, 0.5f, 0.35f, 0.9f, 0.08f, ToggleMute, "Mute Music", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
	i = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(_change_scene, button, 0.5f, 0.5f, 0.9f, 0.08f, ResetProgress, "Reset Progress", "courier");	// clickable child surface text
	Factory::Instance()[i].AddComponent<Com_GUISurfaceHoverShadow>();
	Factory::Instance().FF_CreateGUIChildClickableSurface(_change_scene, { "cross" }, 0.9f, 0.05f, 0.08f, 0.04f, ToggleChangeSceneButton);					// clickable child surface text

	// quit game confirmation menu
	Factory::SpriteData title{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	_quit_confirmation_window = Factory::Instance().FF_CreateGUISurface(title, 0.5f, 0.2f, 0.8f, 0.3f, 990);
	Factory::Instance().FF_CreateGUIChildSurfaceText(_quit_confirmation_window, { "transparent" }, 0.5f, 0.5f, 0.9f, 0.05f, "Confirm?", "courier");
	Factory::Instance()[_quit_confirmation_window].Get<Com_GUISurface>()._active = false;
}

/**************************************************************************/
	/*!
	  \brief
		Update function for the GUI Settings Interface
	*/
/**************************************************************************/
void GUISettingsUpdate() {
	// Escape key opens and closes the settings menu
	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		// Pauses or unpauses the game
		SceneManager::Instance()._pause = !SceneManager::Instance()._pause;

		// Toggles the settings menu depending on the pause state
		SceneManager::Instance()._settings_toggle = SceneManager::Instance()._pause;
		_change_scene_toggle = false;
		_quit_confirmation = false;

		SceneManager::Instance()._currentlyplaying = false;

		//draws back cursor 
		if (SceneManager::Instance()._inlevel == true) {
			if (SceneManager::Instance()._settings_toggle == true) {
				SceneManager::Instance()._currentlyplaying = false;
			}
			//removes cursor
			if (SceneManager::Instance()._settings_toggle == false) {
				SceneManager::Instance()._currentlyplaying = true;
			}
		}
	}

	// Shows the QUIT CONFIRMATION text if the user pressed the QUIT button once
	if (_quit_confirmation)
	{
		Factory::Instance()[_quit_confirmation_window].Get<Com_GUISurface>()._active = true;
	}
	else
	{
		Factory::Instance()[_quit_confirmation_window].Get<Com_GUISurface>()._active = false;
	}

	Factory::Instance()[_settings].Get<Com_GUISurface>()._active = SceneManager::Instance()._settings_toggle;
	Factory::Instance()[_change_scene].Get<Com_GUISurface>()._active = _change_scene_toggle;
}

// WEAPON STUFF
/**************************************************************************/
	/*!
	  \brief
		Unlocks the pistol (if player can afford), and equips it
	*/
/**************************************************************************/
void EquipPistol(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	_playerInv.Inventory_SetWeaponUnlocked("Pistol");
	_playerInv.Inventory_EquipWeapon("Pistol");
}

/**************************************************************************/
	/*!
	  \brief
		Unlocks the trick pistol (if player can afford), and equips it
	*/
/**************************************************************************/
void EquipTrickPistol(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	_playerInv.Inventory_SetWeaponUnlocked("TrickPistol");
	_playerInv.Inventory_EquipWeapon("TrickPistol");
}

/**************************************************************************/
	/*!
	  \brief
		Unlocks the dual pistol (if player can afford), and equips it
	*/
/**************************************************************************/
void EquipDualPistol(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	_playerInv.Inventory_SetWeaponUnlocked("DualPistol");
	_playerInv.Inventory_EquipWeapon("DualPistol");
}

/**************************************************************************/
	/*!
	  \brief
		Unlocks the dual diagonal pistol (if player can afford), and equips it
	*/
/**************************************************************************/
void EquipDualDiagPistol(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	_playerInv.Inventory_SetWeaponUnlocked("DualDiagPistol");
	_playerInv.Inventory_EquipWeapon("DualDiagPistol");
}

/**************************************************************************/
	/*!
	  \brief
		Unlocks the dagger (if player can afford), and equips it
	*/
/**************************************************************************/
void EquipDagger(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	_quit_confirmation = false;
	_playerInv.Inventory_SetWeaponUnlocked("Dagger");
	_playerInv.Inventory_EquipWeapon("Dagger");
}


/*___________________________________________________________________
	Level for custom maps
_____________________________________________________________________*/
struct TestScenePF : public Scene
{
	/*
	Member Variables
	________________________________*/
	eid player = -1;
	eid enemytest = -1;
	eid tilemap = -1;
	eid lives{ -1 };
	eid waves{ -1 };
	eid spawner{ -1 };
	eid menu{ -1 };
	eid _WinOrLose{ -1 };
	eid wall{ -1 };
	eid bomb{ -1 };
	eid bossy{ -1 };

	//inventory
	Inventory playerInv;

	//sprite data 
	Factory::SpriteData box{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };
	Factory::SpriteData boom{ "kaboom", 40.0f, 40.0f, 1, 1, 1, 0.15f };
	Vec2i passin[5] = { {-1,3},{3,7},{8,11},{0,0},{0,0} };
	Factory::SpriteData man{ "hero.png", 200.0f, 320.0f, 4, 3, 12, 0.1f, 0, passin };
	Factory::SpriteData underline{ "underline.png", 80.0f, 200.0f, 4, 1, 4, 0.25f };
	Vec2i passin2[5] = { {0,1},{2,3},{4,5},{6,7},{0,0} };
	Factory::SpriteData arrows{ "arrows.png", 50.0f, 50.0f, 3, 3, 8, 0.1f, -900, passin2 };
	Factory::SpriteData title{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	eid arrow = -1;
	Com_Sprite* arrow_sprite{ nullptr };
	Factory::SpriteData buttonsurface{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Vec2i passin3[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin3 };

	//map data 
	bool once = false, checkBoss = false;
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		//for cursor disable
		SceneManager::Instance()._currentlyplaying = true;
		SceneManager::Instance()._inlevel = true;
		
		//reset map data
		once = false;
		checkBoss = false;

		//init tilemap 
		tilemap = Factory::Instance().FF_Tilemap("tilemap", ResourceManager::Instance()._tilemap_names[ResourceManager::Instance()._tilemap_id]._binary + ".txt",
															ResourceManager::Instance()._tilemap_names[ResourceManager::Instance()._tilemap_id]._map + ".txt");
		Factory::Instance()[tilemap].Get<Com_Position>().x = -5;
		Factory::Instance()[tilemap].Get<Com_Position>().y = 3;
		Factory::Instance()[tilemap].Get<Com_Tilemap>()._render_pack._layer = -1000;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_tilemap = tilemap;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_tilemap = tilemap;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateBoss>()->_tilemap = tilemap;

		//spawning of monsters
		spawner = Factory::Instance().FF_CreateSpawner();
		//init grid
		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		pf2._initialized = true;
		SystemDatabase::Instance().GetSystem<Sys_TilePosition>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_AABB>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_ArrowKeysTilemap>()->_grid = &pf2._grid;
		SystemDatabase::Instance().GetSystem<Sys_AABB>()->_spawner = &Factory::Instance()[spawner].Get<Com_EnemySpawn>();
		Factory::Instance()[spawner].Get<Com_Boss>().BossHealth = 20;
		SystemDatabase::Instance().GetSystem<Sys_AABB>()->Boss = &Factory::Instance()[spawner].Get<Com_Boss>();
		SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->boss = &Factory::Instance()[spawner].Get<Com_Boss>();
		SystemDatabase::Instance().GetSystem<Sys_EnemyStateBoss>()->boss = &Factory::Instance()[spawner].Get<Com_Boss>(); 

		//creating based on location of object
		for (int y = 0; y < com_tilemap._height; ++y) {
			for (int x = 0; x < com_tilemap._width; ++x) {
				//if it's a player spawn location 
				if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 2) {
					player = Factory::Instance().FF_SpriteTile(man, tilemap, x, y);
					Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Health, Com_EnemyStateOne, Com_TileMoveSpriteState, Com_type, Com_BoundingBox, Com_Velocity, Com_CollisionData>();
					Factory::Instance()[player].Get<Com_TilePosition>()._is_player = true;
					Factory::Instance()[player].Get<Com_type>().type = 0; // set player type
					SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->tile = tilemap;
					SystemDatabase::Instance().GetSystem<Sys_PathFinding>()->playerPos = player;
					SystemDatabase::Instance().GetSystem<Sys_EnemyStateOne>()->_player_id = player;
					SystemDatabase::Instance().GetSystem<Sys_EnemyStateBoss>()->player = player;

					SystemDatabase::Instance().GetSystem<Sys_AABB>()->_PLayerHealth = &Factory::Instance()[player].Get<Com_Health>();
					SystemDatabase::Instance().GetSystem<Sys_EnemySpawning>()->playerpos = player;

				}
				//if its' a destructible wall 
				if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 4) {
					wall = Factory::Instance().FF_SpriteTile(box, tilemap, x, y);
					Factory::Instance()[wall].AddComponent<Com_YLayering,Com_Health,Com_type, Com_BoundingBox, Com_Velocity, Com_CollisionData, Com_ParticleEmitter>();
					SystemDatabase::Instance().GetSystem<Sys_Obstacle>()->_grid = &pf2._grid;
					Entity& e = Factory::Instance()[wall];
					e.Get<Com_Health>().health = 3;
					e.Get<Com_type>().type = 3;
					continue;
				}
				//if it's a explosive barrel 
				if (com_tilemap._map[x * (size_t)com_tilemap._height + y] == 5) {
					bomb = Factory::Instance().FF_SpriteTile(boom, tilemap, x, y);
					Factory::Instance()[bomb].AddComponent<Com_YLayering, Com_type, Com_Health, Com_BoundingBox, Com_Velocity, Com_CollisionData, Com_ParticleEmitter, Com_GameTimer>();
					SystemDatabase::Instance().GetSystem<Sys_ParticleEmitter>()->tilemap = tilemap;
					SystemDatabase::Instance().GetSystem<Sys_Obstacle>()->_grid = &pf2._grid; 
					Entity& e = Factory::Instance()[bomb];
					e.Get<Com_Health>().health = 1;
					e.Get<Com_type>().type = 4;
					continue;
				}
			}
		}
		//init arrow
		arrow = Factory::Instance().FF_Sprite(arrows, 0.0f, 0.0f);
		Entity& a = Factory::Instance()[arrow];
		a.AddComponent<Com_ParentPosition>();
		a.Get<Com_ParentPosition>()._parent_id = player;
		arrow_sprite = &a.Get<Com_Sprite>();
		//init lives
		lives = Factory::Instance().FF_CreateGUISurface(underline, 0.2f, 0.1f, 0.4f, 0.1f, 100);
		Factory::Instance().FF_CreateGUIChildSurfaceText(lives, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Lives: ", "courier");
		std::stringstream ss;
		ss << Factory::Instance()[player].Get<Com_Health>().health;
		lives = Factory::Instance().FF_CreateGUIChildSurfaceText(lives, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss.str().c_str(), "courier");
		//init waves
		waves = Factory::Instance().FF_CreateGUISurface(underline, 0.8f, 0.1f, 0.4f, 0.1f, 100);
		std::stringstream ss1;
		ss1 << Factory::Instance()[spawner].Get<Com_Wave>().numberofwaves;
		waves = Factory::Instance().FF_CreateGUIChildSurfaceText(waves, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss1.str().c_str(), "courier");
		//init boss
		bossy = Factory::Instance().FF_CreateGUISurface(underline, 0.8f, 0.1f, 0.4f, 0.1f, 100);
		std::stringstream ss2;
		ss2 << Factory::Instance()[spawner].Get<Com_Boss>().BossHealth;
		bossy = Factory::Instance().FF_CreateGUIChildSurfaceText(bossy, { "transparent" }, 0.5f, 0.5f, 0.8f, 0.4f, ss2.str().c_str(), "courier");
		//init menu
		menu = Factory::Instance().FF_CreateGUISurface(buttonsurface, 0.5f, 0.5f, 0.9f, 0.6f, 120);
		//init win or lose  screen
		_WinOrLose = Factory::Instance().FF_CreateGUISurface(title, 0.5f, 0.2f, 0.8f, 0.3f, 140);
		eid start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.35f, 0.4f, 0.2f, ChangeTestScenePF, "Restart", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		start = Factory::Instance().FF_CreateGUIChildClickableSurfaceText(menu, button, 0.5f, 0.60f, 0.4f, 0.2f, ChangeMainMenu, "Main Menu", "courier");
		Factory::Instance()[start].AddComponent<Com_GUISurfaceHoverShadow>();
		Factory::Instance()[menu].Get<Com_GUISurface>()._active = false;
		Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = false;
		Factory::Instance()[bossy].Get<Com_GUISurface>()._active = false;
		//gui init
		GUISettingsInitialize();
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		GUISettingsUpdate();
		//if paused
		if (AEInputCheckTriggered(AEVK_P)) {
			SceneManager::Instance()._pause = !SceneManager::Instance()._pause;
		}
		//get enemy info
		Com_Boss& bs = Factory::Instance()[spawner].Get<Com_Boss>();
		Com_Wave& com_wave = Factory::Instance()[spawner].Get<Com_Wave>();
		Com_EnemySpawn& em = Factory::Instance()[spawner].Get<Com_EnemySpawn>();
		//display player health
		std::stringstream ss;
		ss << Factory::Instance()[player].Get<Com_Health>().health;
		Factory::Instance()[lives].Get<Com_Text>()._data._text = ss.str();
		//display waves
		if (com_wave.numberofwaves > 0 && em.CurrNoOfEnemies > 0 && checkBoss == false) 
		{
			Factory::Instance().FF_CreateGUIChildSurfaceText(waves, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Waves: ", "courier");
			checkBoss = true;
		}
		// for wave
		std::stringstream ss1;
		ss1 << Factory::Instance()[spawner].Get<Com_Wave>().numberofwaves;
		Factory::Instance()[waves].Get<Com_Text>()._data._text = ss1.str();
		//display boss hp
		if (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && checkBoss ==true)
		{
			Factory::Instance().FF_CreateGUIChildSurfaceText(bossy, { "transparent" }, 0.3f, 0.5f, 0.4f, 0.4f, "Boss HP:   ", "courier");
			Factory::Instance()[waves].Get<Com_GUISurface>()._active = false;
			Factory::Instance()[bossy].Get<Com_GUISurface>()._active = true;
			checkBoss = false;
		}
		std::stringstream ss2;
		ss2 << Factory::Instance()[spawner].Get<Com_Boss>().BossHealth;
		Factory::Instance()[bossy].Get<Com_Text>()._data._text = ss2.str();
		//screen shake
		if (AEInputCheckCurr('L')) {
			ResourceManager::Instance()._screen_shake = 1.0f;
		}
		//disable boss
		if (AEInputCheckTriggered('R')) {
			bs.disable = 0;
			bs.bossdefeat = false;
			bs.BossHealth = 20;
			SceneManager::Instance().RestartScene();
		}
		//pausing of gameplay
		Com_Sprite& sprite = Factory::Instance()[player].Get<Com_Sprite>();
		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			SceneManager::Instance()._pause = !SceneManager::Instance()._pause;
			SceneManager::Instance()._settings_toggle = SceneManager::Instance()._pause;
		}
		//shooting 
		if (AEInputCheckTriggered(AEVK_SPACE) && !SceneManager::Instance()._pause) {
			const_cast<Weapon*>(&_playerInv.Inventory_GetCurrentWeapon())->Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);
			sprite._lock = true;
			sprite._current_frame = 0;
			sprite._frame_interval_counter = 0.0f;
			sprite._current_frame_segment = 2;
		}

		//movement of player 
		if (AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_A)) {
			sprite._flip = true;
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 0;
		}
		else if (AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_W)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 1;
		}
		else if (AEInputCheckCurr(AEVK_RIGHT) || AEInputCheckCurr(AEVK_D)) {
			sprite._flip = false;
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 2;
		}
		else if (AEInputCheckCurr(AEVK_DOWN) || AEInputCheckCurr(AEVK_S)) {
			arrow_sprite->_visible = true;
			arrow_sprite->_current_frame_segment = 3;
		}
		else if (AEInputCheckTriggered(AEVK_Z) && !SceneManager::Instance()._pause) {
			const_cast<Weapon*>(&_playerInv.Inventory_GetCurrentSecondaryWeapon())->Weapon_Shoot({ Factory::Instance()[player].Get<Com_TilePosition>()._grid_x, Factory::Instance()[player].Get<Com_TilePosition>()._grid_y }, Factory::Instance()[player].Get<Com_Direction>(), tilemap);
		}
		else {
			arrow_sprite->_visible = false;
		}

		//if lose
		if (Factory::Instance()[player].Get<Com_Health>().health <= 0 && once == false)
		{
			Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Lose :(", "courier");
			_playerInv.Inventory_AddCoins(25);
			once = true;
			SceneManager::Instance()._currentlyplaying = false;
		}
		//if won
		else if (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <= 0 && bs.bossdefeat == true && once == false)
		{
			Factory::Instance().FF_CreateGUIChildSurfaceText(_WinOrLose, { "transparent" }, 0.5f, 0.4f, 0.8f, 0.4f, "You Win :D", "courier");
			_playerInv.Inventory_AddCoins(50);
			once = true;
			SceneManager::Instance()._currentlyplaying = false;
		}
		//if lose or won
		if (Factory::Instance()[player].Get<Com_Health>().health <= 0 || (com_wave.numberofwaves <= 0 && em.CurrNoOfEnemies <=0 && bs.bossdefeat == true)) {
			Factory::Instance()[menu].Get<Com_GUISurface>()._active = true;
			Factory::Instance()[_WinOrLose].Get<Com_GUISurface>()._active = true;
		}
		//gui init
		GUISettingsUpdate();
	}	
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		//reenable cursor
		SceneManager::Instance()._currentlyplaying = false;
		SceneManager::Instance()._inlevel = false;

	}
};
