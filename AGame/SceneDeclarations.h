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
	int tilemap;
	/*
	Initialize Override (optional)
	________________________________*/
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		/*Entity& entity = Factory::Instance().CreateEntity<Com_Position, Com_Example_Velocity>();
		entity.Get<Com_Example_Velocity>() = { 1.0f,2.0f };*/
		e2 = Factory::Instance().FF_Sprite("test2", 1, 8, 8, 0.1f, 50.0f, 80.0f);
		e2 = Factory::Instance().FF_Sprite("test2", 1, 8, 8, 0.1f, 50.0f, 80.0f);
		/*tile._width = 3;
		tile._height = 4;
		tile._map.resize(12);
		for (int i = 0; i < 12; ++i) {
			tile._map[i] = 1;
		}*/
		//ResourceManager::Instance().WriteTilemapTxt("test2.txt", tile);
		/*ResourceManager::Instance().ReadTilemapTxt("test.txt", tile);
		ResourceManager::Instance().WriteTilemapBin("test.bin", tile);
		Com_Tilemap tile2;
		ResourceManager::Instance().ReadTilemapBin("test.bin", tile2);
		for (int y = 0; y < tile2._height; ++y) {
			for (int x = 0; x < tile2._width; ++x) {
				std::cout << (char)tile2._map[x * tile2._height + y];
			} 
			std::cout << "\n";
		}
		int i = 0;*/
		tilemap = Factory::Instance().FF_Tilemap("test");
		std::vector<Com_Sprite*> vref = SystemDatabase::Instance().GetAllComponents<Com_Sprite>();
	}
	/*
	Update Override (optional)
	________________________________*/
	void Update(const float& dt) override {
		if (AEInputCheckCurr('L')) {
			SceneManager::Instance().ChangeScene("Test Scene 2");
		}
		if (AEInputCheckCurr('P')) {
			Factory::Instance().FF_Sprite("test2", 1, 8, 8, 0.2f, 50.0f, 80.0f);
			Factory::Instance().GetEntity(e2).Get<Com_Sprite>()._frame_interval -= dt;
			//Com_Sprite& s = e2->Get<Com_Sprite>();
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