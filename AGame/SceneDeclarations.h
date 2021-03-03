#pragma once
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include "ResourceManager.h"
#include <string>

/*!___________________________________________________________________
	TEST SCENE - Created By : Zac
_____________________________________________________________________*/
struct TestScene : public Scene {
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
		Entity& testing = Factory::Instance()[tilemap];
		Com_Tilemap& test = Factory::Instance()[tilemap].Get<Com_Tilemap>();
		e2 = Factory::Instance().FF_SpriteTile({ "test2", 1, 8, 8, 0.1f, 100.0f, 200.0f }, tilemap, 0, 0);
		Com_TilemapRef& ref = Factory::Instance()[e2].Get<Com_TilemapRef>();
		Com_Sprite& test2 = Factory::Instance()[e2].Get<Com_Sprite>();

		//tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
		/*e2 = Factory::Instance().FF_Sprite({ "test2", 1, 8, 8, 0.1f, 100.0f, 200.0f }, 0, 0);
		Entity& entitytest = Factory::Instance()[e2];
		Com_Sprite& cs = entitytest.Get<Com_Sprite>();
		Com_Position& pos = entitytest.Get<Com_Position>();*/
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		Entity& testing = Factory::Instance()[tilemap];
		if (AEInputCheckCurr('L')) {
			SceneManager::Instance().ChangeScene("ExampleScene");
		}
		if (AEInputCheckTriggered('P')) {
			Factory::Instance().FF_Sprite({ "test2", 1, 8, 8, 0.2f, 50.0f, 80.0f }, 0.0f, 0.0f);
			//Factory::Instance().GetEntity(e2).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = e2->Get<Com_Sprite>();
		}
		if (AEInputCheckCurr('O')) {
			Factory::Instance()[e2].AddComponent<Com_ArrowKeys>();
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
	void Initialize() override {
		std::cout << test << " i came from test scene 1" << std::endl;
		Factory::Instance().CreateEntity<Com_Position>();
	}
	void Update(const float& dt) override {
		//std::cout << "hehe just keep printing" << std::endl;
	}
	// overriding initialize/update/exit is optional
};

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