#include "Scene.h"
#include "SceneDeclarations.h"
#include "zSystem.h"
#include "CSHeaderDef.h"
#include "ResourceManager.h"

#include "Scene_MainMenu.h"
#include "Scene_Zac.h"

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
	//ComponentDescription_DB::Instance().RegisterComponent<Com_PathFinding>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Direction>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_WeaponAttack>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_BoundingBox>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Boundary>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Wave>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_EnemySpawn>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_TypeEnemy>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Projectile>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GameTimer>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_YLayering>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Particle>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_ParticleEmitter>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_objecttype>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_BoundingBox>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_CollisionData>();

	// Pathfinding
	ComponentDescription_DB::Instance().RegisterComponent<Com_FindPath>();

	// GUI COMPONENTS
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUISurface>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIMouseCheck>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIOnClick>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_Text>();
	ComponentDescription_DB::Instance().RegisterComponent<Com_GUIDrag>();

	// enemy states
	ComponentDescription_DB::Instance().RegisterComponent<Com_EnemyStateOne>();

	// 3. Registering all systems for the game
	//SystemDatabase::Instance().RegisterSystem<Example_UpdatePosition, Position, Example_Velocity>();
	SystemDatabase::Instance().RegisterSystem<Sys_Tilemap, Com_Tilemap>();
	SystemDatabase::Instance().RegisterSystem<Sys_DrawSprite, Com_Position, Com_Sprite>();
	SystemDatabase::Instance().RegisterSystem<Sys_ArrowKeys, Com_Position, Com_ArrowKeys>();
	SystemDatabase::Instance().RegisterSystem<Sys_TilemapPosition, Com_Tilemap, Com_Position>();
	SystemDatabase::Instance().RegisterSystem<Sys_TilePosition, Com_TilemapRef, Com_TilePosition, Com_Position>();
	// SystemDatabase::Instance().RegisterSystem<Sys_ArrowKeysTilemap, Com_TilePosition>();
	SystemDatabase::Instance().RegisterSystem<Sys_PathFinding, Com_Node,Com_TilePosition,Com_TilemapRef>();
	SystemDatabase::Instance().RegisterSystem<Sys_ArrowKeysTilemap, Com_TilePosition, Com_ArrowKeysTilemap, Com_Direction>();
	SystemDatabase::Instance().RegisterSystem<Sys_PlayerAttack, Com_Direction, Com_WeaponAttack, Com_TilePosition, Com_Projectile>();
	SystemDatabase::Instance().RegisterSystem<Sys_GameTimer, Com_GameTimer>();
	SystemDatabase::Instance().RegisterSystem<Sys_EnemyAttack, Com_Direction, Com_TypeEnemy, Com_TilePosition, Com_Tilemap>();
	SystemDatabase::Instance().RegisterSystem<Sys_EnemySpawning, Com_EnemySpawn, Com_Wave>();
	SystemDatabase::Instance().RegisterSystem<Sys_Velocity, Com_Position, Com_Velocity>();
	SystemDatabase::Instance().RegisterSystem<Sys_ArrowKeysTilemap, Com_TilePosition, Com_ArrowKeysTilemap>();
	SystemDatabase::Instance().RegisterSystem<Sys_YLayering, Com_Sprite, Com_Position, Com_YLayering>();
	SystemDatabase::Instance().RegisterSystem<Sys_Boundary, Com_Position, Com_Boundary>();
	SystemDatabase::Instance().RegisterSystem<Sys_ParticleSys,Com_Particle, Com_GameTimer >();
	SystemDatabase::Instance().RegisterSystem<Sys_ParticleEmitter, Com_ParticleEmitter, Com_GameTimer>();
	SystemDatabase::Instance().RegisterSystem<Sys_RegisteringEntity, Com_objecttype>();
	//test 
	SystemDatabase::Instance().RegisterSystem <Sys_Boundingbox, Com_BoundingBox, Com_Position,Com_Sprite>();
	SystemDatabase::Instance().RegisterSystem <Sys_AABB, Com_BoundingBox, Com_Velocity, Com_CollisionData, Com_objecttype>();

	SystemDatabase::Instance().RegisterSystem<Sys_Projectile2, Com_TilePosition, Com_Projectile>();

	// GUI SYSTEMS
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceRender, Com_Position, Com_GUISurface, Com_Sprite>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceMouseCheck, Com_GUIMouseCheck, Com_Position, Com_GUISurface>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUISurfaceOnClick, Com_GUIOnClick, Com_GUIMouseCheck, Com_GUISurface>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUIDrag, Com_GUIMouseCheck, Com_GUIDrag, Com_GUISurface>();
	SystemDatabase::Instance().RegisterSystem<Sys_GUITextRender, Com_Position, Com_GUISurface, Com_Text>();

	// pathfinding
	SystemDatabase::Instance().RegisterSystem<Sys_Pathfinding_v2, Com_FindPath>();

	// enemy states
	SystemDatabase::Instance().RegisterSystem<Sys_EnemyStateOne, Com_EnemyStateOne, Com_FindPath, Com_TilePosition>();


	// 4. Registering scenes
	AddScene<TestScene>("Test Scene");
	//AddScene<TestScene2>("Test Scene 2");
	AddScene<TestScenePF>("Test PathFinding");
	//AddScene<ExampleScene>("ExampleScene");
	AddScene<MainMenu>("Main Menu");
	AddScene<TestScenewilfred>("TestScenewilfred");
	AddScene<ShootingRange>("ShootingRange");
}

void SceneManager::Free()
{
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
	assert(_scenes.find(name) != _scenes.end());
	if (_current_scene) {
		// calls exit and unload
		_current_scene->Exit();
		_current_scene->Unload();
		std::cout << "SCENE |" << _current_scene_name << "| FREED AND UNLOADED." << std::endl;
		// reload the scene into memory
		//_scenes[_current_scene_name] = std::make_shared<Scene>();
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
	if (_current_scene) {
		_current_scene->Update(dt);
		if (AEInputCheckTriggered('H')) {
			SceneManager::Instance().ChangeScene("Main Menu");
		}
		SystemDatabase::Instance().SystemDatabaseUpdate((float)AEFrameRateControllerGetFrameTime());
		ResourceManager::Instance().FlushDraw();
		ResourceManager::Instance().FlushDrawText();
		SystemDatabase::Instance().RemoveAllEntities();
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
	ArchetypeDatabase::Instance().FlushEntities();
	ResourceManager::Instance().ResetRenderQueue();
}

void Scene::Load()
{
}

void Scene::Draw(const float& dt)
{
}

void Scene::Unload()
{
	ResourceManager::Instance().ResetRenderQueue();
}
