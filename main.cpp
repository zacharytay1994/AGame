// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "AGame/zEntity.h"
#include "AGame/zSystem.h"
#include "AGame/Scene.h"

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

	///////////////////////
	// Variable declaration

	int gGameRunning = 1;

	// Variable declaration end
	///////////////////////////
	/*zEntity entity;
	ComponentMask mask;
	mask[0] = 1;
	zSystem system{ mask };*/
	
	/////////////////
	// Initialization


	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);


	// Changing the window title
	AESysSetWindowTitle("A Beautiful Window!");

	// reset the system modules
	AESysReset();

	SceneManager::Instance();
	SceneManager::Instance().ChangeScene("TestScenewilfred");


	// Initialization end
	/////////////////////
	/*ComponentDescription_DB _cdb;
	_cdb.RegisterComponent<Position>();
	_cdb.RegisterComponent<Velocity>();

	SystemDatabase::Instance().AddSystem<Position,Velocity>(UpdatePosition);
	SystemDatabase::Instance().AddSystem<Position>(PrintPosition);

	Entity<Position, Velocity> e1;
	Entity<Position, Velocity> e2;*/

	////////////////////////////////
	// Creating the objects (Shapes)

	// Creating the objects (Shapes) end
	////////////////////////////////////



	////////////////////////////
	// Loading textures (images)

	// Loading textures (images) end
	//////////////////////////////////

	//////////////////////////////////
	// Creating Fonts	

	// Creating Fonts end
	//////////////////////////////////
	//AEGfxTexture* tex1 = AEGfxTextureLoad("../bin/Assets/test2.png");
	//AEGfxMeshStart();
	//// create triangle one
	//AEGfxTriAdd(-100.0f, -100.0f, 0x00FF00FF, 1.0f, 0.0f,
	//	100.0f, -100.0f, 0x00FF00FF, 1.0f, 1.0f,
	//	-100.0f, 100.0f, 0x00FF00FF, 0.0f, 0.0f);
	//AEGfxTriAdd(100.0f, -100.0f, 0x00FF00FF, 1.0f, 1.0f,
	//	100.0f, 100.0f, 0x00FF00FF, 1.0f, 0.0f,
	//	-100.0f, 100.0f, 0x00FF00FF, 0.0f, 0.0f);
	//AEGfxVertexList* mesh = AEGfxMeshEnd();

	//AEGfxMeshStart();
	//// This shape has 5 vertices
	//AEGfxVertexAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	//AEGfxVertexAdd(100.0f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f);
	//AEGfxVertexAdd(200.0f, 150.0f, 0xFFFFFFFF, 0.0f, 1.0f);
	//AEGfxVertexAdd(300.0f, -100.0f, 0xFFFFFFFF, 1.0f, 0.0f);
	//AEGfxVertexAdd(100.0f, -250.0f, 0xFFFFFFFF, 1.0f, 0.0f);
	//AEGfxVertexList* mesh2 = AEGfxMeshEnd();
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		SceneManager::Instance().Update((float)AEFrameRateControllerGetFrameTime());
		//std::cout << com.y << std::endl;
		/*AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxTextureSet(tex1, 0.0f, 0.0f);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(mesh2, AE_GFX_MDM_LINES_STRIP);
		AEGfxMeshDraw(mesh2, AE_GFX_MDM_TRIANGLES);*/
		///////////////////
		// Game loop update

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
	SceneManager::Instance().Free();
	SceneManager::Instance().Unload();
}