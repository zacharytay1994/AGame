/******************************************************************************/
/*!
\File Name		: main.cpp
\Project Name	: AGame
\Authors 		:
				Primary - Zachary Tay (100%)
				Secondary -
\brief		Entry point of AGame

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/


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

// ---------------------------------------------------------------------------

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
	
	/////////////////
	// Initialization

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);
	//full screen
	//AEToogleFullScreen(true);
	AEInputShowCursor(0);
	// music
	ResourceManager::Instance().CreateMusic();

	// Changing the window title
	AESysSetWindowTitle("AGame");

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
				ShowWindow(AESysGetWindowHandle(), SW_MINIMIZE);
			}
		}

		///////////////////
		// Game loop update
		SceneManager::Instance().CheckGame(gGameRunning);

		if (GetFocus() != AESysGetWindowHandle())	// If the game is not in focus
		{
			SceneManager::Instance()._pause = true;	// Pause the game
			ResourceManager::Instance().ToggleMuteMusic(0);	// Mute the music
		}
		else
		{
			// If the game is not paused by player, unpause it
			if(!SceneManager::Instance()._settings_toggle) { SceneManager::Instance()._pause = false; }
			if (SceneManager::Instance()._musicmmute == false) {
				ResourceManager::Instance().ToggleMuteMusic(1);	// Unmute the music
			}
		}

		g_dt = (float)AEFrameRateControllerGetFrameTime();	// Get framerate
		if (SceneManager::Instance()._pause) // If the game is paused
		{ 
			ResourceManager::Instance().ToggleMuteMusic(0);	// Mute the music
			g_dt = 0.f; // Set the dt to 0, disables most of the systems
		}

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
	// free chunk data resources
}