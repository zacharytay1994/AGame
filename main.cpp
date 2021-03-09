// ---------------------------------------------------------------------------
// includes
#include "AEEngine.h"
#include "AGame/zEntity.h"
#include "AGame/zSystem.h"
#include "AGame/Scene.h"
#include "AGame/CSHeaderDef.h"

#include <iostream>
#include <string>

//struct Vec2 {
//	float x = 0;
//	float y = 0;
//};
//
//struct Position {
//	Vec2 _pos = { 0,0 };
//};
//
//struct Velocity {
//	Vec2 _vel = { 1,1 };
//};
//
//void UpdatePosition(System& s) {
//	s.c<Position>()._pos.x += s.c<Velocity>()._vel.x * s._dt;
//	s.c<Position>()._pos.y += s.c<Velocity>()._vel.y * s._dt;
//}
//
//void PrintPosition(System& s) {
//	std::cout << "Entity: " << s._current_id << " : " << s.c<Position>()._pos.x << " | " << s.c<Position>()._pos.y << std::endl;
//}

// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	///////////////////////////////////////////////////////////////////////////
	// DEBUGGER			(2 /2)
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	// END OF DEBUGGER	(2 /2)
	///////////////////////////////////////////////////////////////////////////

	///////////////////////
	// Variable declaration

	int gGameRunning = 1;
	
	/*Vec2f a{ 1.0f, 1.0f };
	Vec2f b{ 2.0f,2.0f };*/
	//std::cout << "hgello" << std::endl;
	//std::cout << a.x << ":" << a.y << std::endl;
 	//std::cout << a << std::endl;
	/////////////////
	// Initialization

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);


	// Changing the window title
	AESysSetWindowTitle("A Beautiful Window!");

	// reset the system modules
	AESysReset();

	SceneManager::Instance();
	SceneManager::Instance().ChangeScene("Main Menu");
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		///////////////////
		// Game loop update
		SceneManager::Instance().CheckGame(gGameRunning);
		SceneManager::Instance().Update((float)AEFrameRateControllerGetFrameTime());
		// Game loop update end
		///////////////////////

		//////////////////
		// Game loop draw

		// Game loop draw end
		/////////////////////

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		//s32 shor = AESysDoesWindowExist();
		if (AEInputCheckTriggered(AEVK_ESCAPE) || !AESysDoesWindowExist())
			gGameRunning = 0;
	}
	// free the system
	AESysExit();
	// free singleton resources
	SceneManager::Instance().Free();
	SceneManager::Instance().Unload();
	// free chunk data resources
}