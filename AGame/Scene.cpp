/******************************************************************************/
/*!
\File Name		: Scene.cpp
\Project Name	: AGame
\Authors 		:
				Primary - Zachary Tay (100%)
				Secondary -
\brief		Scene handling for AGame

All content � 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/

#include "Scene.h"
#include "SceneDeclarations.h"
#include "zSystem.h"
#include "CSHeaderDef.h"
#include "ResourceManager.h"
#include "Scene_LevelEditor.h"
#include "Scene_LevelEditor2.h"
#include "Scene_MainMenu.h"
#include "Scene_Opening.h"
#include "Scene_Credits.h"
#include "Scene_Inventory.h"
#include "Scene_LevelSelect.h"
#include "Scene_Instruction.h"
#include "Scene_LevelSelectNormal.h"
#include "Scene_Levels.h"
#include <iostream>

/*______________________________________________________
* Brief:	Constructor of scene manager.
* Access:	private (singleton)
________________________________________________________*/
SceneManager::SceneManager()
{
	Initialize();
}

/*______________________________________________________
* Brief:	Handle to the singleton instance.
* Access:	public
*
* Use e.g:	SceneManager::Instance().ExampleFunction()
*
* \return	Reference to a scenemanager static instance.
________________________________________________________*/
SceneManager& SceneManager::Instance()
{
	static SceneManager instance;
	return instance;
}

/*______________________________________________________
* Brief:	Initializes a few things.
*			1. ResourceManager singleton
*			2. Registering components
*			3. Registering systems
*			4. Registering scenes
* Access:	private
*
* Called:	In SceneManager::SceneManager()
*
* \return	Reference to a scenemanager static instance.
________________________________________________________*/
void SceneManager::Initialize() {

	// 1. Initialize Resource Manager
	ResourceManager::Instance();
	// set default texture settings
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 2. Registering all components for the game
	ComponentDescription_DB::Instance().RegisterComponent<Com_Position>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Velocity>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Sprite>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_ArrowKeys>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Tilemap>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_TilemapRef>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_TilePosition>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_ArrowKeysTilemap>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Node>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Direction>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_WeaponAttack>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_BoundingBox>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Boundary>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Wave>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_EnemySpawn>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Boss>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_TypeEnemy>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Projectile>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GameTimer>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_YLayering>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Particle>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_ParticleEmitter>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_CollisionData>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Camera>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_ParentPosition>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_type>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GridColData>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Health>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIMap>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_BoundingBoxGUI>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_instructionsGUI>();

	// Pathfinding
	ComponentDescription_DB::Instance().RegisterComponent<Com_FindPath>();

	// GUI Components
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUISurface>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIMouseCheck>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIOnClick>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Text>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIDrag>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUISurfaceHoverShadow>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUISurfaceHoverShadow_Inventory>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUItextboxinput>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Writetofile>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUItextboxinputwords>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_errormessageGUI>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_TextMovingGUI>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIDelay>();

	// Enemy States
	ComponentDescription_DB::Instance().RegisterComponent<Com_EnemyStateOne>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_TileMoveSpriteState>();

	// particle
	ComponentDescription_DB::Instance().RegisterComponent<Com_ParticleFriction>();

	ComponentDescription_DB::Instance().RegisterComponent<Com_FadeOut>();

	
	// 3. Registering all systems for the game
	SystemDatabase::Instance().RegisterSystem<Sys_Tilemap, Com_Tilemap>();
	SystemDatabase::Instance().RegisterSystem<Sys_DrawSprite, Com_Position, Com_Sprite>();
	SystemDatabase::Instance().RegisterSystem<Sys_ArrowKeys, Com_Position, Com_ArrowKeys>();
	SystemDatabase::Instance().RegisterSystem<Sys_ArrowKeysTilemap, Com_TilePosition, Com_ArrowKeysTilemap>();
	SystemDatabase::Instance().RegisterSystem<Sys_TilemapPosition, Com_Tilemap, Com_Position>();
	SystemDatabase::Instance().RegisterSystem<Sys_TilePosition, Com_TilemapRef, Com_TilePosition, Com_Position>();
	SystemDatabase::Instance().RegisterSystem<Sys_PlayerAttack, Com_Direction, Com_WeaponAttack, Com_TilePosition, Com_Projectile>();
	SystemDatabase::Instance().RegisterSystem<Sys_GameTimer, Com_GameTimer>();
	SystemDatabase::Instance().RegisterSystem<Sys_Velocity, Com_Position, Com_Velocity>();
	SystemDatabase::Instance().RegisterSystem<Sys_YLayering, Com_Sprite, Com_Position, Com_YLayering>();
	SystemDatabase::Instance().RegisterSystem<Sys_Boundary, Com_Position, Com_Boundary>();
	SystemDatabase::Instance().RegisterSystem<Sys_ParticleSys,Com_Particle, Com_GameTimer >();
	SystemDatabase::Instance().RegisterSystem<Sys_ParticleEmitter, Com_ParticleEmitter, Com_GameTimer, Com_TilePosition>();
	SystemDatabase::Instance().RegisterSystem<Sys_Obstacle, Com_type,Com_Health,Com_TilePosition, Com_ParticleEmitter>();
	SystemDatabase::Instance().RegisterSystem <Sys_Boundingbox, Com_BoundingBox, Com_Position,Com_Sprite>();
	SystemDatabase::Instance().RegisterSystem <Sys_AABB, Com_BoundingBox, Com_Velocity, Com_CollisionData, Com_type,Com_Health, Com_Position>();
	SystemDatabase::Instance().RegisterSystem<Sys_Projectile2, Com_TilePosition, Com_Projectile>();
	SystemDatabase::Instance().RegisterSystem<Sys_Camera, Com_Position, Com_Camera>();
	SystemDatabase::Instance().RegisterSystem<Sys_GridCollision, Com_type, Com_TilePosition, Com_GridColData>();
	SystemDatabase::Instance().RegisterSystem<Sys_TileMoveSpriteState, Com_Sprite, Com_TilePosition, Com_TileMoveSpriteState>();
	SystemDatabase::Instance().RegisterSystem<Sys_ParentPosition, Com_ParentPosition>();
	SystemDatabase::Instance().RegisterSystem<Sys_FadeOut, Com_FadeOut, Com_Sprite>();

	// GUI SYSTEMS
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceRender, Com_Position, Com_GUISurface, Com_Sprite>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceMouseCheck, Com_GUIMouseCheck, Com_Position, Com_GUISurface>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceOnClick, Com_GUIOnClick, Com_GUIMouseCheck, Com_GUISurface>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUIDrag, Com_GUIMouseCheck, Com_GUIDrag, Com_GUISurface>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUITextRender, Com_Position, Com_GUISurface, Com_Text>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceHoverShadow, Com_GUISurfaceHoverShadow>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceHoverShadow_Inventory, Com_GUISurfaceHoverShadow_Inventory>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUItextboxinput, Com_GUItextboxinput, Com_Text>();
	SystemDatabase::Instance().RegisterSystem<Sys_writetofile, Com_Tilemap, Com_Writetofile, Com_GUIMouseCheck>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUItextboxinputwords, Com_GUItextboxinputwords, Com_Text>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUIMapClick, Com_GUIMap, Com_Tilemap, Com_BoundingBoxGUI>();
	SystemDatabase::Instance().RegisterSystem<Sys_errormessageGUI, Com_errormessageGUI>();
	SystemDatabase::Instance().RegisterSystem<Sys_InstructionsGUI, Com_instructionsGUI>();
	SystemDatabase::Instance().RegisterSystem<Sys_TextMovingGUI, Com_TextMovingGUI, Com_GUISurface>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUIDelay, Com_GUIDelay, Com_GUISurface>();

	// pathfinding
	SystemDatabase::Instance().RegisterSystem<Sys_PathFinding, Com_type, Com_FindPath>();

	// enemy states, spawn, attack
	SystemDatabase::Instance().RegisterSystem<Sys_EnemyStateOne, Com_EnemyStateOne, Com_FindPath, Com_TilePosition, Com_Sprite, Com_type>();
	SystemDatabase::Instance().RegisterSystem<Sys_EnemyStateBoss,Com_TilePosition, Com_Sprite, Com_type, Com_Boss,Com_GameTimer>();
	SystemDatabase::Instance().RegisterSystem<Sys_EnemySpawning, Com_EnemySpawn, Com_Wave, Com_Boss>();
	
	//Health test
	SystemDatabase::Instance().RegisterSystem<Sys_HealthUpdate, Com_Health>();

	// particle
	SystemDatabase::Instance().RegisterSystem<Sys_ParticleFriction, Com_Position, Com_ParticleFriction, Com_Sprite>();


	// 4. Registering scenes
	AddScene<TestScenePF>("Test PathFinding");
	AddScene<MainMenu>("Main Menu");
	AddScene<Opening>("Opening");
	AddScene<LevelEditor>("Leveleditor");
	AddScene<LevelEditor2>("Leveleditor2");
	AddScene<InventoryMenu>("InventoryMenu");
	AddScene<LevelSelect>("LevelSelect");
	AddScene<Scene_Instructions>("Instructions");
	AddScene<Scene_Credits>("Credits");
	AddScene<LevelSelectNormal>("LevelSelectNormal");
	AddScene<Level>("Level");
}

void SceneManager::Free()
{
	if (_current_scene) {
		_current_scene->Exit();
	}
	ResourceManager::Instance().ResetRenderQueue();
	ResourceManager::Instance().ResetTextStack();
	ResourceManager::Instance().FreeResources();
}

/*______________________________________________________
* Brief:	Exits the current scene and initializes
*			the next scene.
*
* Access:	public
*
* Used e.g:	SceneManager::Instance().ChangeScene("myScene");
*
* arg[in]:	name
*			- The name of the scene to change to,
*			  assigned at SceneManager::AddScene()
________________________________________________________*/
void SceneManager::ChangeScene(const std::string& name) {
	if (!_lock) {
		_delay = 0.5f;
		_next_scene = name;
		ResourceManager::Instance()._panel_timer = 3.14159f;
		_lock = true;
	}
}

void SceneManager::ChangeSceneNow(const std::string& name)
{
	_pause = false;
	_settings_toggle = false;
	assert(_scenes.find(name) != _scenes.end());
	if (_current_scene) {
		// calls exit and unload
		_current_scene->Exit();
		_current_scene->Unload();
		std::cout << "SCENE |" << _current_scene_name << "| FREED AND UNLOADED." << std::endl;
	}
	ResourceManager::Instance().ResetRenderQueue();
	ResourceManager::Instance().ResetTextStack();
	ArchetypeDatabase::Instance().FlushEntities();
	_current_scene = _scenes[name];
	_current_scene_name = name;
	_current_scene->Load();
	_current_scene->Initialize();
	std::cout << "SCENE |" << _current_scene_name << "| LOADED AND INITIALIZED." << std::endl;
}

/*______________________________________________________
* Brief:	Updates the current scene bound to the
*			SceneManager.
*
* Access:	public (needs to be called in main.cpp)
*
* called:	In main.cpp
*
* arg[in]:	dt
*			- Delta/Frame time between frames in seconds.
________________________________________________________*/
void SceneManager::Update(const float& dt)
{
	if (_delay > 0.0f) {
		_delay -= (float)AEFrameRateControllerGetFrameTime();
	}
	else {
		if (_next_scene != "") {
			_lock = false;
			ChangeSceneNow(_next_scene);
			_next_scene = "";
			return;
		}
	}
	if (_current_scene) {
		_current_scene->Update(dt);
		float _dt = _pause ? 0.0f : static_cast<float>(AEFrameRateControllerGetFrameTime());
		SystemDatabase::Instance().SystemDatabaseUpdate(_dt);
		ResourceManager::Instance().FlushDraw();
		ResourceManager::Instance().FlushDrawText();
		ResourceManager::Instance().Update(_dt);
		SystemDatabase::Instance().RemoveAllEntities();
	}
	// draw cursor
	if (_currentlyplaying == false) {
		ResourceManager::Instance().DrawScenePanels((float)AEFrameRateControllerGetFrameTime());
		ResourceManager::Instance().CursorParticlesUpdate((float)AEFrameRateControllerGetFrameTime());
		ResourceManager::Instance().DrawCursor();
	}
}

/*______________________________________________________
* Brief:	Virtual (overridable) function for
*			derived systems.
*
* Access:	private
*			- friend class SceneManager
*
* called:	SceneManager::ChangeScene();
________________________________________________________*/
void Scene::Initialize()
{
}

/*______________________________________________________
* Brief:	Virtual (overridable) function for
*			derived systems.
*
* Access:	private
*			- friend class SceneManager
*
* called:	SceneManager::Update();
________________________________________________________*/
void Scene::Update(const float& dt)
{
	UNREFERENCED_PARAMETER(dt);
}

/*______________________________________________________
* Brief:	Virtual (overridable) function for
*			derived systems.
*
* Access:	private
*			- friend class SceneManager
*
* called:	SceneManager::ChangeScene();
________________________________________________________*/
void Scene::Exit()
{
}
// new functions
void SceneManager::RestartScene()
{
	if (_current_scene) {
		_current_scene->Exit();
		std::cout << "Scene Freed." << std::endl;
		ResourceManager::Instance().ResetRenderQueue();
		ArchetypeDatabase::Instance().FlushEntities();
		Factory::Instance().Free();
		_current_scene->Initialize();
		std::cout << "Scene Initialized." << std::endl;
	}
}

void SceneManager::Load()
{
}

void SceneManager::Draw(const float& dt)
{
	if (_current_scene) {
		_current_scene->Draw(dt);
	}
}

void SceneManager::Unload()
{
	if (_current_scene) {
		_current_scene->Unload();
	}
	ArchetypeDatabase::Instance().FlushEntities();
	ResourceManager::Instance().ResetRenderQueue();
}

void Scene::Load()
{
}

void Scene::Draw(const float& dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Scene::Unload()
{
	ResourceManager::Instance().ResetRenderQueue();
}
