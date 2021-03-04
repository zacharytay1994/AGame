#pragma once
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include "ResourceManager.h"
#include <string>

struct testdata {
	int i;
};
void testfunction(Sys_Tilemap* t, testdata td) {
	t->i = td.i;
}
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
	Factory::SpriteData data{ "test2", 1, 8, 8, 0.1f, 100.0f, 200.0f };
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		std::cout << sizeof(Com_Tilemap) << std::endl;

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		//player = Factory::Instance().FF_SpriteMovableTile<Com_ArrowKeysTilemap>(data, tilemap, 0, 0);
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		//Entity& testing = Factory::Instance()[tilemap];
		//if (AEInputCheckTriggered('E')) {
		//}

		//if (AEInputCheckCurr('L')) {
		//	SceneManager::Instance().ChangeScene("Test Scene 2");
		//}
		if (AEInputCheckTriggered('N')) {
			std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i++ << std::endl;
			std::cout << SystemDatabase::Instance().GetSystem<Sys_Tilemap>().i << std::endl;
		}
		if (AEInputCheckTriggered('P')) {
			player = Factory::Instance().FF_SpriteTile({ "test2", 1, 8, 8, 0.2f, 50.0f, 80.0f }, tilemap, 3.0f, 0.0f);
			//Factory::Instance().GetEntity(player).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = player->Get<Com_Sprite>();
		}
		if (AEInputCheckTriggered('O')) {
			Factory::Instance()[player].AddComponent<Com_ArrowKeysTilemap>();
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
		e = Factory::Instance().FF_Sprite({ "test2", 1, 8, 8, 0.1f, 100.0f, 200.0f }, 0, 0);
	}
	void Update(const float& dt) override {
		//std::cout << "hehe just keep printing" << std::endl;
		if (AEInputCheckCurr('C')) {
			SceneManager::Instance().ChangeScene("Test Scene");
		}
	}
	// overriding initialize/update/exit is optional
};

// Example Code
struct ExampleScene : public Scene {
	AEGfxTexture* scene_texture;	// scene persistent resource
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



/*!___________________________________________________________________
	TEST SCENE - Created By : Wilfredddd
_____________________________________________________________________*/
struct TestScenewilfred : public Scene {
	/*
	Member Variables
	________________________________*/
	std::string test = "hello";
	eid e2 = -1;
	//Com_Tilemap tile;
	eid tilemap = -1;
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		std::cout << sizeof(Com_Tilemap) << std::endl;

		tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		e2 = Factory::Instance().FF_SpriteMovableTile<Com_ArrowKeysTilemap>({ "test2", 1, 8, 8, 0.1f, 100.0f, 200.0f }, tilemap, 0, 0);
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
			e2 = Factory::Instance().FF_SpriteMovableTile<Com_ArrowKeysTilemap>({ "test2", 1, 8, 8, 0.2f, 50.0f, 80.0f }, tilemap, 3.0f, 0.0f);
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
		}*/
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {
		std::cout << "woo switching to scene 2!" << std::endl;
	}
};