// ---------------------------------------------------------------------------
// includes
#include "AEEngine.h"
#include "AGame/zEntity.h"
#include "AGame/zSystem.h"
#include "AGame/Scene.h"
#include "AGame/CSHeaderDef.h"
#include "AGame/Inventory.h"

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
// main - ENGINE PROOF

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
	float g_dt = 0.f;
	bool fullscreen{ false };
	
	/*Vec2f a{ 1.0f, 1.0f };
	Vec2f b{ 2.0f,2.0f };*/
	//std::cout << "hgello" << std::endl;
	//std::cout << a.x << ":" << a.y << std::endl;
 	//std::cout << a << std::endl;
	/////////////////
	// Initialization

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);
	//AEToogleFullScreen(true);
	AEInputShowCursor(0);
	// music
	ResourceManager::Instance().CreateMusic();

	// Changing the window title
	AESysSetWindowTitle("A Beautiful Game!");

	// reset the system modules
	AESysReset();

	SceneManager::Instance();
	SceneManager::Instance().ChangeScene("Opening");
	//SceneManager::Instance().ChangeScene("Test PathFinding");
	AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		if (GetAsyncKeyState(AEVK_LALT)) {
			if (AEInputCheckTriggered(AEVK_RETURN)) {
				fullscreen = !fullscreen;
				AEToogleFullScreen(fullscreen);
			}
			else if (GetAsyncKeyState(AEVK_TAB)) {
				
			}
		}

		///////////////////
		// Game loop update
		SceneManager::Instance().CheckGame(gGameRunning);

		if (GetFocus() != AESysGetWindowHandle())
		{
			SceneManager::Instance()._pause = true;
			ResourceManager::Instance().ToggleMuteMusic(0);
		}
		else
		{
			if(!SceneManager::Instance()._settings_toggle) { SceneManager::Instance()._pause = false; }
			ResourceManager::Instance().ToggleMuteMusic(1);
		}

		g_dt = (float)AEFrameRateControllerGetFrameTime();
		if (SceneManager::Instance()._pause) { g_dt = 0.f; }

		SceneManager::Instance().Update(g_dt);
		ResourceManager::Instance().UpdateAndPlayMusic();
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
		if (!AESysDoesWindowExist())
			gGameRunning = 0;
	}
	// free the system
	AESysExit();
	// free singleton resources
	ResourceManager::Instance().FreeMusic();
	SceneManager::Instance().Free();
	SceneManager::Instance().Unload();
	//Inventory::Inventory_Free();
	// free chunk data resources
}