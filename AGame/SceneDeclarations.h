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

//#include "zMath.h"

/*___________________________________________________________________
	MENU - Created By : Aus
_____________________________________________________________________*/
struct Menu : public Scene 
{
	std::string test = "hi";
	eid e;
	Factory::SpriteData data{ "test2", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	void Initialize() override {
		std::cout << test << " Welcome, press enter to start" << std::endl;
		e = Factory::Instance().FF_Sprite(data, 0, 0);

	}
	void Update(const float& dt) override {
		//std::cout << "hehe just keep printing" << std::endl;
		if (AEInputCheckTriggered(VK_RETURN)) {
			SceneManager::Instance().ChangeScene("Test PathFinding");
		}
	}
};

/*!___________________________________________________________________
	TEST SCENE - Created By : Zac
_____________________________________________________________________*/
struct TestScene : public Scene {
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

		//tilemap = Factory::Instance().FF_Sprite(data, 0.0f, 0.0f);

		//SystemDatabase::Instance().GetSystem<Sys_GUISurfaceOnClick>()->_left_mouse = true;

		player = Factory::Instance().FF_SpriteTile(data, tilemap, 5, 2);
		Factory::Instance()[player].AddComponent<Com_YLayering>();
		player = Factory::Instance().FF_SpriteTile(data2, tilemap, 8, 3);
		Factory::Instance()[player].AddComponent<Com_YLayering>();
		player = Factory::Instance().FF_SpriteTile(data22, tilemap, 1, 1);
		Factory::Instance()[player].AddComponent<Com_YLayering>();
		player = Factory::Instance().FF_SpriteTile(data1, tilemap, 0, 0);
		Factory::Instance()[player].AddComponent<Com_YLayering, Com_ArrowKeysTilemap>();
		player = Factory::Instance().FF_SpriteTile(data3, tilemap, 5, 3);
		Factory::Instance()[player].AddComponent<Com_YLayering>();

		//player = Factory::Instance().CreateEntity<Com_Position>();
		/*int* i = new int{ 0 };
		std::shared_ptr<int> a{ i };
		std::shared_ptr<int> b{ i };*/
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
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		std::cout << "woo switching to scene 2!" << std::endl;
	}
};

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
		//std::cout << "hehe just keep printing" << std::endl;
		if (AEInputCheckCurr('C')) {
			SceneManager::Instance().ChangeScene("Test Scene");
		}
	}
	// overriding initialize/update/exit is optional
};

/*___________________________________________________________________
	TEST SCENE 3 - Created By : Aus
_____________________________________________________________________*/
struct TestScene3 : public Scene
{
	std::string test = "hi";
	eid e2 = -1;
	eid e3[5] = { -1,-1,-1,-1,-1 };
	//Com_Tilemap tile;
	eid tilemap = -1;
	Factory::SpriteData data{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	void Initialize() override {
		srand(100);

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		Entity& testing = Factory::Instance()[tilemap];
		Com_Tilemap& test = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		e2 = Factory::Instance().FF_SpriteTile(data, tilemap, 0, 0);
		//e2 = Factory::Instance().FF_Sprite({ "test2", 1, 8, 8, 0.1f , 50.0f, 80.0f }, 0 ,0);
		Com_Sprite& test2 = Factory::Instance()[e2].Get<Com_Sprite>();
		for (int i{ 0 }; i < 1; ++i)
		{
			float randomPosx = static_cast<float>(rand() % 4);
			float randomPosy = static_cast<float>(rand() % 3);
			e3[i] = Factory::Instance().FF_SpriteTile(data, tilemap, 4, 3);

		}
		int i = 0;

	}
	void Update(const float& dt) override {

		/*Com_Sprite& s = Factory::Instance().GetEntity(e3[0]).Get<Com_Sprite>();
		if (!s._texture) {
			int i = 0;
		}*/
		if (AEInputCheckCurr('L')) {
			SceneManager::Instance().ChangeScene("ExampleScene");
		}
		if (AEInputCheckTriggered('P')) {
			e2 = Factory::Instance().FF_SpriteTile(data, tilemap, 3.0f, 0.0f);
			//Factory::Instance().GetEntity(e2).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = e2->Get<Com_Sprite>();
		}
		if (AEInputCheckTriggered('O')) {
			Factory::Instance()[e2].AddComponent<Com_ArrowKeys>();
		}
		if (AEInputCheckTriggered('M')) {
			Entity& test = Factory::Instance()[tilemap];
			Factory::Instance()[tilemap].AddComponent<Com_Position>();
			Factory::Instance()[tilemap].AddComponent<Com_ArrowKeys>();
		}
		/*if (AEInputCheckCurr('I')) {
			Entity& e = Factory::Instance().GetEntity(e2);
			e.AddComponent<Com_Tilemap>();
			ResourceManager::Instance().GetResource(e.Get<Com_Tilemap>()._texture, e.Get<Com_Tilemap>()._mesh, "tilemap", 4, 4, 16);
			ResourceManager::Instance().ReadTilemapTxt("t_test.txt", e.Get<Com_Tilemap>());
			ResourceManager::Instance().ReadFloorMapTxt("c_test.txt", e.Get<Com_Tilemap>());
			Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
			e.Get<Com_Tilemap>()._scale_x = 100.0f;
			e.Get<Com_Tilemap>()._scale_y = 100.0f;
			e.Get<Com_Tilemap>()._initialized = true;
		}*/
		if (AEInputCheckTriggered('R')) {
			SceneManager::Instance().RestartScene();
		}

		for (int j{ 0 }; j < 1; j++)
		{
			Entity& enemy = Factory::Instance().GetEntity(e3[j]);
			Com_TilePosition& tilemap = enemy.Get<Com_TilePosition>();

			//warping so that enemy dont go out of bound
			if (enemy.Get<Com_Position>().x <= tilemap._grid_x)
			{
				enemy.Get<Com_Position>().x -= 1;
			}
			else if (enemy.Get<Com_Position>().x >= 0)
			{
				enemy.Get<Com_Position>().x += 1;
			}
			else if (enemy.Get<Com_Position>().y <= tilemap._grid_y)
			{
				enemy.Get<Com_Position>().y -= 1;
			}
			else if (enemy.Get<Com_Position>().y >= 0)
			{
				enemy.Get<Com_Position>().y += 1;
			}
		}

	}
};
	// overriding initialize/update/exit is optional

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
};

void TestFunction(Com_GUISurface* surface) {
	std::cout << "button1" << std::endl;
}

void TestFunction2(Com_GUISurface* surface) {
	std::cout << "button2" << std::endl;
}

void TestFunction3(Com_GUISurface* surface) {
	std::cout << "button3" << std::endl;
	SceneManager::Instance().ChangeScene("Test Scene");
}

struct MainMenu : public Scene {
	eid i{ -1 };
	eid parent{ -1 };
	Factory::SpriteData data1{ "menubackground" };
	Factory::SpriteData data2{ "buttonsurface" };
	Factory::SpriteData data3{ "button1" };
	Factory::SpriteData data4{ "button2" };
	Factory::SpriteData data5{ "button3" };
	Factory::SpriteData data6{ "transparent" };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: Now entering main menu." << std::endl;
		//i = Factory::Instance().FF_CreateGUISurface(data1, 0.5f, 0.5f, 1.0f, 1.0f);									// surface
		//parent = i;
		//i = Factory::Instance().FF_CreateGUIChildSurface(i, data2, 0.5f, 0.6f, 0.3f, 0.4f);							// non clickable child surface
		//Factory::Instance().FF_CreateGUIChildClickableSurface(i, data3, 0.5f, 0.25f, 0.75f, 0.2f, TestFunction);	// clickable child surface
		//Factory::Instance().FF_CreateGUIChildClickableSurface(i, data4, 0.5f, 0.5f, 0.75f, 0.2f, TestFunction2);	// clickable child surface
		//Factory::Instance().FF_CreateGUIChildClickableSurface(i, data5, 0.5f, 0.75f, 0.75f, 0.2f, TestFunction3);	// clickable child surface
		i = Factory::Instance().FF_CreateGUISurfaceText(data6, 0.5f, 0.3f, 0.4f, 0.3f, "Something", "strawberry");
		parent = i;
		i = Factory::Instance().FF_CreateGUIChildSurfaceText(i, data6, 0.2f, 0.6f, 0.4f, 0.4f, "test2", "strawberry");
		i = Factory::Instance().FF_CreateGUIChildSurfaceText(i, data6, 0.2f, 0.6f, 0.4f, 0.4f, "test2", "strawberry");
		i = Factory::Instance().FF_CreateGUIChildSurfaceText(i, data6, 0.8f, 0.8f, 0.4f, 0.4f, "test2", "strawberry");
	}
	void Update(const float& dt) override {
		if (AEInputCheckTriggered('C')) {
			SceneManager::Instance().ChangeScene("Test Scene");
		}
		if (AEInputCheckTriggered('P')) {
			Factory::Instance()[parent].Get<Com_GUISurface>()._active = false;
		}
		if (AEInputCheckTriggered('O')) {
			Factory::Instance()[parent].Get<Com_GUISurface>()._active = true;
		}
		if (AEInputCheckCurr(VK_LEFT)) {
			Factory::Instance()[parent].Get<Com_GUISurface>()._position.x -= 1.0f * dt;
		}
		if (AEInputCheckCurr(VK_RIGHT)) {
			Factory::Instance()[parent].Get<Com_GUISurface>()._position.x += 1.0f * dt;
		}
		if (AEInputCheckCurr(VK_UP)) {
			Factory::Instance()[parent].Get<Com_GUISurface>()._position.y -= 1.0f * dt;
		}
		if (AEInputCheckCurr(VK_DOWN)) {
			Factory::Instance()[parent].Get<Com_GUISurface>()._position.y += 1.0f * dt;
		}
	}
};

/*___________________________________________________________________
	TEST FOR PATHFINDING - Created By : Aus
_____________________________________________________________________*/
struct TestScenePF : public Scene
{
	std::string test = "hi";
	eid e2 = -1;
	eid e3 = -1;
	//Com_Tilemap tile;
	eid tilemap = -1;
	Factory::SpriteData data{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	void Initialize() override {
		srand(100);

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		Entity& testing = Factory::Instance()[tilemap];
		Com_Tilemap& test = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		e2 = Factory::Instance().FF_SpriteTile(data, tilemap, 0, 0);
		e3 = Factory::Instance().FF_SpriteTile(data, tilemap, 4, 3);
		//e2 = Factory::Instance().FF_Sprite({ "test2", 1, 8, 8, 0.1f , 50.0f, 80.0f }, 0 ,0);
		Com_Sprite& test2 = Factory::Instance()[e2].Get<Com_Sprite>();

	}
	void Update(const float& dt) override
	{

		/*Com_Sprite& s = Factory::Instance().GetEntity(e3[0]).Get<Com_Sprite>();
		if (!s._texture) {
			int i = 0;
		}*/
		if (AEInputCheckTriggered('N')) {
			//std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i++ << std::endl;
			//std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i << std::endl;
		}
		if (AEInputCheckTriggered('P')) {
			e2 = Factory::Instance().FF_SpriteTile(data, tilemap, 3.0f, 0.0f);
			//Factory::Instance().GetEntity(player).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = player->Get<Com_Sprite>();
		}
		if (AEInputCheckTriggered('O')) {
			Factory::Instance()[e2].AddComponent<Com_ArrowKeysTilemap>();
			std::cout << Factory::Instance()[e2].Get<Com_TilePosition>()._grid_x << std::endl;
			std::cout << Factory::Instance()[e2].Get<Com_TilePosition>()._grid_y << std::endl;
		}



	}
};

/*!___________________________________________________________________
	TEST SCENE - Created By : Wilfredddd
_____________________________________________________________________*/
struct TestScenewilfred : public Scene 
{
	/*
	Member Variables
	________________________________*/
	std::string test = "hello";
	eid e2 = -1;
	//Com_Tilemap tile;
	eid tilemap = -1;
	Factory::SpriteData data{ "skeleton", 100.0f, 160.0f, 2, 3, 8, 0.15f };
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		std::cout << sizeof(Com_Tilemap) << std::endl;

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		e2 = Factory::Instance().FF_SpriteMovableTile<Com_ArrowKeysTilemap>(data, tilemap, 0, 0);
		//init to add component to the entity/adding own logic 
		Factory::Instance()[e2].AddComponent<Com_WeaponAttack, Com_Projectile>();
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		//Entity& testing = Factory::Instance()[tilemap];
		if (AEInputCheckCurr('L')) {
			SceneManager::Instance().ChangeScene("Test Scene 2");
		}
		if (AEInputCheckTriggered('P')) {
			e2 = Factory::Instance().FF_SpriteMovableTile<Com_ArrowKeysTilemap>(data, tilemap, 3, 0);
		}
		/*if (AEInputCheckTriggered('O')) {
			Factory::Instance()[e2].AddComponent<Com_ArrowKeysTilemap>();

		}
		if (AEInputCheckTriggered('M')) {
			Entity& test = Factory::Instance()[tilemap];
			Factory::Instance()[tilemap].AddComponent<Com_Position>();
			Factory::Instance()[tilemap].AddComponent<Com_ArrowKeys>();
		}
		if (AEInputCheckCurr('I')) {
			Entity& e = Factory::Instance().GetEntity(e2);
			e.AddComponent<Com_Tilemap>();
			ResourceManager::Instance().GetResource(e.Get<Com_Tilemap>()._texture, e.Get<Com_Tilemap>()._mesh, "tilemap", 4, 4, 16);
			ResourceManager::Instance().ReadTilemapTxt("t_test.txt", e.Get<Com_Tilemap>());
			ResourceManager::Instance().ReadFloorMapTxt("c_test.txt", e.Get<Com_Tilemap>());
			Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
			e.Get<Com_Tilemap>()._scale_x = 100.0f;
			e.Get<Com_Tilemap>()._scale_y = 100.0f;
			e.Get<Com_Tilemap>()._initialized = true;
		}
		if (AEInputCheckTriggered('R')) {
			SceneManager::Instance().RestartScene();
		}

		}*/
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		std::cout << "woo switching to scene 2!" << std::endl;
	}
};