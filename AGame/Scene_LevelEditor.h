#pragma once
#include "SceneDeclarations.h"
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
#include "zEntity.h"
#include "CSHeaderDef.h"
#include "Factory.h"
#include "ResourceManager.h"
#include "Inventory.h"
#include <string>

struct LevelEditor : public Scene {
	eid i{ -1 };
	eid main{ -1 };
	eid _settings{ -1 };
	eid _change_scene{ -1 };
	Factory::SpriteData data1{ "menubackground" };
	Factory::SpriteData data2{ "buttonsurface" };
	Factory::SpriteData data3{ "button1" };
	Factory::SpriteData data4{ "button2" };
	Factory::SpriteData data5{ "button3" };
	Factory::SpriteData data6{ "transparent" };
	Sys_Pathfinding_v2 _pathfinding;

	//test
	bool inputting{ false };
	std::vector<char> result;
	std::string input;
	int col;
	int row;
	std::string mapdata;
	std::string bcdata;


	bool _gui_change_scene{ false };
	void Initialize() override {
		std::cout << "SYSTEM MESSAGE: You're now entering the level editor." << std::endl;
		// main background
		main = Factory::Instance().FF_CreateGUISurface({ "background1" }, 0.5f, 0.5f, 1.0f, 1.0f, 100);																	// surface
		Factory::Instance().FF_CreateGUIChildSurfaceText(main, { "transparent" }, 0.5f, 0.2f, 0.04f, 0.04f, "AGame", "courier");									// settings button
		eid buttons = Factory::Instance().FF_CreateGUIChildSurface(main, { "background1" }, 0.5f, 0.6f, 0.3f, 0.4f);												// non clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.25f, 0.75f, 0.2f, SettingsButton, "Load", "courier");			// clickable child surface
		//Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.5f, 0.75f, 0.2f, SettingsButton, "Credits", "courier");		// clickable child surface
		Factory::Instance().FF_CreateGUIChildClickableSurfaceText(buttons, { "background1" }, 0.5f, 0.75f, 0.75f, 0.2f, QuitGame, "Save", "courier");				// clickable child surface

		//render text box
		
		//render 


		//level editor
		//	1. pass in col
		//	2. pass in row
		//	3. Render grid based
		//	4. click and click
		//	options :
		//enemy spawn
		//	player spawn
		//	wall
		//	bomb
		//	5. name, save

		// initialize gui settings
		GUISettingsInitialize();

		std::vector<int> flags = { 0,0,0,0,1,1,0,0,0 };
		Pathfinding::Grid grid{ 3,3,flags };
		std::vector<Vec2i> path;
		_pathfinding.SolveAStar({ 0,0 }, { 2,2 }, grid, path);
	}
	void Update(const float& dt) override {
		GUISettingsUpdate();



		//input 
		if (inputting) {
			//AEInputReset();
			if (AEInputCheckTriggered(AEVK_0)) {
				std::cout << "0" << std::endl;
				result.push_back('0');
			}
			if (AEInputCheckTriggered(AEVK_1)) {
				std::cout << "1" << std::endl;
				result.push_back('1');
			}
			if (AEInputCheckTriggered(AEVK_2)) {
				std::cout << "2" << std::endl;
				result.push_back('2');
			}
			if (AEInputCheckTriggered(AEVK_3)) {
				std::cout << "3" << std::endl;
				result.push_back('3');
			}
			if (AEInputCheckTriggered(AEVK_4)) {
				std::cout << "4" << std::endl;
				result.push_back('4');
			}
			if (AEInputCheckTriggered(AEVK_5)) {
				std::cout << "5" << std::endl;
				result.push_back('5');
			}
			if (AEInputCheckTriggered(AEVK_6)) {
				std::cout << "6" << std::endl;
				result.push_back('6');
			}
			if (AEInputCheckTriggered(AEVK_7)) {
				std::cout << "7" << std::endl;
				result.push_back('7');
			}
			if (AEInputCheckTriggered(AEVK_8)) {
				std::cout << "8" << std::endl;
				result.push_back('8');
			}
			if (AEInputCheckTriggered(AEVK_9)) {
				std::cout << "9" << std::endl;
				result.push_back('9');
			}
			if (AEInputCheckTriggered(AEVK_BACK) && result.size() != 0) {
				std::cout << "bspace" << std::endl;
				result.pop_back();
			}
			//end 
			if (AEInputCheckTriggered(AEVK_SPACE)) {
				//break;
				for (char x : result) {
					input += x;
				}
				std::cout << input << std::endl;
				inputting = false;
			}
		}
		//trigger on click 
		if (AEInputCheckTriggered(AEVK_0) && inputting == false) {
			std::cout << "entered" << std::endl;
			//reset result
			input.clear();
			result.clear();
			//for (size_t i{ 0 }; i < result.size(); ++i) {
			//	result.pop_back();
			//}
			inputting = true;
		}



	}


	////getting what user is typeing pass into string 
	//std::string getuserinput() {
	//	std::vector<char> result;
	//	std::string input;
	//	if (inputting) {
	//		//AEInputReset();
	//		if (AEInputCheckCurr(AEVK_0)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('0');
	//		}
	//		if (AEInputCheckCurr(AEVK_1)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('1');
	//		}
	//		if (AEInputCheckCurr(AEVK_2)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('2');
	//		}
	//		if (AEInputCheckCurr(AEVK_3)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('3');
	//		}
	//		if (AEInputCheckCurr(AEVK_4)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('4');
	//		}
	//		if (AEInputCheckCurr(AEVK_5)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('5');
	//		}
	//		if (AEInputCheckCurr(AEVK_6)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('6');
	//		}
	//		if (AEInputCheckCurr(AEVK_7)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('7');
	//		}
	//		if (AEInputCheckCurr(AEVK_8)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('8');
	//		}
	//		if (AEInputCheckCurr(AEVK_9)) {
	//			std::cout << "triggerd" << std::endl;
	//			result.push_back('9');
	//		}
	//		if (AEInputCheckCurr(AEVK_BACK) && result.size() != 0) {
	//			result.pop_back();
	//		}
	//		if (AEInputCheckCurr(AEVK_SPACE)) {
	//			//break;
	//		}
	//	}
	//	for (char x : result) {
	//		input += x;
	//	}
	//	std::cout << input << std::endl;
	//	return input;
	//}
};

