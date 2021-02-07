#pragma once
#include <iostream>
#include "Scene.h"
#include "AEEngine.h"
//#include "../Extern/AlphaEngine_V3.08/include/AEEngine.h"

/*___________________________________________________________________
	TEST SCENE - Created By : Zac
_____________________________________________________________________*/
void TS1_Initialize() {
	std::cout << "TS1::Initialize()" << std::endl;
}

void TS1_Update(const float& dt) {
	std::cout << "TS1::Update()" << std::endl;
	if (AEInputCheckTriggered('L')) {
		SceneManager::Instance().ChangeScene("Test Scene 2");
	}
}

void TS1_Exit() {
	std::cout << "TS1::Exit()" << std::endl;
}

/*___________________________________________________________________
	TEST SCENE 2 - Created By : Zac
_____________________________________________________________________*/
void TS2_Initialize() {
	std::cout << "TS2::Initialize()" << std::endl;
}

void TS2_Update(const float& dt) {
	std::cout << "TS2::Update()" << std::endl;
}

void TS2_Exit() {
	std::cout << "TS2::Exit()" << std::endl;
}