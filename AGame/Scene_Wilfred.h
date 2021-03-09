#pragma once
#include "SceneDeclarations.h"

/*!___________________________________________________________________
	TEST SCENE - Created By : Wilfredddd
_____________________________________________________________________*/
struct TestScenewilfred : public Scene
{
	/*
Member Variables
________________________________*/
	std::string test = "hello";
	eid player = -1;
	//Com_Tilemap tile;
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
		//SystemDatabase::Instance().GetSystem<Sys_Projectile>().tilemap = tilemap;

		player = Factory::Instance().FF_SpriteTile(data, tilemap, 5, 2);
		Factory::Instance()[player].AddComponent<Com_YLayering>();

		player = Factory::Instance().FF_SpriteTile(data2, tilemap, 8, 3);
		Factory::Instance()[player].AddComponent<Com_YLayering>();

		player = Factory::Instance().FF_SpriteTile(data22, tilemap, 1, 1);
		Factory::Instance()[player].AddComponent<Com_YLayering>();

		player = Factory::Instance().FF_SpriteTile(data1, tilemap, 0, 0);
		Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap, Com_Projectile, Com_WeaponAttack, Com_GameTimer>();
		Entity& e = Factory::Instance()[player];

		player = Factory::Instance().FF_SpriteTile(data3, tilemap, 5, 3);
		Factory::Instance()[player].AddComponent<Com_YLayering, Com_GameTimer, Com_objecttype, Com_BoundingBox, Com_CollisionData, Com_Velocity>();
		Entity& e1 = Factory::Instance()[player];
		e1.Get<Com_objecttype>().objtype = Com_objecttype::enemyt;

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
#		endif*/
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