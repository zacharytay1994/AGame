// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"



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


	/////////////////
	// Initialization

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);


	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	// Initialization end
	/////////////////////



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
	AEGfxTexture* tex1 = AEGfxTextureLoad("../bin/Assets/test2.png");
	AEGfxMeshStart();
	// create triangle one
	AEGfxTriAdd(-100.0f, -100.0f, 0x00FF00FF, 1.0f, 0.0f,
		100.0f, -100.0f, 0x00FF00FF, 1.0f, 1.0f,
		-100.0f, 100.0f, 0x00FF00FF, 0.0f, 0.0f);
	AEGfxTriAdd(100.0f, -100.0f, 0x00FF00FF, 1.0f, 1.0f,
		100.0f, 100.0f, 0x00FF00FF, 1.0f, 0.0f,
		-100.0f, 100.0f, 0x00FF00FF, 0.0f, 0.0f);
	AEGfxVertexList* mesh = AEGfxMeshEnd();

	AEGfxMeshStart();
	// This shape has 5 vertices
	AEGfxVertexAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(100.0f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f);
	AEGfxVertexAdd(200.0f, 150.0f, 0xFFFFFFFF, 0.0f, 1.0f);
	AEGfxVertexAdd(300.0f, -100.0f, 0xFFFFFFFF, 1.0f, 0.0f);
	AEGfxVertexAdd(100.0f, -250.0f, 0xFFFFFFFF, 1.0f, 0.0f);
	AEGfxVertexList* mesh2 = AEGfxMeshEnd();


	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxTextureSet(tex1, 0.0f, 0.0f);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(mesh2, AE_GFX_MDM_LINES_STRIP);

		AEGfxMeshDraw(mesh2, AE_GFX_MDM_TRIANGLES);
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
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}


	// free the system
	AESysExit();
}