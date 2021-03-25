#pragma once
#include "SceneDeclarations.h"

struct TestScene : public Scene {
	/*
	Member Variables
	________________________________*/
	std::string test = "hello";
	eid player = -1;
	eid enemytest = -1;
	eid arrow = -1;
	Com_Sprite* arrow_sprite{ nullptr };
	//Com_Tilemap tile;
	eid tilemap = -1;
	//Factory::SpriteData data{ "heroidle.png", 100.0f, 160.0f, 2, 2, 4, 0.15f };
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData man{ "hero.png", 100.0f, 160.0f, 3, 3, 8, 0.1f, 0, passin };
	Factory::SpriteData dog{ "dog.png", 80.0f, 140.0f, 3, 3, 8, 0.1f, 0, passin };
	Vec2i passin2[5] = { {0,1},{2,3},{4,5},{6,7},{0,0} };
	Factory::SpriteData arrows{ "arrows.png", 50.0f, 50.0f, 3, 3, 8, 0.1f, -900, passin2 };
	/*Factory::SpriteData data22{ "coolguy", 130.0f, 200.0f, 3, 4, 10, 0.25f };
	Factory::SpriteData data3{ "box", 80.0f, 200.0f, 1, 1, 1, 10.0f };*/
	//Factory::SpriteData data{ 0,"test2", 1, 8, 8, 0.1f, 100.0f, 200.0f };
	//Inventory playerInv;
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

		Com_Tilemap& com_tilemap = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		Sys_PathFinding& pf2 = *SystemDatabase::Instance().GetSystem<Sys_PathFinding>();
		pf2._grid = Grid(com_tilemap._width, com_tilemap._height, com_tilemap._map);
		pf2._initialized = true;

		player = Factory::Instance().FF_SpriteTile(man, tilemap, 0, 0);
		Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_TileMoveSpriteState, Com_Health, Com_EnemyStateOne>();

		arrow = Factory::Instance().FF_Sprite(arrows, 0.0f, 0.0f);
		Entity& a = Factory::Instance()[arrow];
		a.AddComponent<Com_ParentPosition>();
		a.Get<Com_ParentPosition>()._parent_id = player;
		arrow_sprite = &a.Get<Com_Sprite>();

		/*enemytest = Factory::Instance().FF_SpriteTile(dog, tilemap, 5, 2);
		Factory::Instance()[enemytest].AddComponent<Com_YLayering, Com_EnemyStateOne, Com_FindPath, Com_EnemySpawn, Com_Wave, Com_type, Com_GridColData,Com_TileMoveSpriteState>();
		Factory::Instance()[enemytest].Get<Com_EnemyStateOne>()._player = &Factory::Instance()[player].Get<Com_TilePosition>();
		enemytest = Factory::Instance().FF_SpriteTile(dog, tilemap, 8, 3);
		Factory::Instance()[enemytest].AddComponent<Com_YLayering, Com_EnemyStateOne, Com_FindPath, Com_EnemySpawn, Com_Wave, Com_type, Com_GridColData, Com_TileMoveSpriteState>();
		Factory::Instance()[enemytest].Get<Com_EnemyStateOne>()._player = &Factory::Instance()[player].Get<Com_TilePosition>();
		enemytest = Factory::Instance().FF_SpriteTile(man, tilemap, 1, 1);
		Factory::Instance()[enemytest].AddComponent<Com_YLayering, Com_EnemyStateOne, Com_FindPath, Com_EnemySpawn, Com_Wave, Com_type, Com_GridColData, Com_TileMoveSpriteState>();
		Factory::Instance()[enemytest].Get<Com_EnemyStateOne>()._player = &Factory::Instance()[player].Get<Com_TilePosition>();
		enemytest = Factory::Instance().FF_SpriteTile(dog, tilemap, 5, 3);
		Factory::Instance()[enemytest].AddComponent<Com_YLayering, Com_EnemyStateOne, Com_FindPath, Com_EnemySpawn, Com_Wave, Com_type, Com_GridColData, Com_TileMoveSpriteState>();
		Factory::Instance()[enemytest].Get<Com_EnemyStateOne>()._player = &Factory::Instance()[player].Get<Com_TilePosition>();*/
		

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
		GUISettingsUpdate();
		if (AEInputCheckTriggered('L')) {
			Factory::Instance()[player].Get<Com_Sprite>()._current_frame_segment++;
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
		//if (AEInputCheckTriggered('N')) {
		//	/*std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i++ << std::endl;
		//	std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i << std::endl;*/
		//}
		//if (AEInputCheckTriggered('P')) {
		//	player = Factory::Instance().FF_Sprite(data, 100.0f, 100.0f);
		//	//Factory::Instance().GetEntity(player).Get<Com_Sprite>()._frame_interval -= dt;
		//	//Com_Sprite& s = player->Get<Com_Sprite>();
		//}
		//if (AEInputCheckTriggered('O')) {
		//	Factory::Instance()[player].AddComponent<Com_ArrowKeys>();
		//	Factory::Instance()[player].AddComponent<Com_YLayering>();
		//}
		//if (AEInputCheckTriggered('U')) {
		//	player = Factory::Instance().FF_Sprite({ "test2", 1, 8, 8, 0.2f, 50.0f, 80.0f }, 3.0f, 0.0f);
		//	Factory::Instance()[player].AddComponent<Com_ArrowKeys>();
		//}
		//if (AEInputCheckTriggered('M')) {
		//	Entity& test = Factory::Instance()[tilemap];
		//	Factory::Instance()[tilemap].AddComponent<Com_Position>();
		//	Factory::Instance()[tilemap].AddComponent<Com_ArrowKeys>();
		//}
		//if (AEInputCheckCurr('I')) {
		//	int e1 =
		//		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		//}
		//if (AEInputCheckTriggered('R')) {
		//	SceneManager::Instance().RestartScene();
		//}

		if (AEInputCheckTriggered(AEVK_G)) {
			SceneManager::Instance().ChangeScene("ShootingRange");
		}
		/*#if defined(DEBUG) | defined(_DEBUG)
			if (AEInputCheckTriggered(AEVK_G)) {
				playerInv.Inventory_PrintCurrentWeapon();
			}

			if (AEInputCheckTriggered(AEVK_H)) {
				std::cout << playerInv.Inventory_EquipWeapon("Pistol") << std::endl;
			}

			if (AEInputCheckTriggered(AEVK_F)) {
				std::cout << playerInv.Inventory_SetWeaponUnlocked("Pistol") << std::endl;
			}

			if (AEInputCheckTriggered(AEVK_D)) {
				playerInv.Inventory_GetCurrentWeapon().Weapon_Shoot({ static_cast<int>(Factory::Instance()[player].Get<Com_Position>().x), static_cast<int>(Factory::Instance()[player].Get<Com_Position>().y) }, Factory::Instance()[player].Get<Com_Direction>());
			}
		#endif*/
		if (AEInputCheckTriggered('R')) {
			SceneManager::Instance().RestartScene();
		}
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		std::cout << "woo switching to scene 2!" << std::endl;
	}
};