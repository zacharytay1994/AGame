#include "Scene.h"
#include "SceneDeclarations.h"
#include "zSystem.h"
#include "CSHeaderDef.h"
#include "ResourceManager.h"
#include <iostream>

SceneManager::SceneManager()
{
	Initialize();
}

SceneManager& SceneManager::Instance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::Initialize() {

	// Initialize Resource Manager
	ResourceManager::Instance();

	// Registering all components for the game
	ComponentDescription_DB::Instance().RegisterComponent<Position>();
	ComponentDescription_DB::Instance().RegisterComponent<Example_Velocity>();

	// Registering all systems for the game
	SystemDatabase::Instance().RegisterSystem<Position>(Example_PrintPosition);
	//SystemDatabase::Instance().RegisterSystem<Example_PrintPositionSys, Example_Position>();
	SystemDatabase::Instance().RegisterSystem<Example_UpdatePosition, Position, Example_Velocity>();

	// Registering scenes
	AddScene<TestScene>("Test Scene");
	AddScene<TestScene2>("Test Scene 2");
}

void SceneManager::ChangeScene(const std::string& name) {
	assert(_scenes.find(name) != _scenes.end());
	if (_current_scene) {
		_current_scene->Exit();
		std::cout << "SCENE |" << _current_scene_name << "| EXITED." << std::endl;
		// reload the scene into memory
		_scenes[_current_scene_name] = std::make_shared<Scene>();
	}
	ArchetypeDatabase::Instance().FlushEntities();
	_current_scene = _scenes[name];
	_current_scene_name = name;
	_current_scene->Initialize();
	std::cout << "SCENE |" << _current_scene_name << "| INITIALIZED." << std::endl;
}

//void SceneManager::AddScene(const std::string& name/*, scene_initialize init, scene_update update, scene_exit exit*/)
//}

void SceneManager::Update(const float& dt)
{
	if (_current_scene) {
		_current_scene->Update(dt);
		SystemDatabase::Instance().SystemDatabaseUpdate((float)AEFrameRateControllerGetFrameTime());
	}
}

void Scene::Initialize()
{
}

void Scene::Update(const float& dt)
{
}

void Scene::Exit()
{
}
