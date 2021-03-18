#pragma once

class GlobalGameData {
	GlobalGameData() {};
public:
	static GlobalGameData& Instance();
	struct GUIData {
		bool _menu_ChangeScene{ false };
		bool _menu_Settings{ false };
	};
	GUIData _gui;
};