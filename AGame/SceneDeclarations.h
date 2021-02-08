#pragma once
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include <string>

/*___________________________________________________________________
	TEST SCENE - Created By : Zac
_____________________________________________________________________*/
struct TestScene : public Scene {
	std::string test = "hello";
	void Initialize() override {
		std::cout << test << " this is a test scene" << std::endl;
		Entity& entity = Factory::Instance().CreateEntity<Position, Example_Velocity>();
		entity.Get<Example_Velocity>() = { 1.0f,2.0f };
	}
	void Update(const float& dt) override {
		if (AEInputCheckCurr('L')) {
			SceneManager::Instance().ChangeScene("Test Scene 2");
		}
		if (AEInputCheckCurr('P')) {
			Entity& entity = Factory::Instance().CreateEntity<Position, Example_Velocity>();
			entity.Get<Example_Velocity>() = { 1.0f,2.0f };
		}
	}
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
		Factory::Instance().CreateEntity<Position>();
	}
	void Update(const float& dt) override {
		//std::cout << "hehe just keep printing" << std::endl;
	}
	// overriding initialize/update/exit is optional
};