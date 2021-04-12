#pragma once
#include "SceneDeclarations.h"

//static variables for the level selector of normal maps 
static std::string current_tilemap2{ "" };
static AEGfxTexture* _current_texture2{ nullptr };

//browse right button 
void BrowseLevelRight2(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	//switch tilemap
	current_tilemap2 = ResourceManager::Instance().SwitchTilemap2(1);
	_current_texture2 = ResourceManager::Instance()._tilemap_images2[ResourceManager::Instance()._tilemap_id2];
	//if unable to get texture 
	if (!_current_texture2) {
		_current_texture2 = ResourceManager::Instance().GetTexture("noimage");
	}
}

//browse left button
void BrowseLevelLeft2(Com_GUISurface* surface) {
	UNREFERENCED_PARAMETER(surface);
	//switch tilemap 
	current_tilemap2 = ResourceManager::Instance().SwitchTilemap2(-1);
	_current_texture2 = ResourceManager::Instance()._tilemap_images2[ResourceManager::Instance()._tilemap_id2];
	//if unable to get texture 
	if (!_current_texture2) {
		_current_texture2 = ResourceManager::Instance().GetTexture("noimage");
	}
}

struct LevelSelectNormal : public Scene {
	//member variables 
	eid _background{ -1 };
	eid _right_button{ -1 };
	eid _left_button{ -1 };
	eid _enter_level{ -1 };
	eid _name{ -1 };

	//sprite texture data 
	Factory::SpriteData background{ "title.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Factory::SpriteData rightbutton{ "rightbutton.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Factory::SpriteData leftbutton{ "leftbutton.png", 1.0f, 1.0f, 2, 2, 4, 0.2f, 0 };
	Vec2i passin[5] = { {0,3},{4,7},{0,0},{0,0},{0,0} };
	Factory::SpriteData button{ "buttonsprite.png", 1.0f, 1.0f, 3, 3, 8, 0.1f, 0, passin };

	//varables for current level selected 
	Com_Text* _name_text{ nullptr };
	Com_Sprite* _lvl_display{ nullptr };
	bool skip = false;
	bool first = true;

	void Initialize() override {

		//unlock level 2 
		if (levelsunlocked == 2) {
			//double check if the file name already exist 
			std::ifstream filecheck;
			filecheck.open("../bin/Assets/Tilemaps/leveltilemaps.txt");
			std::string tmp;
			while (std::getline(filecheck, tmp)) {
				//already exist! 
				if (tmp == "level2") {
					skip = true;
				}
			}

			// open text file
			if (skip == false) {
				std::ofstream file;
				assert(file);
				file.open("../bin/Assets/Tilemaps/leveltilemaps.txt", std::ios_base::app); // append instead of overwrite
				file << "\n" << "level2";
				file.close();
			}
			skip = false;
		}

		//unlock level 3 
		if (levelsunlocked == 3) {
			//double check if the file name already exist 
			std::ifstream filecheck;
			filecheck.open("../bin/Assets/Tilemaps/leveltilemaps.txt");
			std::string tmp;
			while (std::getline(filecheck, tmp)) {
				//already exist! 
				if (tmp == "level3") {
					skip = true;
				}
			}

			// open text file
			if (skip == false) {
				std::ofstream file;
				assert(file);
				file.open("../bin/Assets/Tilemaps/leveltilemaps.txt", std::ios_base::app); // append instead of overwrite
				file << "\n" << "level3";
				file.close();
			}
			skip = false;
		}

		// read all tilemaps
		ResourceManager::Instance().ReadTilemapNames2();
		current_tilemap = ResourceManager::Instance()._tilemap_names2[0]._name;
		_current_texture = ResourceManager::Instance()._tilemap_images2[0];

		// main background
		_background = Factory::Instance().FF_CreateGUISurface(background, 0.5f, 0.5f, 0.6f, 0.6f, 1);
		_background = Factory::Instance().FF_CreateGUIChildSurface(_background, { "tilemaps/test.png" }, 0.5f, 0.5f, 0.5f, 0.5f);
		_right_button = Factory::Instance().FF_CreateGUIClickableSurface(rightbutton, 0.85f, 0.5f, 0.1f, 0.5f, BrowseLevelRight2, 1);
		_left_button = Factory::Instance().FF_CreateGUIClickableSurface(leftbutton, 0.15f, 0.5f, 0.1f, 0.5f, BrowseLevelLeft2, 1);
		_name = Factory::Instance().FF_CreateGUISurfaceText(background, 0.5f, 0.25f, 0.5f, 0.08f, current_tilemap, "courier", 1);
		Factory::Instance().FF_CreateGUISurfaceText(background, 0.5f, 0.1f, 0.7f, 0.2f, "Select a Level", "courier", 1);
		_enter_level = Factory::Instance().FF_CreateGUIClickableSurfaceText(button, 0.5f, 0.85f, 0.3f, 0.1f, ChangeInventoryScene, "Enter", "courier", 1);
		Factory::Instance()[_enter_level].AddComponent<Com_GUISurfaceHoverShadow>();

		//level display and name 
		_lvl_display = &Factory::Instance()[_background].Get<Com_Sprite>();
		_name_text = &Factory::Instance()[_name].Get<Com_Text>();

		//if it the first tile map next once 
		if (first == true) {
			current_tilemap2 = ResourceManager::Instance().SwitchTilemap2(-1);
			_current_texture2 = ResourceManager::Instance()._tilemap_images2[ResourceManager::Instance()._tilemap_id2];
			if (!_current_texture2) {
				_current_texture2 = ResourceManager::Instance().GetTexture("noimage");
			}
			first = false;
		}

		// initialize gui settings
		GUISettingsInitialize();

	}
	void Update(const float& dt) override {
		UNREFERENCED_PARAMETER(dt);
		//switching of tile maps 
		if (_name_text->_data._text != current_tilemap2) {
			_name_text->_data._text = current_tilemap2;
		}
		//swithcing of tile map textures 
		if (_lvl_display->_render_pack._texture != _current_texture2) {
			_lvl_display->_render_pack._texture = _current_texture2;
		}
		//GUI update 
		GUISettingsUpdate();
	}
	/*
	Exit Override (optional)
	________________________________*/
	void Exit() override {

	}
};