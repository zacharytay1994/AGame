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
	int e2 = -1;
	//Com_Tilemap tile;
	int tilemap = -1;
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		e2 = Factory::Instance().FF_Sprite("test2", 1, 8, 8, 0.1f, 50.0f, 80.0f);
		//tilemap = Factory::Instance().FF_Tilemap("tilemap", "c_test.txt", "t_test.txt");
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		if (AEInputCheckCurr('L')) {
			SceneManager::Instance().ChangeScene("Test Scene 2");
		}
		if (AEInputCheckCurr('P')) {
			Factory::Instance().FF_Sprite("test2", 1, 8, 8, 0.2f, 50.0f, 80.0f); // create sprite
			Factory::Instance().GetEntity(e2).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = e2->Get<Com_Sprite>();
		}
		if (AEInputCheckCurr('O')) {
			Factory::Instance().GetEntity(e2).AddComponent<Com_ArrowKeys>();
		}
		if (AEInputCheckCurr('I')) {
			Entity& e = Factory::Instance().GetEntity(e2);
			e.AddComponent<Com_Tilemap>();
			ResourceManager::Instance().GetResource(e.Get<Com_Tilemap>()._texture, e.Get<Com_Tilemap>()._mesh, "tilemap", 4, 4, 16);
			ResourceManager::Instance().ReadTilemapTxt("t_test.txt", e.Get<Com_Tilemap>());
			ResourceManager::Instance().ReadFloorMapTxt("c_test.txt", e.Get<Com_Tilemap>());
			Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
			e.Get<Com_Tilemap>()._scale_x = 50.0f;
			e.Get<Com_Tilemap>()._scale_y = 50.0f;
			e.Get<Com_Tilemap>()._initialized = true;
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

/*___________________________________________________________________
	TEST SCENE 3 - Created By : Aus
_____________________________________________________________________*/
struct TestScene3 : public Scene {
	std::string test = "hi";
	int e2 = -1;
	int e3[5] = { -1,-1,-1,-1,-1 };
	void Initialize() override {
		srand(100);
		e2 = Factory::Instance().FF_Sprite("test2", 1, 8, 8, 0.1f, 50.0f, 80.0f);
		for (int i{ 0 }; i < 5; ++i)
		{
			float randomPosx = static_cast<float>(rand() % 100) + 100.0f;
			float randomPosy = static_cast<float>(rand() % 100) + 20.0f;
			e3[i] = Factory::Instance().FF_SpriteRandomPosition("test", 1, 1, 1, 1.0f, 50.0f, 50.0f, randomPosx, randomPosy);
		}
		int i = 0;
		
	}
	void Update(const float& dt) override {
		
		/*Com_Sprite& s = Factory::Instance().GetEntity(e3[0]).Get<Com_Sprite>();
		if (!s._texture) {
			int i = 0;
		}*/
		if (AEInputCheckCurr('P')) {
			Factory::Instance().FF_Sprite("test2", 1, 8, 8, 0.2f, 50.0f, 80.0f); // create sprite
			//Factory::Instance().GetEntity(e2).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = e2->Get<Com_Sprite>();
		}	
		if (AEInputCheckCurr('O')) {
			Factory::Instance().GetEntity(e2).AddComponent<Com_ArrowKeys>();
		}
		if (AEInputCheckCurr('I')) {
			Entity& e = Factory::Instance().GetEntity(e2);
			e.AddComponent<Com_Tilemap>();
			ResourceManager::Instance().GetResource(e.Get<Com_Tilemap>()._texture, e.Get<Com_Tilemap>()._mesh, "tilemap", 4, 4, 16);
			ResourceManager::Instance().ReadTilemapTxt("t_test.txt", e.Get<Com_Tilemap>());
			ResourceManager::Instance().ReadFloorMapTxt("c_test.txt", e.Get<Com_Tilemap>());
			Com_Tilemap& tilemap = e.Get<Com_Tilemap>();
			e.Get<Com_Tilemap>()._scale_x = 50.0f;
			e.Get<Com_Tilemap>()._scale_y = 50.0f;
			e.Get<Com_Tilemap>()._initialized = true;
		}

		for (int j{ 0 }; j < 5; j++) 
		{
			Entity& enemy = Factory::Instance().GetEntity(e3[j]);
			enemy.Get<Com_Example_Velocity>().x += 0.0f; // replace with pathfinding
			enemy.Get<Com_Example_Velocity>().y += 0.0f;

			//warping so that enemy dont go out of bound
			if(enemy.Get<Com_Position>().x > AEGfxGetWinMaxX() || 
				enemy.Get<Com_Position>().x < AEGfxGetWinMinX() ||
				enemy.Get<Com_Position>().y > AEGfxGetWinMaxY() || 
				enemy.Get<Com_Position>().y < AEGfxGetWinMinY())
				{
					enemy.Get<Com_Position>().x = AEWrap((enemy.Get<Com_Position>()).x, AEGfxGetWinMinX(),	AEGfxGetWinMaxX());
					enemy.Get<Com_Position>().y = AEWrap((enemy.Get<Com_Position>()).y, AEGfxGetWinMinY(),	AEGfxGetWinMaxY());
					enemy.Get<Com_Example_Velocity>().x -= 0.0f;
					enemy.Get<Com_Example_Velocity>().y -= 0.0f;
				}
		}

	}
	// overriding initialize/update/exit is optional
};